#include "umsuser.h"

#include "umsuser_p.h"

namespace Proof {
namespace Ums {
ObjectsCache<QString, UmsUser> &umsUsersCache()
{
    return WeakObjectsCache<QString, UmsUser>::instance();
}

} // namespace Ums
} // namespace Proof

using namespace Proof;
using namespace Proof::Ums;

UmsUser::UmsUser(const QString &userName) : User(*new UmsUserPrivate(userName))
{}

UmsUser::~UmsUser()
{}

QStringList UmsUser::roles() const
{
    Q_D(const UmsUser);
    return d->roles;
}

UmsUserQmlWrapper *UmsUser::toQmlWrapper(QObject *parent) const
{
    Q_D(const UmsUser);
    UmsUserSP castedSelf = qSharedPointerCast<UmsUser>(d->weakSelf);
    Q_ASSERT(castedSelf);
    return new UmsUserQmlWrapper(castedSelf, parent);
}

UmsUserSP UmsUser::create(const QString &userName)
{
    UmsUserSP result(new UmsUser(userName));
    initSelfWeakPtr(result);
    return result;
}

void UmsUserPrivate::updateFrom(const NetworkDataEntitySP &other)
{
    UmsUserSP castedOther = qSharedPointerCast<UmsUser>(other);
    setRoles(castedOther->roles());

    UserPrivate::updateFrom(other);
}

void UmsUserPrivate::setRoles(QStringList newList)
{
    Q_Q(UmsUser);
    std::sort(newList.begin(), newList.end(),
              [](const auto &l, const auto &r) { return QString::compare(l, r, Qt::CaseInsensitive) < 0; });
    if (roles == newList)
        return;
    roles = newList;
    emit q->rolesChanged(roles);
}
