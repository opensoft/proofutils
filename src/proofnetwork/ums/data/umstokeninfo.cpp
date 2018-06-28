#include "umstokeninfo.h"

#include "umsuser_p.h"

#include "proofnetwork/networkdataentity_p.h"
#include "proofnetwork/ums/data/umsuser.h"

#include <QJsonArray>

namespace Proof {
namespace Ums {
class UmsTokenInfoPrivate : public NetworkDataEntityPrivate
{
    Q_DECLARE_PUBLIC(UmsTokenInfo)

    void setVersion(const QString &arg);
    void setExpiresAt(const QDateTime &arg);
    void setValidFrom(const QDateTime &arg);
    UmsUserSP updateUser(const QString &arg);
    void setToken(const QString &arg);

    QString version;
    QDateTime expiresAt;
    QDateTime validFrom;

    UmsUserSP user;
    QString token;
};
} // namespace Ums
} // namespace Proof

using namespace Proof::Ums;

UmsTokenInfo::UmsTokenInfo() : NetworkDataEntity(*new UmsTokenInfoPrivate)
{}

UmsTokenInfo::~UmsTokenInfo()
{}

QString UmsTokenInfo::version() const
{
    Q_D(const UmsTokenInfo);
    return d->version;
}

QDateTime UmsTokenInfo::expiresAt() const
{
    Q_D(const UmsTokenInfo);
    return d->expiresAt;
}

QDateTime UmsTokenInfo::validFrom() const
{
    Q_D(const UmsTokenInfo);
    return d->validFrom;
}

UmsUserSP UmsTokenInfo::user() const
{
    Q_D(const UmsTokenInfo);
    return d->user;
}

QString UmsTokenInfo::token() const
{
    Q_D(const UmsTokenInfo);
    return d->token;
}

UmsTokenInfoQmlWrapper *UmsTokenInfo::toQmlWrapper(QObject *parent) const
{
    UmsTokenInfoSP castedSelf = castedSelfPtr<UmsTokenInfo>();
    Q_ASSERT(castedSelf);
    return new UmsTokenInfoQmlWrapper(castedSelf, parent);
}

UmsTokenInfoSP UmsTokenInfo::create()
{
    UmsTokenInfoSP result(new UmsTokenInfo);
    initSelfWeakPtr(result);
    return result;
}

UmsTokenInfoSP UmsTokenInfo::fromJson(const QJsonObject &tokenJson, const QString &token)
{
    if (!tokenJson.contains(QLatin1String("email")))
        return UmsTokenInfoSP();

    QString userName = tokenJson.value(QStringLiteral("email")).toString();
    UmsTokenInfoSP umsToken = create();
    UmsTokenInfoPrivate *const d = umsToken->d_func();
    d->setToken(token);
    d->setVersion(tokenJson.value(QStringLiteral("ver")).toString());
    d->setExpiresAt(QDateTime::fromTime_t(static_cast<uint>(tokenJson.value(QStringLiteral("exp")).toInt())));
    d->setValidFrom(QDateTime::fromTime_t(static_cast<uint>(tokenJson.value(QStringLiteral("nbf")).toInt())));

    UmsUserSP user = umsUsersCache().add(userName, UmsUser::create(userName));
    UmsUserPrivate *userD = user->d_func();

    QStringList roles;
    const QJsonArray rolesArray = tokenJson.value(QStringLiteral("roles")).toArray();
    for (const auto &roleValue : rolesArray)
        roles << roleValue.toString();

    userD->setFullName(tokenJson.value(QStringLiteral("name")).toString());
    userD->setEmail(tokenJson.value(QStringLiteral("email")).toString());
    userD->setRoles(roles);
    user->setFetched(true);
    d->updateUser(user->userName());
    umsToken->setFetched(true);

    return umsToken;
}

void UmsTokenInfo::updateSelf(const Proof::NetworkDataEntitySP &other)
{
    Q_D(UmsTokenInfo);
    UmsTokenInfoSP castedOther = qSharedPointerCast<UmsTokenInfo>(other);
    d->setVersion(castedOther->version());
    d->setExpiresAt(castedOther->expiresAt());
    d->setValidFrom(castedOther->validFrom());
    d->updateUser(castedOther->user()->userName());
    d->setToken(castedOther->token());

    NetworkDataEntity::updateSelf(other);
}

void UmsTokenInfoPrivate::setVersion(const QString &arg)
{
    Q_Q(UmsTokenInfo);
    if (version != arg) {
        version = arg;
        emit q->versionChanged(arg);
    }
}

void UmsTokenInfoPrivate::setExpiresAt(const QDateTime &arg)
{
    Q_Q(UmsTokenInfo);
    if (expiresAt != arg) {
        expiresAt = arg;
        emit q->expiresAtChanged(arg);
    }
}

void UmsTokenInfoPrivate::setValidFrom(const QDateTime &arg)
{
    Q_Q(UmsTokenInfo);
    if (validFrom != arg) {
        validFrom = arg;
        emit q->validFromChanged(arg);
    }
}

UmsUserSP UmsTokenInfoPrivate::updateUser(const QString &arg)
{
    Q_Q(UmsTokenInfo);
    return NDE::updateEntityField(user, arg, umsUsersCache(), &UmsUser::userName, q, &UmsTokenInfo::userChanged);
}

void UmsTokenInfoPrivate::setToken(const QString &arg)
{
    Q_Q(UmsTokenInfo);
    if (token != arg) {
        token = arg;
        emit q->tokenChanged(arg);
    }
}
