#ifndef PROOF_UMS_TOKENSAPI_H
#define PROOF_UMS_TOKENSAPI_H

#include "proofnetwork/baserestapi.h"
#include "proofnetwork/ums/proofnetworkums_global.h"
#include "proofnetwork/ums/proofnetworkums_types.h"

#ifndef QCA_DISABLED
#    include <qca_publickey.h>
#endif

namespace Proof {
namespace Ums {

class TokensApiPrivate;
class PROOF_NETWORK_UMS_EXPORT TokensApi : public BaseRestApi
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TokensApi)
public:
    explicit TokensApi(const QString &clientId, const QString &clientSecret, const RestClientSP &restClient,
                       QObject *parent = nullptr);

#ifndef QCA_DISABLED
    QCA::RSAPublicKey rsaKey() const;
    void setRsaKey(const QCA::RSAPublicKey &key);
#endif

    CancelableFuture<UmsTokenInfoSP> fetchToken();
    CancelableFuture<UmsTokenInfoSP> fetchTokenByBarcode(const QString &barcode);
    CancelableFuture<UmsTokenInfoSP> fetchTokenByLogin(const QString &login, const QString &password);
    CancelableFuture<UmsTokenInfoSP> refreshToken(const QString &oldToken);

    CancelableFuture<QString> fetchPublicKey();
    CancelableFuture<QString> fetchCertificate();
    //TODO: remove signals after full switch to new network
signals:
    void tokenFetched(qulonglong operationId, const Proof::Ums::UmsTokenInfoSP &tokenInfo);
    void publicKeyFetched(qulonglong operationId, const QString &publicKey);
    void certificateFetched(qulonglong operationId, const QString &certificate);
};

} // namespace Ums
} // namespace Proof

#endif // PROOF_UMS_TOKENSAPI_H
