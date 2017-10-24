#include "lprprinterapi.h"

#include "proofnetworklprprinter_types.h"

#include "proofnetwork/proofservicerestapi_p.h"
#include "proofnetwork/lprprinter/errormessages_p.h"

#include <QSet>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

namespace Proof {
namespace NetworkServices {

class LprPrinterApiPrivate : public ProofServiceRestApiPrivate
{
    Q_DECLARE_PUBLIC(LprPrinterApi)

    LprPrinterApiPrivate() : ProofServiceRestApiPrivate(lprPrinterServiceErrors()) {}

    bool parsePrinterStatus(qulonglong operationId, QNetworkReply *reply, LprPrinterStatus &result);
};

LprPrinterApi::LprPrinterApi(const RestClientSP &restClient, QObject *parent)
    : ProofServiceRestApi(restClient, *new LprPrinterApiPrivate, parent)
{
}

qulonglong LprPrinterApi::fetchStatus(const QString& printer)
{
    Q_D(LprPrinterApi);
    qulonglong operationId = 0;
    if (!call(this, &LprPrinterApi::fetchStatus, Proof::Call::BlockEvents, operationId, printer)) {
        RestAnswerHandler handler = [this, d](qulonglong operationId, QNetworkReply *reply) {
            LprPrinterStatus status;
            if (d->parsePrinterStatus(operationId, reply, status))
                emit statusFetched(operationId, status);
        };
        QUrlQuery query;
        if (!printer.isEmpty())
            query.addQueryItem(QStringLiteral("printer"), printer);
        d->get(operationId, std::move(handler), QStringLiteral("/lpr/status"), query);
    }
    return operationId;
}

qulonglong LprPrinterApi::printLabel(const QByteArray &label, const QString& printer)
{
    Q_D(LprPrinterApi);
    qulonglong operationId = 0;
    if (!call(this, &LprPrinterApi::printLabel, Proof::Call::BlockEvents, operationId, label, printer)) {
        RestAnswerHandler handler = [this, d](qulonglong operationId, QNetworkReply *reply) {
            LprPrinterStatus status;
            if (d->parsePrinterStatus(operationId, reply, status)) {
                if (status.isReady)
                    emit labelPrinted(operationId);
                else
                    emit apiErrorOccurred(operationId, {RestApiError::Level::ServerError, 0,
                                                     NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::ServerError,
                                                     status.reason});
            }
        };
        QUrlQuery query;
        if (!printer.isEmpty())
            query.addQueryItem(QStringLiteral("printer"), printer);
        d->post(operationId, std::move(handler), QStringLiteral("/lpr/print-raw"), query,
                QStringLiteral("{\"data\": \"%1\"}").arg(label.toBase64().constData()).toLatin1());
    }
    return operationId;
}

qulonglong LprPrinterApi::printFile(const QString& fileName, const QString& printer, unsigned int copies)
{
    Q_D(LprPrinterApi);
    qulonglong operationId = 0;
    if (!call(this, &LprPrinterApi::printFile, Proof::Call::BlockEvents, operationId, fileName, printer, copies)) {
        RestAnswerHandler handler = [this, d](qulonglong operationId, QNetworkReply *reply) {
            LprPrinterStatus status;
            if (d->parsePrinterStatus(operationId, reply, status)) {
                if (status.isReady)
                    emit filePrinted(operationId);
                else
                    emit apiErrorOccurred(operationId, {RestApiError::Level::ServerError, 0,
                                                     NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::ServerError,
                                                     status.reason});
            }
        };
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            qCDebug(proofNetworkLprPrinterLog) << "File error:" << file.error() << file.errorString();
            emit apiErrorOccurred(operationId, {RestApiError::Level::JsonDataError, 0,
                                             NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::FileError,
                                             QStringLiteral("Can't open file")});
            return operationId;
        }
        QByteArray data = file.readAll();
        if (file.error() != QFileDevice::NoError) {
            qCDebug(proofNetworkLprPrinterLog) << "File error:" << file.error() << file.errorString();
            emit apiErrorOccurred(operationId, {RestApiError::Level::JsonDataError, 0,
                                             NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::FileError,
                                             QStringLiteral("Can't read file")});
            return operationId;
        }
        QUrlQuery query;
        query.addQueryItem(QStringLiteral("copies"), QString::number(copies));
        if (!printer.isEmpty())
            query.addQueryItem(QStringLiteral("printer"), printer);
        d->post(operationId, std::move(handler), QStringLiteral("/lpr/print"), query, data);
    }
    return operationId;
}

qulonglong LprPrinterApi::fetchPrintersList()
{
    Q_D(LprPrinterApi);
    qulonglong operationId = 0;
    if (!call(this, &LprPrinterApi::fetchPrintersList, Proof::Call::BlockEvents, operationId)) {
        RestAnswerHandler handler = [this, d](qulonglong operationId, QNetworkReply *reply) {
            QJsonParseError jsonError;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &jsonError);
            if (jsonError.error != QJsonParseError::NoError) {
                emit apiErrorOccurred(operationId, RestApiError{RestApiError::Level::JsonParseError, jsonError.error,
                                                             NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply,
                                                             QStringLiteral("JSON error: %1").arg(jsonError.errorString())});
            } else if (!doc.isArray()) {
                emit apiErrorOccurred(operationId, {RestApiError::Level::JsonDataError, 0,
                                                 NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply,
                                                 QStringLiteral("Array is not found in document")});
            } else {
                QList<LprPrinterInfo> printers;
                QJsonArray array = doc.array();
                for (const QJsonValue &value : array) {
                    QJsonObject object = value.toObject();
                    QString printer = object.value(QStringLiteral("printer")).toString();
                    if (!printer.isEmpty())
                        printers << LprPrinterInfo{printer, object.value("accepts_raw").toBool(), object.value("accepts_files").toBool()};
                }
                emit printersListFetched(operationId, printers);
            }
        };
        d->get(operationId, std::move(handler), QStringLiteral("/lpr/list"));
    }
    return operationId;
}

bool LprPrinterApiPrivate::parsePrinterStatus(qulonglong operationId, QNetworkReply *reply, LprPrinterStatus &result)
{
    Q_Q(LprPrinterApi);
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        emit q->apiErrorOccurred(operationId, RestApiError{RestApiError::Level::JsonParseError, jsonError.error,
                                                        NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply,
                                                        QStringLiteral("JSON error: %1").arg(jsonError.errorString())});
    } else if (!doc.isObject()) {
        emit q->apiErrorOccurred(operationId, {RestApiError::Level::JsonDataError, 0,
                                            NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidReply,
                                            QStringLiteral("Object is not found in document")});
    } else {
        QJsonObject object = doc.object();
        result = LprPrinterStatus{object.value("is_ready").toBool(), object.value("reason").toString()};
        return true;
    }
    return false;
}

} // namespace NetworkServices
} // namespace Proof
