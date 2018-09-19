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
#ifndef PROOF_UMS_UMSTOKENINFO_H
#define PROOF_UMS_UMSTOKENINFO_H

#include "proofnetwork/networkdataentity.h"
#include "proofnetwork/ums/data/qmlwrappers/umstokeninfoqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_global.h"
#include "proofnetwork/ums/proofnetworkums_types.h"

namespace Proof {
namespace Ums {

class UmsTokenInfoPrivate;
class PROOF_NETWORK_UMS_EXPORT UmsTokenInfo : public NetworkDataEntity
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UmsTokenInfo)
public:
    ~UmsTokenInfo();

    QString version() const;
    QDateTime expiresAt() const;
    QDateTime validFrom() const;
    UmsUserSP user() const;
    QString token() const;

    UmsTokenInfoQmlWrapper *toQmlWrapper(QObject *parent = nullptr) const override;

    static UmsTokenInfoSP create();
    static UmsTokenInfoSP fromJson(const QJsonObject &tokenJson, const QString &token);

signals:
    void versionChanged(const QString &arg);
    void expiresAtChanged(const QDateTime &arg);
    void validFromChanged(const QDateTime &arg);
    void userChanged(const Proof::Ums::UmsUserSP &arg);
    void tokenChanged(const QString &arg);

protected:
    UmsTokenInfo();
    void updateSelf(const Proof::NetworkDataEntitySP &other) override;
};

} // namespace Ums
} // namespace Proof

#endif // PROOF_UMS_UMSTOKENINFO_H
