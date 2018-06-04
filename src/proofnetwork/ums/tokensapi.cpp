#include "tokensapi.h"

#include "proofnetwork/baserestapi_p.h"
#include "proofnetwork/ums/data/umstokeninfo.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

#ifndef QCA_DISABLED
#    include <QtCrypto>
#endif

namespace Proof {
namespace Ums {

class TokensApiPrivate : public BaseRestApiPrivate
{
    Q_DECLARE_PUBLIC(TokensApi)
    std::function<UmsTokenInfoSP(const RestApiReply &)> tokenUnmarshaller();
    bool verifyToken(const QByteArrayList &tokenList);

#ifndef QCA_DISABLED
    QCA::RSAPublicKey rsaPublicKey;
#endif
    QString clientId;
    QString clientSecret;
};

} // namespace Ums
} // namespace Proof

using namespace Proof;
using namespace Proof::Ums;

TokensApi::TokensApi(const QString &clientId, const QString &clientSecret, const RestClientSP &restClient, QObject *parent)
    : BaseRestApi(restClient, *new TokensApiPrivate, parent)
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

CancelableFuture<UmsTokenInfoSP> TokensApi::fetchToken()
{
    Q_D(TokensApi);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("client_credentials"));
    urlQuery.addQueryItem(QStringLiteral("client_id"), d->clientId);
    urlQuery.addQueryItem(QStringLiteral("client_secret"), d->clientSecret);
    return d->unmarshalReply(d->post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
                             d->tokenUnmarshaller());
}

CancelableFuture<UmsTokenInfoSP> TokensApi::fetchTokenByBarcode(const QString &barcode)
{
    Q_D(TokensApi);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("client_credentials"));
    urlQuery.addQueryItem(QStringLiteral("client_id"), d->clientId);
    urlQuery.addQueryItem(QStringLiteral("client_secret"), d->clientSecret);
    urlQuery.addQueryItem(QStringLiteral("barcode"), barcode);
    return d->unmarshalReply(d->post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
                             d->tokenUnmarshaller());
}

CancelableFuture<UmsTokenInfoSP> TokensApi::fetchTokenByLogin(const QString &login, const QString &password)
{
    Q_D(TokensApi);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("password"));
    urlQuery.addQueryItem(QStringLiteral("client_id"), d->clientId);
    urlQuery.addQueryItem(QStringLiteral("client_secret"), d->clientSecret);
    urlQuery.addQueryItem(QStringLiteral("username"), login);
    urlQuery.addQueryItem(QStringLiteral("password"), password);
    return d->unmarshalReply(d->post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
                             d->tokenUnmarshaller());
}

CancelableFuture<UmsTokenInfoSP> TokensApi::refreshToken(const QString &oldToken)
{
    Q_D(TokensApi);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("refresh_token"));
    urlQuery.addQueryItem(QStringLiteral("client_id"), d->clientId);
    urlQuery.addQueryItem(QStringLiteral("client_secret"), d->clientSecret);
    urlQuery.addQueryItem(QStringLiteral("refresh_token"), oldToken);
    return d->unmarshalReply(d->post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
                             d->tokenUnmarshaller());
}

CancelableFuture<QString> TokensApi::fetchPublicKey()
{
    Q_D(TokensApi);
    return d->unmarshalReply(d->get(QStringLiteral("/Token/GetPublicKey")),
                             [](const RestApiReply &reply) { return QString(reply.data); });
}

CancelableFuture<QString> TokensApi::fetchCertificate()
{
    Q_D(TokensApi);
    return d->unmarshalReply(d->get(QStringLiteral("/Token/GetCertificate")),
                             [](const RestApiReply &reply) { return QString(reply.data); });
}

std::function<UmsTokenInfoSP(const RestApiReply &)> TokensApiPrivate::tokenUnmarshaller()
{
    return [this](const RestApiReply &reply) -> UmsTokenInfoSP {
        QString token = QJsonDocument::fromJson(reply.data).object().value(QStringLiteral("access_token")).toString();
        QByteArrayList tokenList = token.toUtf8().split('.');

        Proof::Ums::UmsTokenInfoSP tokenInfo;
        bool signatureVerified = verifyToken(tokenList);
        if (signatureVerified && tokenList.count() == 3) {
            QJsonObject payload = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[1])).object();
            tokenInfo = Proof::Ums::UmsTokenInfo::fromJson(payload, token);
        }

        if (!signatureVerified) {
            return WithFailure(QStringLiteral("Token signature verification failed"), NETWORK_UMS_MODULE_CODE,
                               NetworkErrorCode::InvalidTokenSignature);
        } else if (token.isEmpty() || !tokenInfo || !tokenInfo->isDirty()) {
            return WithFailure(QStringLiteral("Failed to parse JSON from server reply"), NETWORK_UMS_MODULE_CODE,
                               NetworkErrorCode::InvalidReply);
        }
        return tokenInfo;
    };
}

bool TokensApiPrivate::verifyToken(const QByteArrayList &tokenList)
{
    bool signatureVerified = false;
    if (tokenList.count() == 3) {
        QJsonObject header = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[0])).object();
        QString algorithm = header.value(QStringLiteral("alg")).toString(QStringLiteral("none")).toLower();

        QByteArray signature = QByteArray::fromBase64(tokenList[2], QByteArray::Base64UrlEncoding);
        QByteArray signedMessage;
        signedMessage.append(tokenList[0]);
        signedMessage.append(".");
        signedMessage.append(tokenList[1]);
        //TODO: add HS256 support if ever will be needed
        if (algorithm == QLatin1String("rs256")) {
#ifndef QCA_DISABLED
            signatureVerified = rsaPublicKey.verifyMessage(signedMessage, signature, QCA::EMSA3_SHA256);
#else
            Q_UNUSED(signature)
            qCWarning(proofNetworkUmsApiLog)
                << "rs256 algorithm" << algorithm << "is not supported. Token verification forcily succeeded!";
            signatureVerified = true;
#endif
        } else if (algorithm == QLatin1String("none")) {
            signatureVerified = true;
        } else {
            qCWarning(proofNetworkUmsApiLog)
                << "JWT algorithm" << algorithm << "is not supported. Token verification failed";
        }
    }

    return signatureVerified;
}
