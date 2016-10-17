#ifndef PROOF_MOBILEUTILS_H
#define PROOF_MOBILEUTILS_H

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

namespace Proof {

class MobileUtilsPrivate;
class PROOF_UTILS_EXPORT MobileUtils : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MobileUtils)
public:
    explicit MobileUtils(QObject *parent = 0);
    ~MobileUtils();

    Q_INVOKABLE void callPhoneNumber(const QString &phoneNumber);

signals:
    void errorOccurred(long moduleCode, long errorCode, const QString &errorMessage, bool userFriendly);
};

} // namespace Proof

#endif // PROOF_MOBILEUTILS_H