#ifndef PROOF_NETWORKINFO_H
#define PROOF_NETWORKINFO_H

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

namespace Proof {

class NetworkInfoPrivate;
class PROOF_UTILS_EXPORT NetworkInfo : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NetworkInfo)
public:
    explicit NetworkInfo(QObject *parent = nullptr);

    Q_INVOKABLE QVariantMap addresses() const;
};

} // namespace Proof

#endif // PROOF_NETWORKINFO_H
