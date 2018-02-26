#include "roleqmlwrapper.h"

#include "proofnetwork/qmlwrappers/networkdataentityqmlwrapper_p.h"
#include "proofnetwork/ums/data/role.h"

namespace Proof {
namespace Ums {
class RoleQmlWrapperPrivate : public NetworkDataEntityQmlWrapperPrivate
{
    Q_DECLARE_PUBLIC(RoleQmlWrapper)
};
}
}

using namespace Proof;
using namespace Proof::Ums;

RoleQmlWrapper::RoleQmlWrapper(const RoleSP &role, QObject *parent)
    : NetworkDataEntityQmlWrapper(role, *new RoleQmlWrapperPrivate, parent)
{
    setupEntity();
}

PROOF_NDE_WRAPPER_TOOLS_IMPL(Role)

PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Role, QString, location)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Role, QString, service)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Role, QString, name)

void RoleQmlWrapper::setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old)
{
    Q_D(RoleQmlWrapper);
    RoleSP role = d->entity<Role>();
    Q_ASSERT(role);

    connect(role.data(), &Role::locationChanged,
            this, &RoleQmlWrapper::locationChanged);
    connect(role.data(), &Role::serviceChanged,
            this, &RoleQmlWrapper::serviceChanged);
    connect(role.data(), &Role::nameChanged,
            this, &RoleQmlWrapper::nameChanged);

    auto castedOld = qSharedPointerCast<Role>(old);
    if (castedOld) {
        if (role->location() != castedOld->location())
            emit locationChanged(role->location());
        if (role->service() != castedOld->service())
            emit serviceChanged(role->service());
        if (role->name() != castedOld->name())
            emit nameChanged(role->name());
    }
}
