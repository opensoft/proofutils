#ifndef PROOF_UPDATEMANAGER_H
#define PROOF_UPDATEMANAGER_H

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

#include <QStringList>

namespace Proof {

class UpdateManagerPrivate;
class PROOF_UTILS_EXPORT UpdateManager : public ProofObject
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(QString currentVersion READ currentVersion WRITE setCurrentVersion NOTIFY currentVersionChanged)
    Q_PROPERTY(QString packageName READ packageName WRITE setPackageName NOTIFY packageNameChanged)
    Q_PROPERTY(QStringList rollbackVersions READ rollbackVersions NOTIFY rollbackVersionsChanged)
    Q_DECLARE_PRIVATE(UpdateManager)

public:
    explicit UpdateManager(QObject *parent = nullptr);
    ~UpdateManager();

    Q_INVOKABLE void update(const QString &password);
    Q_INVOKABLE void installVersion(const QString &version, const QString &password);
    Q_INVOKABLE void checkPassword(const QString &password);

    bool enabled() const;
    int timeout() const;
    QString currentVersion() const;
    QString packageName() const;
    QStringList rollbackVersions() const;

    void setEnabled(bool arg);
    void setTimeout(int arg);
    void setCurrentVersion(const QString &arg);
    void setPackageName(const QString &arg);

signals:
    void enabledChanged(bool enabled);
    void timeoutChanged(int timeout);
    void currentVersionChanged(const QString &currentVersion);
    void packageNameChanged(const QString &packageName);
    void rollbackVersionsChanged(const QStringList &rollbackVersions);
    void newVersionFound(const QString &version);
    void updateSucceeded();
    void updateFailed();
    void manualUpdateNeeded(const QString &version);
    void installationSucceeded();
    void installationFailed();
    void passwordChecked(bool isCorrect);
};

} // namespace Proof

#endif // PROOF_UPDATEMANAGER_H
