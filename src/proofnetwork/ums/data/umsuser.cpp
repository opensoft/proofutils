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
#include "proofnetwork/ums/data/umsuser.h"

#include "proofnetwork/ums/data/umsuser_p.h"

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
    Q_D_CONST(UmsUser);
    return d->roles;
}

UmsUserQmlWrapper *UmsUser::toQmlWrapper(QObject *parent) const
{
    UmsUserSP castedSelf = castedSelfPtr<UmsUser>();
    Q_ASSERT(castedSelf);
    return new UmsUserQmlWrapper(castedSelf, parent);
}

UmsUserSP UmsUser::create(const QString &userName)
{
    UmsUserSP result(new UmsUser(userName));
    initSelfWeakPtr(result);
    return result;
}

void UmsUser::updateSelf(const NetworkDataEntitySP &other)
{
    Q_D(UmsUser);
    UmsUserSP castedOther = qSharedPointerCast<UmsUser>(other);
    d->setRoles(castedOther->roles());

    User::updateSelf(other);
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
