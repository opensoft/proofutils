#include "umsuserqmlwrapper.h"

#include "proofnetwork/qmlwrappers/userqmlwrapper_p.h"
#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/ums/data/role.h"

namespace Proof {
namespace Ums {
class UmsUserQmlWrapperPrivate : public UserQmlWrapperPrivate
{
    Q_DECLARE_PUBLIC(UmsUserQmlWrapper)

    static RoleQmlWrapper *roleAt(QQmlListProperty<RoleQmlWrapper> *property, int index);
    static int rolesCount(QQmlListProperty<RoleQmlWrapper> *property);

    void updateRoles();

    QList<RoleQmlWrapper *> roles;
    QQmlListProperty<Proof::Ums::RoleQmlWrapper> qmlRolesList;
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

PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsUser, QString, id)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsUser, QString, version)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsUser, QDateTime, expiresAt)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsUser, QDateTime, validFrom)

QQmlListProperty<Proof::Ums::RoleQmlWrapper> UmsUserQmlWrapper::roles() const
{
    Q_D(const UmsUserQmlWrapper);
    return d->qmlRolesList;
}

RoleQmlWrapper *UmsUserQmlWrapperPrivate::roleAt(QQmlListProperty<RoleQmlWrapper> *property, int index)
{
    return static_cast<QList<RoleQmlWrapper *> *>(property->data)->at(index);
}

int UmsUserQmlWrapperPrivate::rolesCount(QQmlListProperty<RoleQmlWrapper> *property)
{
    return static_cast<QList<RoleQmlWrapper *> *>(property->data)->count();
}

void UmsUserQmlWrapper::setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old)
{
    UserQmlWrapper::setupEntity();

    Q_D(UmsUserQmlWrapper);
    UmsUserSP user = d->entity<UmsUser>();
    Q_ASSERT(user);

    connect(user.data(), &UmsUser::idChanged,
            this, &UmsUserQmlWrapper::idChanged);
    connect(user.data(), &UmsUser::versionChanged,
            this, &UmsUserQmlWrapper::versionChanged);
    connect(user.data(), &UmsUser::expiresAtChanged,
            this, &UmsUserQmlWrapper::expiresAtChanged);
    connect(user.data(), &UmsUser::validFromChanged,
            this, &UmsUserQmlWrapper::validFromChanged);

    connect(user.data(), &UmsUser::rolesChanged,
            d->lambdaConnectContext, [d](){d->updateRoles();});

    auto castedOld = qSharedPointerCast<UmsUser>(old);
    if (castedOld) {
        if (user->id() != castedOld->id())
            emit idChanged(user->id());
        if (user->version() != castedOld->version())
            emit versionChanged(user->version());
        if (user->expiresAt() != castedOld->expiresAt())
            emit expiresAtChanged(user->expiresAt());
        if (user->validFrom() != castedOld->validFrom())
            emit validFromChanged(user->validFrom());
    }

    d->updateRoles();
}

void UmsUserQmlWrapperPrivate::updateRoles()
{
    Q_Q(UmsUserQmlWrapper);
    UmsUserSP user = entity<UmsUser>();
    for (RoleQmlWrapper *wrapper : qAsConst(roles))
        wrapper->deleteLater();

    roles.clear();
    const auto ndeRoles = user->roles();
    for (const RoleSP &role : ndeRoles)
        roles << role->toQmlWrapper(q);

    qmlRolesList = QQmlListProperty<Proof::Ums::RoleQmlWrapper>(q, &roles,
                                                                &UmsUserQmlWrapperPrivate::rolesCount,
                                                                &UmsUserQmlWrapperPrivate::roleAt);

    emit q->rolesChanged(qmlRolesList);
}
