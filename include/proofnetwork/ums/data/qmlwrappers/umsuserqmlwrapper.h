#ifndef PROOF_UMS_UMSUSERQMLWRAPPER_H
#define PROOF_UMS_UMSUSERQMLWRAPPER_H

#include "proofnetwork/qmlwrappers/userqmlwrapper.h"
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
    Q_PROPERTY(QStringList roles READ roles NOTIFY rolesChanged)

    Q_DECLARE_PRIVATE(UmsUserQmlWrapper)
public:
    UmsUserQmlWrapper(const UmsUserSP &user, QObject *parent = nullptr);
    PROOF_NDE_WRAPPER_TOOLS(UmsUser)

    QStringList roles() const;

signals:
    void rolesChanged(const QStringList &roles);

protected:
    void setupEntity(const QSharedPointer<Proof::NetworkDataEntity> &old = QSharedPointer<Proof::NetworkDataEntity>()) override;
};

}
}
#endif // PROOF_UMS_UMSUSERQMLWRAPPER_H
