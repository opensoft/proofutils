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
#ifndef PROOF_UMS_UMSTOKENINFOQMLWRAPPER_H
#define PROOF_UMS_UMSTOKENINFOQMLWRAPPER_H

#include "proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_global.h"
#include "proofnetwork/ums/proofnetworkums_types.h"

#include <QDateTime>
#include <QtQml/QQmlListProperty>

namespace Proof {
namespace Ums {
class UmsTokenInfoQmlWrapperPrivate;
class PROOF_NETWORK_UMS_EXPORT UmsTokenInfoQmlWrapper : public NetworkDataEntityQmlWrapper
{
    Q_OBJECT
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QDateTime expiresAt READ expiresAt NOTIFY expiresAtChanged)
    Q_PROPERTY(QDateTime validFrom READ validFrom NOTIFY validFromChanged)
    Q_PROPERTY(QString token READ token NOTIFY tokenChanged)
    Q_PROPERTY(Proof::Ums::UmsUserQmlWrapper *user READ user NOTIFY userChanged)

    Q_DECLARE_PRIVATE(UmsTokenInfoQmlWrapper)
public:
    explicit UmsTokenInfoQmlWrapper(const UmsTokenInfoSP &umsTokenInfo, QObject *parent = nullptr);
    ~UmsTokenInfoQmlWrapper();
    PROOF_NDE_WRAPPER_TOOLS(UmsTokenInfo)

    QString version() const;
    QDateTime expiresAt() const;
    QDateTime validFrom() const;
    QString token() const;
    Proof::Ums::UmsUserQmlWrapper *user() const;

signals:
    void versionChanged(const QString &version);
    void expiresAtChanged(const QDateTime &expiresAt);
    void validFromChanged(const QDateTime &validFrom);
    void tokenChanged(const QString &token);
    void userChanged(Proof::Ums::UmsUserQmlWrapper *user);

protected:
    void
    setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old = QSharedPointer<Proof::NetworkDataEntity>()) override;
};

} // namespace Ums
} // namespace Proof
#endif // PROOF_UMS_UMSTOKENINFOQMLWRAPPER_H
