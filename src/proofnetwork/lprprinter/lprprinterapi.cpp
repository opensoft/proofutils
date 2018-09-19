/* Copyright 2018, OpenSoft Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *     * Neither the name of OpenSoft Inc. nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "proofnetwork/lprprinter/lprprinterapi.h"

#include "proofnetwork/lprprinter/errormessages_p.h"
#include "proofnetwork/lprprinter/proofnetworklprprinter_types.h"
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
            if (!object.value(QStringLiteral("is_ready")).toBool()) {
                return WithFailure(object.value(QStringLiteral("reason")).toString(), NETWORK_LPR_PRINTER_MODULE_CODE,
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
    return unmarshalReply(get(QStringLiteral("/lpr/status"), query), d->printerStatusUnmarshaller());
}

CancelableFuture<bool> LprPrinterApi::printLabel(const QByteArray &label, const QString &printer)
{
    Q_D(LprPrinterApi);
    QUrlQuery query;
    if (!printer.isEmpty())
        query.addQueryItem(QStringLiteral("printer"), printer);
    return unmarshalReply(post(QStringLiteral("/lpr/print-raw"), query,
                               QStringLiteral("{\"data\": \"%1\"}").arg(label.toBase64().constData()).toLatin1()),
                          d->discardingPrinterStatusUnmarshaller());
}

CancelableFuture<bool> LprPrinterApi::printFile(const QString &fileName, const QString &printer, unsigned int copies)
{
    Q_D(LprPrinterApi);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(proofNetworkLprPrinterLog) << "Lpr-Printer: file error:" << file.error() << file.errorString();
        return invalidArgumentsFailure<bool>(
            Failure(QStringLiteral("Can't open file"), NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::FileError));
    }
    QByteArray data = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        qCWarning(proofNetworkLprPrinterLog) << "Lpr-Printer: file error:" << file.error() << file.errorString();
        return invalidArgumentsFailure<bool>(
            Failure(QStringLiteral("Can't read file"), NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::FileError));
    }
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("copies"), QString::number(copies));
    if (!printer.isEmpty())
        query.addQueryItem(QStringLiteral("printer"), printer);
    return unmarshalReply(post(QStringLiteral("/lpr/print"), query, data), d->discardingPrinterStatusUnmarshaller());
}

CancelableFuture<QVector<LprPrinterInfo>> LprPrinterApi::fetchPrintersList()
{
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
    return unmarshalReply(get(QStringLiteral("/lpr/list")), unmarshaller);
}
