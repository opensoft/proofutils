#include "tokensapi.h"
#include "proofnetwork/abstractrestapi_p.h"

#include "proofnetwork/ums/data/umsuser.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

#ifndef QCA_DISABLED
# include <QtCrypto>
#endif

namespace Proof {
namespace Ums {

class TokensApiPrivate : public AbstractRestApiPrivate
{
    Q_DECLARE_PUBLIC(TokensApi)
    void extractToken(qulonglong operationId, QNetworkReply *reply);
    bool verifyToken(const QByteArrayList &tokenList);

#ifndef QCA_DISABLED
    QCA::RSAPublicKey rsaPublicKey;
#endif
    QString clientId;
    QString clientSecret;
};

}
}

using namespace Proof;
using namespace Proof::Ums;

TokensApi::TokensApi(const QString &clientId, const QString &clientSecret, const RestClientSP &restClient, QObject *parent)
    : AbstractRestApi(restClient, *new TokensApiPrivate, parent)
{
    Q_D(TokensApi);
    d->clientId = clientId;
    d->clientSecret = clientSecret;
}

#ifndef QCA_DISABLED
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

qulonglong TokensApi::fetchToken()
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchToken, Proof::Call::BlockEvents, operationId)) {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("grant_type", "client_credentials");
        urlQuery.addQueryItem("client_id", d->clientId);
        urlQuery.addQueryItem("client_secret", d->clientSecret);

        RestAnswerHandler handler = [d](qulonglong operationId, QNetworkReply *reply) {
            d->extractToken(operationId, reply);
        };
        d->post(operationId, std::move(handler), QString("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1());
    }
    return operationId;
}

qulonglong TokensApi::fetchTokenByBarcode(const QString &barcode)
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchTokenByBarcode, Proof::Call::BlockEvents, operationId, barcode)) {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("grant_type", "client_credentials");
        urlQuery.addQueryItem("client_id", d->clientId);
        urlQuery.addQueryItem("client_secret", d->clientSecret);
        urlQuery.addQueryItem("barcode", barcode);

        RestAnswerHandler handler = [d](qulonglong operationId, QNetworkReply *reply) {
            d->extractToken(operationId, reply);
        };
        d->post(operationId, std::move(handler), QString("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1());
    }
    return operationId;
}

qulonglong TokensApi::fetchTokenByLogin(const QString &login, const QString &password)
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchTokenByLogin, Proof::Call::BlockEvents, operationId, login, password)) {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("grant_type", "password");
        urlQuery.addQueryItem("client_id", d->clientId);
        urlQuery.addQueryItem("client_secret", d->clientSecret);
        urlQuery.addQueryItem("username", login);
        urlQuery.addQueryItem("password", password);

        RestAnswerHandler handler = [d](qulonglong operationId, QNetworkReply *reply) {
            d->extractToken(operationId, reply);
        };
        d->post(operationId, std::move(handler), QString("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1());
    }
    return operationId;
}

qulonglong TokensApi::refreshToken(const QString &oldToken)
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::refreshToken, Proof::Call::BlockEvents, operationId, oldToken)) {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("grant_type", "refresh_token");
        urlQuery.addQueryItem("client_id", d->clientId);
        urlQuery.addQueryItem("client_secret", d->clientSecret);
        urlQuery.addQueryItem("refresh_token", oldToken);

        RestAnswerHandler handler = [d](qulonglong operationId, QNetworkReply *reply) {
            d->extractToken(operationId, reply);
        };
        d->post(operationId, std::move(handler), QString("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1());
    }
    return operationId;
}

qulonglong TokensApi::fetchPublicKey()
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchPublicKey, Proof::Call::BlockEvents, operationId)) {

        RestAnswerHandler handler = [this](qulonglong operationId, QNetworkReply *reply) {
            emit publicKeyFetched(operationId, reply->readAll());
        };
        d->get(operationId, std::move(handler), QString("/Token/GetPublicKey"));
    }
    return operationId;
}

qulonglong TokensApi::fetchCertificate()
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchCertificate, Proof::Call::BlockEvents, operationId)) {

        RestAnswerHandler handler = [this](qulonglong operationId, QNetworkReply *reply) {
            emit certificateFetched(operationId, reply->readAll());
        };
        d->get(operationId, std::move(handler), QString("/Token/GetCertificate"));
    }
    return operationId;
}

void TokensApiPrivate::extractToken(qulonglong operationId, QNetworkReply *reply)
{
    Q_Q(TokensApi);

    QString token = QJsonDocument::fromJson(reply->readAll()).object().value("access_token").toString();
    QByteArrayList tokenList = token.toUtf8().split('.');

    Proof::Ums::UmsUserSP umsUser;
    bool signatureVerified = verifyToken(tokenList);
    if (signatureVerified && tokenList.count() == 3) {
        QJsonObject payload = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[1])).object();
        umsUser = Proof::Ums::UmsUser::fromJson(payload);
    }

    if (!signatureVerified) {
        emit q->errorOccurred(operationId, {RestApiError::Level::ClientError, 0,
                                            NETWORK_UMS_MODULE_CODE, NetworkErrorCode::InvalidTokenSignature,
                                            "Token signature verification failed"});
    } else if (token.isEmpty() || !umsUser || !umsUser->isDirty()) {
        emit q->errorOccurred(operationId, {RestApiError::Level::JsonDataError, 0,
                                            NETWORK_UMS_MODULE_CODE, NetworkErrorCode::InvalidReply,
                                            "Failed to parse JSON from server reply"});
    } else {
        emit q->tokenFetched(operationId, token, umsUser);
    }
}

bool TokensApiPrivate::verifyToken(const QByteArrayList &tokenList)
{
    bool signatureVerified = false;
    if (tokenList.count() == 3) {
        QJsonObject header = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[0])).object();
        QString algorithm = header.value("alg").toString("none").toLower();

        QByteArray signature = QByteArray::fromBase64(tokenList[2], QByteArray::Base64UrlEncoding);
        QByteArray signedMessage;
        signedMessage.append(tokenList[0]);
        signedMessage.append(".");
        signedMessage.append(tokenList[1]);
        //TODO: add HS256 support if ever will be needed
        if (algorithm == "rs256") {
#ifndef QCA_DISABLED
            signatureVerified = rsaPublicKey.verifyMessage(signedMessage, signature, QCA::EMSA3_SHA256);
#else
            Q_UNUSED(signature)
            qCDebug(proofNetworkUmsApiLog) << "rs256 algorithm" << algorithm << "is not supported. Token verification force successed!";
            signatureVerified = true;
#endif
        } else if (algorithm == "none") {
            signatureVerified = true;
        } else {
            qCDebug(proofNetworkUmsApiLog) << "JWT algorithm" << algorithm << "is not supported. Token verification failed";
        }
    }

    return signatureVerified;
}

