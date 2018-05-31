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
    UmsTokenInfoQmlWrapper(const UmsTokenInfoSP &umsTokenInfo, QObject *parent = nullptr);
    PROOF_NDE_WRAPPER_TOOLS(UmsUser)

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
