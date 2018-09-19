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
#include "proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.h"

#include "proofnetwork/qmlwrappers/userqmlwrapper_p.h"
#include "proofnetwork/ums/data/umsuser.h"

namespace Proof {
namespace Ums {
class UmsUserQmlWrapperPrivate : public UserQmlWrapperPrivate
{
    Q_DECLARE_PUBLIC(UmsUserQmlWrapper)
};

} // namespace Ums
} // namespace Proof

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

    UmsUserSP user = entity<UmsUser>();
    Q_ASSERT(user);

    connect(user.data(), &UmsUser::rolesChanged, this, &UmsUserQmlWrapper::rolesChanged);

    auto castedOld = qSharedPointerCast<UmsUser>(old);
    if (castedOld) {
        if (user->roles() != castedOld->roles())
            emit rolesChanged(user->roles());
    }
}
