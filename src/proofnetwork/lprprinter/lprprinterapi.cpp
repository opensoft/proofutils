#include "lprprinterapi.h"

#include "proofnetworklprprinter_types.h"

#include "proofnetwork/lprprinter/errormessages_p.h"
#include "proofnetwork/proofservicerestapi_p.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace Proof {
namespace NetworkServices {

class LprPrinterApiPrivate : public ProofServiceRestApiPrivate
{
    Q_DECLARE_PUBLIC(LprPrinterApi)

    LprPrinterApiPrivate() : ProofServiceRestApiPrivate(lprPrinterServiceErrors()) {}

    auto printerStatusUnmarshaller()
    {
        return [](const RestApiReply &reply) -> LprPrinterStatus {
            QJsonParseError jsonError;
            QJsonDocument doc = QJsonDocument::fromJson(reply.data, &jsonError);
            if (jsonError.error != QJsonParseError::NoError) {
                return WithFailure(QStringLiteral("JSON error: %1").arg(jsonError.errorString()),
                                   NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply, Failure::NoHint,
                                   jsonError.error);
            }
            if (!doc.isObject()) {
                return WithFailure(QStringLiteral("Object is not found in document"), NETWORK_LPR_PRINTER_MODULE_CODE,
                                   NetworkErrorCode::InvalidReply);
            }
            QJsonObject object = doc.object();
            return LprPrinterStatus{object.value("is_ready").toBool(), object.value("reason").toString()};
        };
    }

    auto discardingPrinterStatusUnmarshaller()
    {
        return [](const RestApiReply &reply) -> bool {
            QJsonParseError jsonError;
            QJsonDocument doc = QJsonDocument::fromJson(reply.data, &jsonError);
            if (jsonError.error != QJsonParseError::NoError) {
                return WithFailure(QStringLiteral("JSON error: %1").arg(jsonError.errorString()),
                                   NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply, Failure::NoHint,
                                   jsonError.error);
            }
            if (!doc.isObject()) {
                return WithFailure(QStringLiteral("Object is not found in document"), NETWORK_LPR_PRINTER_MODULE_CODE,
                                   NetworkErrorCode::InvalidReply);
            }
            QJsonObject object = doc.object();
            if (!object.value("is_ready").toBool()) {
                return WithFailure(object.value("reason").toString(), NETWORK_LPR_PRINTER_MODULE_CODE,
                                   NetworkErrorCode::ServerError);
            }
            return true;
        };
    }
};
} // namespace NetworkServices
} // namespace Proof

using namespace Proof;
using namespace Proof::NetworkServices;

LprPrinterApi::LprPrinterApi(const RestClientSP &restClient, QObject *parent)
    : ProofServiceRestApi(restClient, *new LprPrinterApiPrivate, parent)
{}

CancelableFuture<LprPrinterStatus> LprPrinterApi::fetchStatus(const QString &printer)
{
    Q_D(LprPrinterApi);
    QUrlQuery query;
    if (!printer.isEmpty())
        query.addQueryItem(QStringLiteral("printer"), printer);
    return d->unmarshalReply(d->get(QStringLiteral("/lpr/status"), query), d->printerStatusUnmarshaller());
}

CancelableFuture<bool> LprPrinterApi::printLabel(const QByteArray &label, const QString &printer)
{
    Q_D(LprPrinterApi);
    QUrlQuery query;
    if (!printer.isEmpty())
        query.addQueryItem(QStringLiteral("printer"), printer);
    return d->unmarshalReply(d->post(QStringLiteral("/lpr/print-raw"), query,
                                     QStringLiteral("{\"data\": \"%1\"}").arg(label.toBase64().constData()).toLatin1()),
                             d->discardingPrinterStatusUnmarshaller());
}

CancelableFuture<bool> LprPrinterApi::printFile(const QString &fileName, const QString &printer, unsigned int copies)
{
    Q_D(LprPrinterApi);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(proofNetworkLprPrinterLog) << "Lpr-Printer: file error:" << file.error() << file.errorString();
        return d->invalidArgumentsFailure<bool>(
            Failure(QStringLiteral("Can't open file"), NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::FileError));
    }
    QByteArray data = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        qCWarning(proofNetworkLprPrinterLog) << "Lpr-Printer: file error:" << file.error() << file.errorString();
        return d->invalidArgumentsFailure<bool>(
            Failure(QStringLiteral("Can't read file"), NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::FileError));
    }
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("copies"), QString::number(copies));
    if (!printer.isEmpty())
        query.addQueryItem(QStringLiteral("printer"), printer);
    return d->unmarshalReply(d->post(QStringLiteral("/lpr/print"), query, data),
                             d->discardingPrinterStatusUnmarshaller());
}

CancelableFuture<QVector<LprPrinterInfo>> LprPrinterApi::fetchPrintersList()
{
    Q_D(LprPrinterApi);
    auto unmarshaller = [](const RestApiReply &reply) -> QVector<LprPrinterInfo> {
        QJsonParseError jsonError;
        QJsonDocument doc = QJsonDocument::fromJson(reply.data, &jsonError);
        if (jsonError.error != QJsonParseError::NoError) {
            return WithFailure(QStringLiteral("JSON error: %1").arg(jsonError.errorString()),
                               NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply, Failure::NoHint,
                               jsonError.error);
        }
        if (!doc.isArray()) {
            return WithFailure(QStringLiteral("Array is not found in document"), NETWORK_LPR_PRINTER_MODULE_CODE,
                               NetworkErrorCode::InvalidReply);
        }
        QVector<LprPrinterInfo> printers;
        const QJsonArray array = doc.array();
        for (const QJsonValue &value : array) {
            QJsonObject object = value.toObject();
            QString printer = object.value(QStringLiteral("printer")).toString();
            if (!printer.isEmpty())
                printers << LprPrinterInfo{printer, object.value("accepts_raw").toBool(),
                                           object.value("accepts_files").toBool()};
        }
        return printers;
    };
    return d->unmarshalReply(d->get(QStringLiteral("/lpr/list")), unmarshaller);
}
