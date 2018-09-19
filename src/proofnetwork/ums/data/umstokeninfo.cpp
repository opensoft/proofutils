/* Copyright 2018, OpenSoft Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *     * Neither the name of OpenSoft Inc. nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "proofnetwork/ums/data/umstokeninfo.h"

#include "proofnetwork/networkdataentity_p.h"
#include "proofnetwork/ums/data/umsuser.h"
#include "proofnetwork/ums/data/umsuser_p.h"

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
    Q_D_CONST(UmsTokenInfo);
    return d->version;
}

QDateTime UmsTokenInfo::expiresAt() const
{
    Q_D_CONST(UmsTokenInfo);
    return d->expiresAt;
}

QDateTime UmsTokenInfo::validFrom() const
{
    Q_D_CONST(UmsTokenInfo);
    return d->validFrom;
}

UmsUserSP UmsTokenInfo::user() const
{
    Q_D_CONST(UmsTokenInfo);
    return d->user;
}

QString UmsTokenInfo::token() const
{
    Q_D_CONST(UmsTokenInfo);
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
    roles.reserve(rolesArray.count());
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
