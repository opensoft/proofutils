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
public:
    UpdateManager(const QString &packageName, const QString &currentVersion, QObject *parent = nullptr);
    void checkForUpdates();
    void update(const QString &password);

signals:
    void newVersionFound(const QString &version);
    void updateSucceeded();
    void updateFailed();
    void manualUpdateNeeded();

};

} // namespace Proof

#endif // PROOF_UPDATEMANAGER_H
