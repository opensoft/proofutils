#ifndef PROOF_NETWORKCONFIGURATIONMANAGER_H
#define PROOF_NETWORKCONFIGURATIONMANAGER_H

#include "proofutils_global.h"
#include "proofcore/proofobject.h"

namespace Proof {

class Settings;
class NetworkConfigurationManagerPrivate;
class PROOF_UTILS_EXPORT NetworkConfigurationManager : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NetworkConfigurationManager)
    Q_ENUMS(ProxyType)
    Q_PROPERTY(bool ipSettingsSupported READ ipSettingsSupported CONSTANT)
    Q_PROPERTY(bool vpnSettingsSupported READ vpnSettingsSupported CONSTANT)

    enum class VpnState {
        Starting = 0,
        Stopping = 1,
        Other = 2
    };

public:
    enum class ProxyType {
        NoProxyType,
        HttpProxyType,
        SocksProxyType,
        AutoConfigurationProxyType,
        AutoDiscoveryProxyType
    };

    explicit NetworkConfigurationManager(QObject *parent = nullptr);
    ~NetworkConfigurationManager();

    bool ipSettingsSupported() const;
    bool vpnSettingsSupported() const;
    Q_INVOKABLE void checkVpnCanBeControlled();
    Q_INVOKABLE bool passwordSupported() const;
    Q_INVOKABLE void checkPassword(const QString &password);
    Q_INVOKABLE QVariantMap addresses() const;
    Q_INVOKABLE void fetchNetworkInterfaces();
    Q_INVOKABLE void fetchNetworkConfiguration(const QString &networkAdapterDescription);
    Q_INVOKABLE void writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                               const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password);
    Q_INVOKABLE QList<int> proxyTypes() const;
    Q_INVOKABLE void fetchProxySettings();
    Q_INVOKABLE void writeProxySettings(ProxyType proxyType, const QString &host, quint16 port, const QString &userName, const QString &password);
    Q_INVOKABLE bool vpnEnabled();
    Q_INVOKABLE void fetchVpnConfiguration();
    Q_INVOKABLE void writeVpnConfiguration(const QString &absoluteFilePath, const QString &configuration, const QString &password);
    Q_INVOKABLE void turnOnVpn(const QString &password);
    Q_INVOKABLE void turnOffVpn(const QString &password);
    Q_INVOKABLE void skipSwitchingVpnCheck();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void vpnCanBeControlledChecked(bool controllable);
    void passwordChecked(bool isCorrect);
    void networkInterfacesFetched(const QStringList &networkInterfaces);
    void networkConfigurationFetched(bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                     const QString &gateway, const QString &preferredDns, const QString &alternateDns);
    void networkConfigurationWritten();

    void proxySettingsFetched(Proof::NetworkConfigurationManager::ProxyType type, const QString &url, quint16 port, const QString &userName, const QString &password);
    void proxySettingsWritten();

    void vpnConfigurationFetched(const QString &absoluteFilePath, const QString &configuration);
    void vpnConfigurationWritten();
    void vpnTurnedOn();
    void vpnTurnedOff();

private:
    void startTimerForCheckVpnState(VpnState vpnState);

    int m_checkVpnStateTimerId = -1;
    VpnState m_vpnState = VpnState::Other;
    int m_currentTimeCheckVpn = 0;
};

} // namespace Proof

Q_DECLARE_METATYPE(Proof::NetworkConfigurationManager::ProxyType)

#endif // PROOF_NETWORKCONFIGURATIONMANAGER_H
