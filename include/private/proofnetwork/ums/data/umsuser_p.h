#ifndef PROOF_UMS_UMSUSER_P_H
#define PROOF_UMS_UMSUSER_P_H

#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/user_p.h"

namespace Proof {
namespace Ums {
class UmsUserPrivate : public UserPrivate
{
    Q_DECLARE_PUBLIC(UmsUser)

    friend class UmsTokenInfo;
    explicit UmsUserPrivate(const QString &userName) : UserPrivate(userName) {}

    void setRoles(QStringList newList);

    QStringList roles;
};
} // namespace Ums
} // namespace Proof

#endif // PROOF_UMS_UMSUSER_P_H
