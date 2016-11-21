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
    Q_PROPERTY(bool supported READ supported CONSTANT)
public:
    explicit NetworkConfigurationManager(QObject *parent = nullptr);
    ~NetworkConfigurationManager();

    bool supported() const;
    Q_INVOKABLE bool passwordSupported() const;
    Q_INVOKABLE void checkPassword(const QString &password);
    Q_INVOKABLE QVariantMap addresses() const;
    Q_INVOKABLE void fetchNetworkInterfaces();
    Q_INVOKABLE void fetchNetworkConfiguration(const QString &networkAdapterDescription);
    Q_INVOKABLE void writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                               const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password);

signals:
    void passwordChecked(bool isCorrect);
    void networkInterfacesFetched(const QStringList &networkInterfaces);
    void networkConfigurationFetched(bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                     const QString &gateway, const QString &preferredDns, const QString &alternateDns);
    void networkConfigurationWrote();

    void errorOccurred(long moduleCode, long errorCode, const QString &errorMessage, bool userFriendly);
};

} // namespace Proof

#endif // PROOF_NETWORKCONFIGURATIONMANAGER_H
