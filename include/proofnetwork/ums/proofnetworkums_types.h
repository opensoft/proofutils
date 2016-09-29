#ifndef PROOFNETWORKUMS_TYPES_H
#define PROOFNETWORKUMS_TYPES_H

#include "proofnetwork/proofnetwork_types.h"

#include <QSharedPointer>
#include <QWeakPointer>

namespace Proof {
namespace Ums {

class Role;
typedef QSharedPointer<Role> RoleSP;
typedef QWeakPointer<Role> RoleWP;

class UmsUser;
typedef QSharedPointer<UmsUser> UmsUserSP;
typedef QWeakPointer<UmsUser> UmsUserWP;

}
}

#endif // PROOFNETWORKUMS_TYPES_H
