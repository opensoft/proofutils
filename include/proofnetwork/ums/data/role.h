#ifndef PROOF_UMS_ROLE_H
#define PROOF_UMS_ROLE_H

#include "proofnetwork/networkdataentity.h"
#include "proofnetwork/ums/data/qmlwrappers/roleqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

namespace Proof {
namespace Ums {

class RolePrivate;

class PROOF_NETWORK_UMS_EXPORT Role : public NetworkDataEntity // clazy:exclude=ctor-missing-parent-argument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Role)
public:
    ~Role();

    QString location() const;
    QString service() const;
    QString name() const;

    RoleQmlWrapper *toQmlWrapper(QObject *parent = nullptr) const override;

    static RoleSP create();
    static RoleSP fromString(const QString &roleString);

signals:
    void locationChanged(const QString &arg);
    void serviceChanged(const QString &arg);
    void nameChanged(const QString &arg);

protected:
    explicit Role();
};

}
}

#endif // PROOF_UMS_ROLE_H
