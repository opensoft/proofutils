#ifndef PROOF_NETWORKCONFIGURATIONMANAGER_H
#define PROOF_NETWORKCONFIGURATIONMANAGER_H

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

namespace Proof {

class NetworkConfigurationManagerPrivate;
class PROOF_UTILS_EXPORT NetworkConfigurationManager : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NetworkConfigurationManager)
public:
    explicit NetworkConfigurationManager(QObject *parent = nullptr);
    ~NetworkConfigurationManager();

    Q_INVOKABLE QVariantMap addresses() const;
    Q_INVOKABLE QStringList networkInterfaces();
    Q_INVOKABLE void fetchNetworkConfiguration(const QString &networkAdapterDescription);
    Q_INVOKABLE void writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address,
                                               const QString &subnetMask, const QString &gateway, const QString &preferredDns, const QString &alternateDns);

signals:
    void networkAdaptersFetched(const QStringList &networkInterfaces);
    void networkConfigurationFetched(bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                     const QString &gateway, const QString &preferredDns, const QString &alternateDns);
    void networkConfigurationWrote();

    void errorOccurred(long moduleCode, long errorCode, const QString &errorMessage, bool userFriendly);
};

} // namespace Proof

#endif // PROOF_NETWORKCONFIGURATIONMANAGER_H
