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
#include "proofnetwork/ums/tokensapi.h"

#include "proofnetwork/baserestapi_p.h"
#include "proofnetwork/ums/data/umstokeninfo.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QtCrypto>

namespace Proof {
namespace Ums {

class TokensApiPrivate : public BaseRestApiPrivate
{
    Q_DECLARE_PUBLIC(TokensApi)
    std::function<UmsTokenInfoSP(const RestApiReply &)> tokenUnmarshaller();
    bool verifyToken(const QByteArrayList &tokenList);

    QCA::RSAPublicKey rsaPublicKey;
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

QCA::RSAPublicKey TokensApi::rsaKey() const
{
    Q_D_CONST(TokensApi);
    return d->rsaPublicKey;
}

void TokensApi::setRsaKey(const QCA::RSAPublicKey &key)
{
    Q_D(TokensApi);
    d->rsaPublicKey = key;
}

CancelableFuture<UmsTokenInfoSP> TokensApi::fetchToken()
{
    Q_D(TokensApi);
    QUrlQuery urlQuery;
    urlQuery.addQueryItem(QStringLiteral("grant_type"), QStringLiteral("client_credentials"));
    urlQuery.addQueryItem(QStringLiteral("client_id"), d->clientId);
    urlQuery.addQueryItem(QStringLiteral("client_secret"), d->clientSecret);
    return unmarshalReply(post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
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
    return unmarshalReply(post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
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
    return unmarshalReply(post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
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
    return unmarshalReply(post(QStringLiteral("/oauth2/token"), QUrlQuery(), urlQuery.toString().toLatin1()),
                          d->tokenUnmarshaller());
}

CancelableFuture<QString> TokensApi::fetchPublicKey()
{
    return unmarshalReply(get(QStringLiteral("/Token/GetPublicKey")),
                          [](const RestApiReply &reply) { return QString(reply.data); });
}

CancelableFuture<QString> TokensApi::fetchCertificate()
{
    return unmarshalReply(get(QStringLiteral("/Token/GetCertificate")),
                          [](const RestApiReply &reply) { return QString(reply.data); });
}

std::function<UmsTokenInfoSP(const RestApiReply &)> TokensApiPrivate::tokenUnmarshaller()
{
    return [this](const RestApiReply &reply) -> UmsTokenInfoSP {
        QString token = QJsonDocument::fromJson(reply.data).object().value(QStringLiteral("access_token")).toString();
        QByteArrayList tokenList = token.toUtf8().split('.');

        Proof::Ums::UmsTokenInfoSP tokenInfo;
        bool signatureVerified = verifyToken(tokenList);
        if (signatureVerified && tokenList.count() >= 2) {
            QJsonObject payload = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[1])).object();
            tokenInfo = Proof::Ums::UmsTokenInfo::fromJson(payload, token);
        }

        if (!signatureVerified) {
            return WithFailure(QStringLiteral("Token signature verification failed"), NETWORK_UMS_MODULE_CODE,
                               NetworkErrorCode::InvalidTokenSignature);
        }
        if (token.isEmpty() || !tokenInfo || !tokenInfo->isDirty()) {
            return WithFailure(QStringLiteral("Failed to parse JSON from server reply"), NETWORK_UMS_MODULE_CODE,
                               NetworkErrorCode::InvalidReply);
        }
        return tokenInfo;
    };
}

bool TokensApiPrivate::verifyToken(const QByteArrayList &tokenList)
{
    bool signatureVerified = false;
    if (tokenList.count() == 3 || tokenList.count() == 2) {
        QJsonObject header = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList[0])).object();
        QString algorithm = header.value(QStringLiteral("alg")).toString(QStringLiteral("none")).toLower();

        if (algorithm == QLatin1String("rs256")) {
            QByteArray signature = tokenList.count() == 3
                                       ? QByteArray::fromBase64(tokenList[2], QByteArray::Base64UrlEncoding)
                                       : QByteArray();
            QByteArray signedMessage;
            signedMessage.append(tokenList[0]);
            signedMessage.append(".");
            signedMessage.append(tokenList[1]);
            signatureVerified = rsaPublicKey.verifyMessage(signedMessage, signature, QCA::EMSA3_SHA256);
        } else if (algorithm == QLatin1String("none")) {
            signatureVerified = true;
        } else {
            qCWarning(proofNetworkUmsApiLog)
                << "JWT algorithm" << algorithm << "is not supported. Token verification failed";
        }
    }

    return signatureVerified;
}
