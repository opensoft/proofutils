#include "proofcore/proofglobal.h"

#include "proofnetwork/ums/data/umstokeninfo.h"
#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

#include <QtQml/QQmlListProperty>

Q_LOGGING_CATEGORY(proofNetworkUmsApiLog, "proof.network.ums.api")
Q_LOGGING_CATEGORY(proofNetworkUmsDataLog, "proof.network.ums.data")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    // clang-format off
    qRegisterMetaType<Proof::Ums::UmsUserSP>("Proof::Ums::UmsUserSP");
    qRegisterMetaType<Proof::Ums::UmsUserWP>("Proof::Ums::UmsUserWP");
    qRegisterMetaType<Proof::Ums::UmsTokenInfoSP>("Proof::Ums::UmsTokenInfoSP");
    qRegisterMetaType<Proof::Ums::UmsTokenInfoWP>("Proof::Ums::UmsTokenInfoWP");

    qRegisterMetaType<Proof::Ums::UmsUserQmlWrapper *>("Proof::Ums::UmsUserQmlWrapper *");
    qRegisterMetaType<Proof::Ums::UmsTokenInfoQmlWrapper *>("Proof::Ums::UmsTokenInfoQmlWrapper *");
    // clang-format on
}
