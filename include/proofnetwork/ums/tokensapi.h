#ifndef PROOF_UMS_TOKENSAPI_H
#define PROOF_UMS_TOKENSAPI_H

#include "proofnetwork/abstractrestapi.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

#include <qca_publickey.h>

namespace Proof {
namespace Ums {

class TokensApiPrivate;
class PROOF_NETWORK_UMS_EXPORT TokensApi : public AbstractRestApi
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TokensApi)
public:
    explicit TokensApi(const RestClientSP &restClient, QObject *parent = nullptr);

    QCA::RSAPublicKey rsaKey() const;
    void setRsaKey(const QCA::RSAPublicKey &key);

    qulonglong fetchTokenByBarcode(const QString &barcode);
signals:
    void tokenFetched(qulonglong operationId, const QString &token, Proof::Ums::UmsUserSP user);
};

}
}

#endif // PROOF_UMS_TOKENSAPI_H
