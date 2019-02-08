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
#include "proofnetwork/ums/data/qmlwrappers/umstokeninfoqmlwrapper.h"

#include "proofnetwork/qmlwrappers/networkdataentityqmlwrapper_p.h"
#include "proofnetwork/ums/data/umstokeninfo.h"
#include "proofnetwork/ums/data/umsuser.h"

namespace Proof {
namespace Ums {
class UmsTokenInfoQmlWrapperPrivate : public NetworkDataEntityQmlWrapperPrivate
{
    Q_DECLARE_PUBLIC(UmsTokenInfoQmlWrapper)

    void updateUser();

    UmsUserQmlWrapper *user = nullptr;
};

} // namespace Ums
} // namespace Proof

using namespace Proof;
using namespace Proof::Ums;

UmsTokenInfoQmlWrapper::UmsTokenInfoQmlWrapper(const UmsTokenInfoSP &umsTokenInfo, QObject *parent)
    : NetworkDataEntityQmlWrapper(umsTokenInfo, *new UmsTokenInfoQmlWrapperPrivate, parent)
{
    setupEntity(); // NOLINT
}

UmsTokenInfoQmlWrapper::~UmsTokenInfoQmlWrapper()
{}

PROOF_NDE_WRAPPER_TOOLS_IMPL(UmsTokenInfo)

PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsTokenInfo, QString, version)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsTokenInfo, QDateTime, expiresAt)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsTokenInfo, QDateTime, validFrom)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(UmsTokenInfo, QString, token)

UmsUserQmlWrapper *UmsTokenInfoQmlWrapper::user() const
{
    Q_D_CONST(UmsTokenInfoQmlWrapper);
    return d->user;
}

void UmsTokenInfoQmlWrapper::setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old)
{
    Q_D(UmsTokenInfoQmlWrapper);
    UmsTokenInfoSP token = entity<UmsTokenInfo>();
    Q_ASSERT(token);

    connect(token.data(), &UmsTokenInfo::versionChanged, this, &UmsTokenInfoQmlWrapper::versionChanged);
    connect(token.data(), &UmsTokenInfo::expiresAtChanged, this, &UmsTokenInfoQmlWrapper::expiresAtChanged);
    connect(token.data(), &UmsTokenInfo::validFromChanged, this, &UmsTokenInfoQmlWrapper::validFromChanged);
    connect(token.data(), &UmsTokenInfo::tokenChanged, this, &UmsTokenInfoQmlWrapper::tokenChanged);
    connect(token.data(), &UmsTokenInfo::userChanged, entityConnectContext(), [d] { d->updateUser(); });

    auto castedOld = qSharedPointerCast<UmsTokenInfo>(old);
    if (castedOld) {
        if (token->version() != castedOld->version())
            emit versionChanged(token->version());
        if (token->expiresAt() != castedOld->expiresAt())
            emit expiresAtChanged(token->expiresAt());
        if (token->validFrom() != castedOld->validFrom())
            emit validFromChanged(token->validFrom());
        if (token->token() != castedOld->token())
            emit tokenChanged(token->token());
    }
    d->updateUser();
}

void UmsTokenInfoQmlWrapperPrivate::updateUser()
{
    Q_Q(UmsTokenInfoQmlWrapper);
    UmsTokenInfoSP token = q->entity<UmsTokenInfo>();
    if (!user)
        user = token->user()->toQmlWrapper(q);
    else
        user->setEntity(token->user());
    emit q->userChanged(user);
}
