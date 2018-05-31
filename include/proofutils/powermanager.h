#ifndef PROOF_POWERMANAGER_H
#define PROOF_POWERMANAGER_H

#include "proofutils_global.h"

#include "proofcore/proofobject.h"

namespace Proof {

class PowerManagerPrivate;
class PROOF_UTILS_EXPORT PowerManager : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(PowerManager)
public:
    explicit PowerManager(QObject *parent = nullptr);
    ~PowerManager();

    Q_INVOKABLE void restart(const QString &password = QLatin1String(QLatin1String("")));
    Q_INVOKABLE void powerOff(const QString &password = QLatin1String(QLatin1String("")));
    Q_INVOKABLE void restartApp();
};

} // namespace Proof

#endif // PROOF_POWERMANAGER_H
