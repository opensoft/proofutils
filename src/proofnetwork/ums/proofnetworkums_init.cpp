#include "data/umsuser.h"

#include "data/umsuser.h"
#include "data/role.h"
#include "proofnetworkums_global.h"
#include "proofcore/proofglobal.h"

#include <QtQml/QQmlListProperty>

Q_LOGGING_CATEGORY(proofNetworkUmsApiLog, "proof.network.ums.api")
Q_LOGGING_CATEGORY(proofNetworkUmsDataLog, "proof.network.ums.data")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    //clang-format off
    qRegisterMetaType<Proof::Ums::UmsUserSP>("Proof::Ums::UmsUserSP");
    qRegisterMetaType<Proof::Ums::UmsUserWP>("Proof::Ums::UmsUserWP");

    qRegisterMetaType<Proof::Ums::RoleSP>("Proof::Ums::RoleSP");
    qRegisterMetaType<Proof::Ums::RoleWP>("Proof::Ums::RoleWP");

    qRegisterMetaType<Proof::Ums::UmsUserQmlWrapper *>("Proof::Ums::UmsUserQmlWrapper *");

    qRegisterMetaType<Proof::Ums::RoleQmlWrapper *>("Proof::Ums::RoleQmlWrapper *");
    qRegisterMetaType<QQmlListProperty<Proof::Ums::RoleQmlWrapper>>("QQmlListProperty<Proof::Ums::RoleQmlWrapper>");
    //clang-format on
}
