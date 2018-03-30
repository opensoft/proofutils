#ifndef PROOFNETWORKUMS_TYPES_H
#define PROOFNETWORKUMS_TYPES_H

#include "proofnetwork/proofnetwork_types.h"

#include <QSharedPointer>
#include <QWeakPointer>

namespace Proof {
namespace Ums {

class UmsUser;
using UmsUserSP = QSharedPointer<UmsUser>;
using UmsUserWP = QWeakPointer<UmsUser>;

class UmsTokenInfo;
using UmsTokenInfoSP = QSharedPointer<UmsTokenInfo>;
using UmsTokenInfoWP = QWeakPointer<UmsTokenInfo>;

}
}

#endif // PROOFNETWORKUMS_TYPES_H
