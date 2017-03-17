#ifndef ROLEQMLWRAPPER_H
#define ROLEQMLWRAPPER_H

#include "proofnetwork/qmlwrappers/userqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

namespace Proof {
namespace Ums {
class RoleQmlWrapperPrivate;
class PROOF_NETWORK_UMS_EXPORT RoleQmlWrapper : public NetworkDataEntityQmlWrapper
{
    Q_OBJECT
    Q_PROPERTY(QString locale READ locale NOTIFY localeChanged)
    Q_PROPERTY(QString service READ service NOTIFY serviceChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_DECLARE_PRIVATE(RoleQmlWrapper)
public:
    RoleQmlWrapper(const RoleSP &role, QObject *parent = nullptr);
    PROOF_NDE_WRAPPER_TOOLS(Role)

    QString locale() const;
    QString service() const;
    QString name() const;

signals:
    void localeChanged(const QString &locale);
    void serviceChanged(const QString &service);
    void nameChanged(const QString &name);

protected:
    void setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old = QSharedPointer<Proof::NetworkDataEntity>()) override;
};

}
}
#endif // ROLEQMLWRAPPER_H
