#ifndef PROOF_UMS_UMSUSERQMLWRAPPER_H
#define PROOF_UMS_UMSUSERQMLWRAPPER_H

#include "proofnetwork/qmlwrappers/userqmlwrapper.h"
#include "proofnetwork/ums/data/qmlwrappers/roleqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

#include <QtQml/QQmlListProperty>
#include <QDateTime>

namespace Proof {
namespace Ums {
class UmsUserQmlWrapperPrivate;
class PROOF_NETWORK_UMS_EXPORT UmsUserQmlWrapper : public UserQmlWrapper
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(QDateTime expiresAt READ expiresAt NOTIFY expiresAtChanged)
    Q_PROPERTY(QDateTime validFrom READ validFrom NOTIFY validFromChanged)

    Q_PROPERTY(QQmlListProperty<Proof::Ums::RoleQmlWrapper> roles READ roles NOTIFY rolesChanged)

    Q_DECLARE_PRIVATE(UmsUserQmlWrapper)
public:
    UmsUserQmlWrapper(const UmsUserSP &user, QObject *parent = 0);
    PROOF_NDE_WRAPPER_TOOLS(UmsUser)

    QString id() const;
    QString version() const;
    QDateTime expiresAt() const;
    QDateTime validFrom() const;
    QQmlListProperty<Proof::Ums::RoleQmlWrapper> roles() const;

signals:
    void idChanged(const QString &id);
    void versionChanged(const QString &version);
    void expiresAtChanged(const QDateTime &expiresAt);
    void validFromChanged(const QDateTime &validFrom);

    void rolesChanged(const QQmlListProperty<Proof::Ums::RoleQmlWrapper> &roles);

protected:
    void setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old = QSharedPointer<Proof::NetworkDataEntity>()) override;
};

}
}
#endif // PROOF_UMS_UMSUSERQMLWRAPPER_H
