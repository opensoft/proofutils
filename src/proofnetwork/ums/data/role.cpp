#include "role.h"

#include "proofnetwork/networkdataentity_p.h"

#include <QJsonObject>

namespace Proof {
namespace Ums {

class RolePrivate : public NetworkDataEntityPrivate
{
    Q_DECLARE_PUBLIC(Role)

    void updateFrom(const Proof::NetworkDataEntitySP &other) override;

    void setLocation(const QString &arg);
    void setService(const QString &arg);
    void setName(const QString &arg);

    QString location;
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

QString Role::location() const
{
    Q_D(const Role);
    return d->location;
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

RoleSP Role::fromString(const QString &roleString)
{
    RoleSP role = create();
    RolePrivate * const d = role->d_func();
    role->setFetched(true);

    QStringList roleStringList = roleString.split(":");
    if (roleStringList.count() == 3)
        d->setLocation(roleStringList.takeFirst());

    if (roleStringList.count() != 2)
        return RoleSP();

    d->setService(roleStringList.at(0));
    d->setName(roleStringList.at(1));
    return role;
}

void RolePrivate::updateFrom(const Proof::NetworkDataEntitySP &other)
{
    RoleSP castedOther = qSharedPointerCast<Role>(other);
    setLocation(castedOther->location());
    setService(castedOther->service());
    setName(castedOther->name());

    NetworkDataEntityPrivate::updateFrom(other);
}

void RolePrivate::setLocation(const QString &arg)
{
    Q_Q(Role);
    if (location != arg) {
        location = arg;
        emit q->locationChanged(arg);
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

