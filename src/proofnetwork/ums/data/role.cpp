#include "role.h"

#include "proofnetwork/networkdataentity_p.h"

#include <QJsonObject>

namespace Proof {
namespace Ums {

class RolePrivate : public NetworkDataEntityPrivate
{
    Q_DECLARE_PUBLIC(Role)

    void updateFrom(const Proof::NetworkDataEntitySP &other) override;

    void setLocale(const QString &arg);
    void setService(const QString &arg);
    void setName(const QString &arg);

    QString locale;
    QString service;
    QString name;
};

}
}

using namespace Proof;
using namespace Proof::Ums;

Role::Role()
    : NetworkDataEntity(*new RolePrivate)
{
}

Role::~Role()
{
}

QString Role::locale() const
{
    Q_D(const Role);
    return d->locale;
}

QString Role::service() const
{
    Q_D(const Role);
    return d->service;
}

QString Role::name() const
{
    Q_D(const Role);
    return d->name;
}

RoleQmlWrapper *Role::toQmlWrapper(QObject *parent) const
{
    Q_D(const Role);
    RoleSP castedSelf = qSharedPointerCast<Role>(d->weakSelf);
    Q_ASSERT(castedSelf);
    return new RoleQmlWrapper(castedSelf, parent);
}

/*!
* Create instance
*/
RoleSP Role::create()
{
    RoleSP result(new Role());
    initSelfWeakPtr(result);
    return result;
}

/*!
* Create instance from JSON
*/
RoleSP Role::fromJson(const QJsonObject &roleJson)
{
    if (!roleJson.contains(QLatin1String("name")))
        return RoleSP();

    RoleSP role = create();
    RolePrivate * const d = role->d_func();
    role->setFetched(true);
    d->setLocale(roleJson.value(QStringLiteral("loc")).toString(QLatin1String("")));
    d->setService(roleJson.value(QStringLiteral("srv")).toString(QLatin1String("")));
    d->setName(roleJson.value(QStringLiteral("name")).toString(QLatin1String("")));

    return role;
}

void RolePrivate::updateFrom(const Proof::NetworkDataEntitySP &other)
{
    RoleSP castedOther = qSharedPointerCast<Role>(other);
    setLocale(castedOther->locale());
    setService(castedOther->service());
    setName(castedOther->name());

    NetworkDataEntityPrivate::updateFrom(other);
}

void RolePrivate::setLocale(const QString &arg)
{
    Q_Q(Role);
    if (locale != arg) {
        locale = arg;
        emit q->localeChanged(arg);
    }
}

void RolePrivate::setService(const QString &arg)
{
    Q_Q(Role);
    if (service != arg) {
        service = arg;
        emit q->serviceChanged(arg);
    }
}

void RolePrivate::setName(const QString &arg)
{
    Q_Q(Role);
    if (name != arg) {
        name = arg;
        emit q->nameChanged(arg);
    }
}

