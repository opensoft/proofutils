#ifndef PROOFNETWORKUMS_TYPES_H
#define PROOFNETWORKUMS_TYPES_H

#include "proofnetwork/proofnetwork_types.h"

#include <QSharedPointer>
#include <QWeakPointer>

namespace Proof {
namespace Ums {

class Role;
using RoleSP = QSharedPointer<Role>;
using RoleWP = QWeakPointer<Role>;

class UmsUser;
using UmsUserSP = QSharedPointer<UmsUser>;
using UmsUserWP = QWeakPointer<UmsUser>;

}
}

#endif // PROOFNETWORKUMS_TYPES_H
