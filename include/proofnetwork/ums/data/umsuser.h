#ifndef PROOF_UMS_USER_H
#define PROOF_UMS_USER_H

#include "proofnetwork/user.h"
#include "proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

#include "proofcore/objectscache.h"

#include <QJsonObject>
#include <QList>

namespace Proof {
namespace Ums {

class UmsUserPrivate;
class PROOF_NETWORK_UMS_EXPORT UmsUser : public User // clazy:exclude=ctor-missing-parent-argument
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UmsUser)
public:
    ~UmsUser();

    QStringList roles() const;

    UmsUserQmlWrapper *toQmlWrapper(QObject *parent = nullptr) const override;

    static UmsUserSP create(const QString &userName = QString());

signals:
    void rolesChanged(const QStringList &arg);

protected:
    explicit UmsUser(const QString &userName = QString());

private:
    friend class UmsTokenInfo;
};

PROOF_NETWORK_UMS_EXPORT ObjectsCache<QString, UmsUser> &umsUsersCache();

}
}

#endif // PROOF_UMS_USER_H
