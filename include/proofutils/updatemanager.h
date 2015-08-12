#ifndef PROOF_UPDATEMANAGER_H
#define PROOF_UPDATEMANAGER_H

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

namespace Proof {

class UpdateManagerPrivate;
class PROOF_UTILS_EXPORT UpdateManager : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(UpdateManager)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(int timeout READ timeout WRITE setTimeout NOTIFY timeoutChanged)
    Q_PROPERTY(QString currentVersion READ currentVersion WRITE setCurrentVersion NOTIFY currentVersionChanged)
    Q_PROPERTY(QString packageName READ packageName WRITE setPackageName NOTIFY packageNameChanged)
public:
    explicit UpdateManager(QObject *parent = nullptr);
    ~UpdateManager();

    Q_INVOKABLE void update(const QString &password);

    bool enabled() const;
    int timeout() const;
    QString currentVersion() const;
    QString packageName() const;

    void setEnabled(bool arg);
    void setTimeout(int arg);
    void setCurrentVersion(const QString &arg);
    void setPackageName(const QString &arg);

signals:
    void enabledChanged(bool arg);
    void timeoutChanged(int arg);
    void currentVersionChanged(const QString &arg);
    void packageNameChanged(const QString &arg);
    void newVersionFound(const QString &version);
    void updateSucceeded();
    void updateFailed();
    void manualUpdateNeeded(const QString &version);
};

} // namespace Proof

#endif // PROOF_UPDATEMANAGER_H
