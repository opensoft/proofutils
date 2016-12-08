#ifndef PROOF_NETWORKCONFIGURATIONMANAGER_H
#define PROOF_NETWORKCONFIGURATIONMANAGER_H

// Use this class only from gui application

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

namespace Proof {

class Settings;
class NetworkConfigurationManagerPrivate;
class PROOF_UTILS_EXPORT NetworkConfigurationManager : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NetworkConfigurationManager)
    Q_PROPERTY(bool ipSettingsSupported READ ipSettingsSupported CONSTANT)
public:
    //TODO: 1.0: remove first arg when CoreApplication and GuiApplication will be castable to each other
    explicit NetworkConfigurationManager(Settings *settings, QObject *parent = nullptr);
    ~NetworkConfigurationManager();

    bool ipSettingsSupported() const;
    Q_INVOKABLE bool passwordSupported() const;
    Q_INVOKABLE void checkPassword(const QString &password);
    Q_INVOKABLE QVariantMap addresses() const;
    Q_INVOKABLE void fetchNetworkInterfaces();
    Q_INVOKABLE void fetchNetworkConfiguration(const QString &networkAdapterDescription);
    Q_INVOKABLE void writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                               const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password);
    Q_INVOKABLE QStringList proxyTypes() const;
    Q_INVOKABLE void fetchProxySettings();
    Q_INVOKABLE void writeProxySettings(bool enabled, const QString &host, quint16 port, const QString &type, const QString &userName, const QString &password);

signals:
    void passwordChecked(bool isCorrect);
    void networkInterfacesFetched(const QStringList &networkInterfaces);
    void networkConfigurationFetched(bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                     const QString &gateway, const QString &preferredDns, const QString &alternateDns);
    void networkConfigurationWrote();

    void proxySettingsFetched(bool proxyEnabled, const QString &host, quint16 port, const QString &type, const QString &userName, const QString &password);
    void proxySettingsWrote();

    void errorOccurred(long moduleCode, long errorCode, const QString &errorMessage, bool userFriendly);
};

} // namespace Proof

#endif // PROOF_NETWORKCONFIGURATIONMANAGER_H
