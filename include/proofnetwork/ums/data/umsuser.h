#ifndef PROOF_UMS_USER_H
#define PROOF_UMS_USER_H

#include "proofnetwork/user.h"
#include "proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.h"
#include "proofnetwork/ums/proofnetworkums_types.h"
#include "proofnetwork/ums/proofnetworkums_global.h"

#include <QJsonObject>
#include <QList>

namespace Proof {
namespace Ums {

class UmsUserPrivate;
class PROOF_NETWORK_UMS_EXPORT UmsUser : public User
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UmsUser)
public:
    ~UmsUser();

    QString id() const;
    QString version() const;
    QDateTime expiresAt() const;
    QDateTime validFrom() const;
    QList<RoleSP> roles() const;

    UmsUserQmlWrapper *toQmlWrapper(QObject *parent = 0) const override;

    static UmsUserSP create(const QString &userName = QString());
    static UmsUserSP fromJson(const QJsonObject &userJson);

signals:
    void idChanged(const QString &arg);
    void versionChanged(const QString &arg);
    void expiresAtChanged(const QDateTime &arg);
    void validFromChanged(const QDateTime &arg);
    void rolesChanged();

protected:
    explicit UmsUser(const QString &userName = QString());
};

}
}

#endif // PROOF_UMS_USER_H
