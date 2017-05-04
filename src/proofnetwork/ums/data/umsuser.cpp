#include "umsuser.h"

#include "proofnetwork/ums/data/role.h"
#include "proofnetwork/user_p.h"

#include <QJsonArray>

namespace Proof {
namespace Ums {
class UmsUserPrivate : public UserPrivate
{
    Q_DECLARE_PUBLIC(UmsUser)

    explicit UmsUserPrivate(const QString &userName) : UserPrivate(userName) {}

    void updateFrom(const Proof::NetworkDataEntitySP &other) override;

    void setId(const QString &arg);
    void setVersion(const QString &arg);
    void setExpiresAt(const QDateTime &arg);
    void setValidFrom(const QDateTime &arg);
    void setRoles(const QList<RoleSP> &newList);

    QString id;
    QString version;
    QDateTime expiresAt;
    QDateTime validFrom;
    QList<RoleSP> roles;
};

}
}

using namespace Proof;
using namespace Proof::Ums;

UmsUser::UmsUser(const QString &userName)
    : User(*new UmsUserPrivate(userName))
{
}

UmsUser::~UmsUser()
{
}

QString UmsUser::id() const
{
    Q_D(const UmsUser);
    return d->id;
}

QString UmsUser::version() const
{
    Q_D(const UmsUser);
    return d->version;
}

QDateTime UmsUser::expiresAt() const
{
    Q_D(const UmsUser);
    return d->expiresAt;
}

QDateTime UmsUser::validFrom() const
{
    Q_D(const UmsUser);
    return d->validFrom;
}

QList<RoleSP> UmsUser::roles() const
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

UmsUserSP UmsUser::fromJson(const QJsonObject &userJson)
{
    if (!userJson.contains(QLatin1String("sub")))
        return UmsUserSP();

    QString fullName = userJson.value(QStringLiteral("name")).toString(QLatin1String(""));
    UmsUserSP umsUser = create(fullName);
    UmsUserPrivate * const d = umsUser->d_func();
    umsUser->setFetched(true);
    d->setFullName(fullName);
    d->setEmail(userJson.value(QStringLiteral("email")).toString(QLatin1String("")));

    d->setId(userJson.value(QStringLiteral("sub")).toString(QLatin1String("")));
    d->setVersion(userJson.value(QStringLiteral("ver")).toString(QLatin1String("")));
    d->setExpiresAt(QDateTime::fromTime_t(userJson.value(QStringLiteral("exp")).toInt()));
    d->setValidFrom(QDateTime::fromTime_t(userJson.value(QStringLiteral("nbf")).toInt()));

    QList<RoleSP> roles;
    QJsonArray rolesArray = userJson.value(QStringLiteral("roles")).toArray();
    for (const auto &roleValue : rolesArray) {
        RoleSP role = Role::fromJson(roleValue.toObject());
        if (!role) {
            qCCritical(proofNetworkUmsDataLog) << "Role not created.";
            return UmsUserSP();
        }
        roles.append(role);
    }
    d->setRoles(roles);

    return umsUser;
}

void UmsUserPrivate::updateFrom(const NetworkDataEntitySP &other)
{
    UmsUserSP castedOther = qSharedPointerCast<UmsUser>(other);
    setId(castedOther->id());
    setVersion(castedOther->version());
    setExpiresAt(castedOther->expiresAt());
    setValidFrom(castedOther->validFrom());
    setRoles(castedOther->roles());

    UserPrivate::updateFrom(other);
}

void UmsUserPrivate::setId(const QString &arg)
{
    Q_Q(UmsUser);
    if (id != arg) {
        id = arg;
        emit q->idChanged(arg);
    }
}

void UmsUserPrivate::setVersion(const QString &arg)
{
    Q_Q(UmsUser);
    if (version != arg) {
        version = arg;
        emit q->versionChanged(arg);
    }
}

void UmsUserPrivate::setValidFrom(const QDateTime &arg)
{
    Q_Q(UmsUser);
    if (validFrom != arg) {
        validFrom = arg;
        emit q->validFromChanged(arg);
    }
}

void UmsUserPrivate::setRoles(const QList<RoleSP> &newList)
{
    Q_Q(UmsUser);
    roles.clear();
    roles.append(newList);
    emit q->rolesChanged();
}

void UmsUserPrivate::setExpiresAt(const QDateTime &arg)
{
    Q_Q(UmsUser);
    if (expiresAt != arg) {
        expiresAt = arg;
        emit q->expiresAtChanged(arg);
    }
}
