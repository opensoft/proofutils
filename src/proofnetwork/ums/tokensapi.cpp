#include "tokensapi.h"
#include "proofnetwork/abstractrestapi_p.h"

#include "proofnetwork/ums/data/umsuser.h"

#include <QNetworkReply>
#include <QFile>

namespace Proof {
namespace Ums {

class TokensApiPrivate : public AbstractRestApiPrivate
{
    Q_DECLARE_PUBLIC(TokensApi)
};

}
}

using namespace Proof;
using namespace Proof::Ums;

TokensApi::TokensApi(const RestClientSP &restClient, QObject *parent)
    : AbstractRestApi(restClient, *new TokensApiPrivate, parent)
{
}

qulonglong TokensApi::fetchTokenByBarcode(const QString &barcode)
{
    Q_D(TokensApi);
    qulonglong operationId = 0;
    if (!call(this, &TokensApi::fetchTokenByBarcode, Proof::Call::BlockEvents, operationId, barcode)) {
        QUrlQuery urlQuery;
        urlQuery.addQueryItem("barcode", barcode);

        RestAnswerHandler handler = [this, d](qulonglong operationId, QNetworkReply *reply) {
            QByteArray token = reply->readAll();
            QByteArrayList tokenList = token.split('.');

            Proof::Ums::UmsUserSP umsUser;
            QByteArray signature;
            if (tokenList.count() == 3) {
                QJsonObject header = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList.first())).object();
                QString algorithm = header.value("alg").toString();
                QString tokenType = header.value("typ").toString();
                Q_UNUSED(algorithm);
                Q_UNUSED(tokenType);

                QJsonObject payload = QJsonDocument::fromJson(QByteArray::fromBase64(tokenList.at(1))).object();
                umsUser = Proof::Ums::UmsUser::fromJson(payload);

                //TODO: verify signature is needed
                signature = tokenList.last();
            }

            if (token.isEmpty() || !umsUser || !umsUser->isDirty()) {
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

