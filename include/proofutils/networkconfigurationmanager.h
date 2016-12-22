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
    Q_PROPERTY(bool vpnSettingsSupported READ vpnSettingsSupported CONSTANT)

    enum class VpnState {
        Starting = 0,
        Stopping = 1,
        Other = 2
    };

public:
    //TODO: 1.0: remove first arg when CoreApplication and GuiApplication will be castable to each other
    explicit NetworkConfigurationManager(Settings *settings, QObject *parent = nullptr);
    ~NetworkConfigurationManager();

    bool ipSettingsSupported() const;
    bool vpnSettingsSupported() const;
    Q_INVOKABLE bool vpnCanBeControlled();
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
    Q_INVOKABLE bool vpnEnabled();
    Q_INVOKABLE void fetchVpnConfiguration();
    Q_INVOKABLE void writeVpnConfiguration(const QString &absoluteFilePath, const QString &configuration, const QString &password);
    Q_INVOKABLE void turnOnVpn(const QString &password);
    Q_INVOKABLE void turnOffVpn(const QString &password);
    Q_INVOKABLE void skipSwitchingVpnCheck();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void passwordChecked(bool isCorrect);
    void networkInterfacesFetched(const QStringList &networkInterfaces);
    void networkConfigurationFetched(bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                     const QString &gateway, const QString &preferredDns, const QString &alternateDns);
    void networkConfigurationWritten();

    void proxySettingsFetched(bool proxyEnabled, const QString &host, quint16 port, const QString &type, const QString &userName, const QString &password);
    void proxySettingsWritten();

    void vpnConfigurationFetched(const QString &absoluteFilePath, const QString &configuration);
    void vpnConfigurationWritten();
    void vpnTurnedOn();
    void vpnTurnedOff();

    void errorOccurred(long moduleCode, long errorCode, const QString &errorMessage, bool userFriendly);

private:
    void startTimerForCheckVpnState(VpnState vpnState);

    int m_checkVpnStateTimerId = -1;
    VpnState m_vpnState = VpnState::Other;
    int m_currentTimeCheckVpn = 0;
};

} // namespace Proof

#endif // PROOF_NETWORKCONFIGURATIONMANAGER_H
