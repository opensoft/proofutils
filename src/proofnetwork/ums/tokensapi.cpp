#include "tokensapi.h"
#include "proofnetwork/abstractrestapi_p.h"

#include "proofnetwork/ums/data/umsuser.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#ifndef Q_OS_ANDROID
# include <QtCrypto>
#endif

namespace Proof {
namespace Ums {

class TokensApiPrivate : public AbstractRestApiPrivate
{
    Q_DECLARE_PUBLIC(TokensApi)
#ifndef Q_OS_ANDROID
    QCA::RSAPublicKey rsaPublicKey;
#endif
};

}
}

using namespace Proof;
using namespace Proof::Ums;

TokensApi::TokensApi(const RestClientSP &restClient, QObject *parent)
    : AbstractRestApi(restClient, *new TokensApiPrivate, parent)
{
}

#ifndef Q_OS_ANDROID
QCA::RSAPublicKey TokensApi::rsaKey() const
{
    Q_D(const TokensApi);
    return d->rsaPublicKey;
}

void TokensApi::setRsaKey(const QCA::RSAPublicKey &key)
{
    Q_D(TokensApi);
    d->rsaPublicKey = key;
}
#endif

qulonglong TokensApi::fetchTokenByBarcode(const QString &barcode)
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchTokenByBarcode, Proof::Call::BlockEvents, operationId, barcode)) {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("barcode", barcode);

        RestAnswerHandler handler = [this, d](qulonglong operationId, QNetworkReply *reply) {
            QByteArray token = QJsonDocument::fromJson(reply->readAll()).object().value("access_token").toString().toUtf8();
            QByteArrayList tokenList = token.split('.');

            Proof::Ums::UmsUserSP umsUser;
            bool signatureVerified = false;
            if (tokenList.count() == 3) {
                QJsonObject header = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[0])).object();
                QString algorithm = header.value("alg").toString("none").toLower();

                QJsonObject payload = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[1])).object();
                umsUser = Proof::Ums::UmsUser::fromJson(payload);

                QByteArray signature = QByteArray::fromBase64(tokenList[2], QByteArray::Base64UrlEncoding);
                QByteArray signedMessage;
                signedMessage.append(tokenList[0]);
                signedMessage.append(".");
                signedMessage.append(tokenList[1]);
                //TODO: add HS256 support if ever will be needed
                if (algorithm == "rs256") {
#ifdef Q_OS_ANDROID
                    //TODO: add qca sippor
                    signatureVerified = true;
#else
                    signatureVerified = d->rsaPublicKey.verifyMessage(signedMessage, signature, QCA::EMSA3_SHA256);
#endif
                } else if (algorithm == "none") {
                    signatureVerified = true;
                } else {
                    qCDebug(proofNetworkUmsApiLog) << "JWT algorithm" << algorithm << "is not supported. Token verification failed";
                }
            }

            if (!signatureVerified) {
                emit errorOccurred(operationId, {RestApiError::Level::ClientError, 0,
                                                 NETWORK_UMS_MODULE_CODE, NetworkErrorCode::InvalidTokenSignature,
                                                 "Token signature verification failed"});
            } else if (token.isEmpty() || !umsUser || !umsUser->isDirty()) {
                emit errorOccurred(operationId, {RestApiError::Level::JsonDataError, 0,
                                                 NETWORK_UMS_MODULE_CODE, NetworkErrorCode::InvalidReply,
                                                 "Failed to parse JSON from server reply"});
            } else {
                emit tokenFetched(operationId, token, umsUser);
            }
        };
        d->post(operationId, std::move(handler), QString("/oauth2/token"), urlQuery);
    }
    return operationId;
}

