#include "umstokeninfoqmlwrapper.h"

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
    setupEntity();
}

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
