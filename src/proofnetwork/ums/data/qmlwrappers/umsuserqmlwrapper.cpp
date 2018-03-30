#include "umsuserqmlwrapper.h"

#include "proofnetwork/qmlwrappers/userqmlwrapper_p.h"
#include "proofnetwork/ums/data/umsuser.h"

namespace Proof {
namespace Ums {
class UmsUserQmlWrapperPrivate : public UserQmlWrapperPrivate
{
    Q_DECLARE_PUBLIC(UmsUserQmlWrapper)
};

}
}

using namespace Proof;
using namespace Proof::Ums;

UmsUserQmlWrapper::UmsUserQmlWrapper(const UmsUserSP &user, QObject *parent)
    : UserQmlWrapper(user, *new UmsUserQmlWrapperPrivate, parent)
{
    setupEntity();
}

PROOF_NDE_WRAPPER_TOOLS_IMPL(UmsUser)

PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsUser, QStringList, roles)

void UmsUserQmlWrapper::setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old)
{
    UserQmlWrapper::setupEntity();

    Q_D(UmsUserQmlWrapper);
    UmsUserSP user = d->entity<UmsUser>();
    Q_ASSERT(user);

    connect(user.data(), &UmsUser::rolesChanged,
            this, &UmsUserQmlWrapper::rolesChanged);

    auto castedOld = qSharedPointerCast<UmsUser>(old);
    if (castedOld) {
        if (user->roles() != castedOld->roles())
            emit rolesChanged(user->roles());
    }
}
