#ifndef PROOF_UMS_TOKENSAPI_H
#define PROOF_UMS_TOKENSAPI_H

#include "proofnetwork/abstractrestapi.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

#ifndef QCA_DISABLED
# include <qca_publickey.h>
#endif

namespace Proof {
namespace Ums {

class TokensApiPrivate;
class PROOF_NETWORK_UMS_EXPORT TokensApi : public AbstractRestApi
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TokensApi)
public:
    explicit TokensApi(const QString &clientId, const QString &clientSecret, const RestClientSP &restClient, QObject *parent = nullptr);

#ifndef QCA_DISABLED
    QCA::RSAPublicKey rsaKey() const;
    void setRsaKey(const QCA::RSAPublicKey &key);
#endif

    qulonglong fetchToken();
    qulonglong fetchTokenByBarcode(const QString &barcode);
    qulonglong fetchTokenByLogin(const QString &login, const QString &password);
    qulonglong refreshToken(const QString &oldToken);

    qulonglong fetchPublicKey();
    qulonglong fetchCertificate();
signals:
    void tokenFetched(qulonglong operationId, const QString &token, const Proof::Ums::UmsUserSP &user);
    void publicKeyFetched(qulonglong operationId, const QString &publicKey);
    void certificateFetched(qulonglong operationId, const QString &certificate);
};

}
}

#endif // PROOF_UMS_TOKENSAPI_H
