#include "networkconfigurationmanager.h"

#include "proofcore/proofobject_p.h"

#include <QNetworkInterface>
#include <QProcess>
#include <QFile>

static const QString NETWORK_SETTINGS_FILE = "/etc/network/interfaces";
static const QString NETWORK_SETTINGS_FILE_TMP = "/tmp/interfaces_tmp";

static const QString STATIC_IP = "static";
static const QString DYNAMIC_IP = "dhcp";

static const QString ADDRESS = "address";
static const QString NETMASK = "netmask";
static const QString GATEWAY = "gateway";
static const QString DNS_NAMESERVERS = "dns-nameservers";

static const QString REGEXP_IP("(((?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))");

namespace {

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(Proof::NetworkConfigurationManagerPrivate *networkConfigurationManager);
    QStringList networkInterfaces();
    void checkPassword(const QString &password);
    void fetchNetworkConfiguration(const QString &networkAdapterDescription);
    void writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                   const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password);

private:
    Proof::NetworkConfigurationManagerPrivate *networkConfigurationManager;
};

}

namespace Proof {

struct NetworkConfiguration {
    QString description;
    QString index;
    bool dhcpEnabled = false;
    QString ipv4Address;
    QString subnetMask;
    QString gateway;
    QString preferredDns;
    QString alternateDns;
};

class NetworkConfigurationManagerPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(NetworkConfigurationManager)
public:
    void checkPassword(const QString &password);
    QStringList networkInterfaces();
    void fetchNetworkConfiguration(const QString &networkAdapterDescription);
    void writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                   const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password);
private:
    NetworkConfiguration fetchNetworkConfigurationPrivate(const QString &networkAdapterDescription);
    bool enterPassword(QProcess &process, const QString &password);

    WorkerThread *thread;
};

NetworkConfigurationManager::NetworkConfigurationManager(QObject *parent)
    : ProofObject(*new NetworkConfigurationManagerPrivate, parent)
{
    Q_D(NetworkConfigurationManager);
    d->thread = new WorkerThread(d);
    d->thread->start();
}

NetworkConfigurationManager::~NetworkConfigurationManager()
{
    Q_D(NetworkConfigurationManager);
    d->thread->quit();
    d->thread->wait(1000);
    delete d->thread;
}

bool NetworkConfigurationManager::passwordSupported() const
{
#ifdef Q_OS_LINUX
    return true;
#else
    return false;
#endif
}

void NetworkConfigurationManager::checkPassword(const QString &password)
{
    Q_D(NetworkConfigurationManager);
    d->checkPassword(password);
}

QVariantMap NetworkConfigurationManager::addresses() const
{
    QVariantMap addresses;
    for (const auto &interface : QNetworkInterface::allInterfaces()) {
        for (const auto &address : interface.addressEntries()) {
            auto ip = address.ip();
            if (ip.isLoopback())
                continue;
            QVariantList list = addresses.value(interface.humanReadableName(), QVariantList{}).toList();
            list << ip.toString();
            addresses[interface.humanReadableName()] = list;
        }
    }
    return addresses;
}

QStringList NetworkConfigurationManager::networkInterfaces()
{
    Q_D(NetworkConfigurationManager);
    return d->networkInterfaces();
}

void NetworkConfigurationManager::fetchNetworkConfiguration(const QString &networkAdapterDescription)
{
    Q_D(NetworkConfigurationManager);
    d->fetchNetworkConfiguration(networkAdapterDescription);
}

void NetworkConfigurationManager::writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                                            const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password)
{
    Q_D(NetworkConfigurationManager);
    d->writeNetworkConfiguration(networkAdapterDescription, dhcpEnabled, ipv4Address,
                                 subnetMask, gateway, preferredDns, alternateDns, password);
}

void NetworkConfigurationManagerPrivate::checkPassword(const QString &password)
{
    Q_Q(NetworkConfigurationManager);
    if (ProofObject::call(thread, &WorkerThread::checkPassword, password))
        return;

#ifdef Q_OS_LINUX
    QProcess checker;
    checker.setProcessChannelMode(QProcess::MergedChannels);
    checker.start(QString("sudo -S -k pwd"));
    if (checker.error() == QProcess::UnknownError)
        emit q->passwordChecked(enterPassword(checker, password));
    else
        qCDebug(proofUtilsNetworkConfigurationLog) << "Process couldn't be started" << checker.error() << checker.errorString();
#else
    Q_UNUSED(password);
    qCDebug(proofUtilsNetworkConfigurationLog) << "Password check is not supported for this platform";
    emit q->passwordChecked(true);
#endif
}

QStringList NetworkConfigurationManagerPrivate::networkInterfaces()
{
    QStringList result;
    if (!ProofObject::call(thread, &WorkerThread::networkInterfaces, Call::Block, result)) {
#ifndef Q_OS_WIN
        QFile settingsFile(NETWORK_SETTINGS_FILE);
        if (settingsFile.open(QIODevice::ReadOnly)) {
            while (!settingsFile.atEnd()) {
                QString line = QString(settingsFile.readLine()).trimmed();
                if (line.startsWith("iface")) {
                    QStringList ifaceLine = line.split(" ", QString::SkipEmptyParts);
                    if (ifaceLine.count() > 1)
                        result.append(ifaceLine.at(1));
                }
            }
        } else {
            qCWarning(proofUtilsNetworkConfigurationLog) << NETWORK_SETTINGS_FILE <<  "can't be opened:" << settingsFile.errorString();
        }
#endif

        for (const auto &interface : QNetworkInterface::allInterfaces()) {
            if (interface.flags().testFlag(QNetworkInterface::IsLoopBack))
                result.removeAll(interface.humanReadableName());
            else if (!result.contains(interface.humanReadableName()))
                result.append(interface.humanReadableName());
        }
    }
    return result;
}

void NetworkConfigurationManagerPrivate::fetchNetworkConfiguration(const QString &networkAdapterDescription)
{
    if (ProofObject::call(thread, &WorkerThread::fetchNetworkConfiguration, networkAdapterDescription))
        return;

    Q_Q(NetworkConfigurationManager);
    NetworkConfiguration networkConfiguration = fetchNetworkConfigurationPrivate(networkAdapterDescription);
    emit q->networkConfigurationFetched(networkConfiguration.dhcpEnabled, networkConfiguration.ipv4Address, networkConfiguration.subnetMask,
                                        networkConfiguration.gateway, networkConfiguration.preferredDns, networkConfiguration.alternateDns);
}

NetworkConfiguration NetworkConfigurationManagerPrivate::fetchNetworkConfigurationPrivate(const QString &networkAdapterDescription)
{
    Q_Q(NetworkConfigurationManager);

    NetworkConfiguration networkConfiguration;
    networkConfiguration.description = networkAdapterDescription;
    for (const auto &interface : QNetworkInterface::allInterfaces()) {
        if (interface.humanReadableName() == networkAdapterDescription) {
            networkConfiguration.index = QString::number(interface.index());
            break;
        }
    }

#ifdef Q_OS_WIN
    QProcess readInfoProcess;
    readInfoProcess.setReadChannel(QProcess::StandardOutput);
    QString splitSimbol = "\";\"";
    readInfoProcess.start("PowerShell", {"-command", QString("$WMI = Get-WmiObject -Class Win32_NetworkAdapterConfiguration -Filter {InterfaceIndex LIKE '%1'};"
                                         "$WMI.DHCPEnabled; %2; $WMI.IPAddress; %2; $WMI.IPSubnet; %2; $WMI.DefaultIPGateway; %2; $WMI.DNSServerSearchOrder;").arg(networkConfiguration.index).arg(splitSimbol)});
    readInfoProcess.waitForStarted();
    if (readInfoProcess.error() != QProcess::UnknownError) {
        qCDebug(proofUtilsNetworkConfigurationLog) <<  "PowerShell can't be started:" << readInfoProcess.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't read network configuration", true);
        return NetworkConfiguration();
    }
    readInfoProcess.waitForFinished();
    QStringList adaptersInfo = QString(readInfoProcess.readAll()).split(splitSimbol.remove("\"") , QString::SkipEmptyParts);
    for (int i = 0; i < adaptersInfo.count(); ++i) {
        if (i == 0) {
            networkConfiguration.dhcpEnabled = adaptersInfo.at(i).toLower().contains("true");
            continue;
        }

        QRegExp regExpIp(REGEXP_IP);
        if (regExpIp.indexIn(adaptersInfo.at(i)) != -1) {
            QString ip = regExpIp.cap();
            switch (i) {
            case 1:
                networkConfiguration.ipv4Address = ip;
                break;
            case 2:
                networkConfiguration.subnetMask = ip;
                break;
            case 3:
                networkConfiguration.gateway = ip;
                break;
            case 4:
                networkConfiguration.preferredDns = ip;
                regExpIp.indexIn(QString(adaptersInfo.at(i)).remove(networkConfiguration.preferredDns));
                networkConfiguration.alternateDns = regExpIp.cap();
                break;
            default:
                break;
            }
        }
    }
#else
    QFile settingsFile(NETWORK_SETTINGS_FILE);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        qCDebug(proofUtilsNetworkConfigurationLog) << NETWORK_SETTINGS_FILE <<  "can't be opened:" << settingsFile.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't read network configuration", true);
        return NetworkConfiguration();
    }
    bool isParsingInterface = false;
    QRegExp regExpIp(REGEXP_IP);
    while (!settingsFile.atEnd()) {
        QString line = QString(settingsFile.readLine()).trimmed();
        if (line.startsWith("iface")) {
            if (isParsingInterface) {
                break;
            } else if (line.contains(networkAdapterDescription)) {
                isParsingInterface = true;
                networkConfiguration.dhcpEnabled = !line.contains(STATIC_IP);
            }
        } else if (!isParsingInterface) {
            continue;
        } else if (regExpIp.indexIn(line) != -1) {
            if (line.startsWith(ADDRESS)) {
                networkConfiguration.ipv4Address = regExpIp.cap();
            } else if (line.startsWith(NETMASK)) {
                networkConfiguration.subnetMask = regExpIp.cap();
            } else if (line.startsWith(GATEWAY)) {
                networkConfiguration.gateway = regExpIp.cap();
            } else if (line.startsWith(DNS_NAMESERVERS)) {
                networkConfiguration.preferredDns = regExpIp.cap();
                regExpIp.indexIn(QString(line).remove(networkConfiguration.preferredDns));
                networkConfiguration.alternateDns = regExpIp.cap();
            }
        }
    }
#endif

    return networkConfiguration;
}

bool NetworkConfigurationManagerPrivate::enterPassword(QProcess &process, const QString &password)
{
    QByteArray readBuffer;
    QByteArray currentRead;

    if (!process.waitForReadyRead()) {
        qCDebug(proofUtilsNetworkConfigurationLog) << "No answer from command. Returning";
        return false;
    }
    currentRead = process.readAll();
    readBuffer.append(currentRead);
    currentRead = currentRead.trimmed();
    if (currentRead.contains("[sudo]") || currentRead.contains("password for")) {
        process.write(QString("%1\n").arg(password).toLatin1());
        process.waitForReadyRead();
        currentRead = process.readAll();
        readBuffer.append(currentRead);
        currentRead = currentRead.trimmed();

        if (currentRead.contains("is not in the sudoers")) {
            qCDebug(proofUtilsNetworkConfigurationLog) << "User not in sudoers list; log:\n" << readBuffer;
            return false;
        }
        if (currentRead.contains("Sorry, try again")) {
            qCDebug(proofUtilsNetworkConfigurationLog) << "Sudo rejected the password; log:\n" << readBuffer;
            return false;
        }
    }
    process.waitForFinished();
    qCDebug(proofUtilsNetworkConfigurationLog) << "Exitcode =" << process.exitCode();
    return process.exitCode() == 0;
}

void NetworkConfigurationManagerPrivate::writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                                                   const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password)
{
    Q_Q(NetworkConfigurationManager);
    if (ProofObject::call(thread, &WorkerThread::writeNetworkConfiguration, networkAdapterDescription, dhcpEnabled, ipv4Address, subnetMask,
                          gateway, preferredDns, alternateDns, password))
        return;

    QString adapterIndex;
    for (const auto &interface : QNetworkInterface::allInterfaces()) {
        if (interface.humanReadableName() == networkAdapterDescription) {
            adapterIndex = QString::number(interface.index());
            break;
        }
    }
#ifdef Q_OS_WIN
    Q_UNUSED(password);
    QProcess writeInfoProcess;
    writeInfoProcess.setReadChannel(QProcess::StandardOutput);
    QString query;
    query.append(" -Verb RunAs");
    query.append(" -FilePath PowerShell");
    query.append(" -WindowStyle hidden");
    QString argumentList;
    argumentList.append(QString(" $WMI = Get-WmiObject -Class Win32_NetworkAdapterConfiguration -Filter {InterfaceIndex LIKE '%1'};").arg(adapterIndex));
    argumentList.append(dhcpEnabled ? " $WMI.EnableDHCP();" : QString(" $WMI.EnableStatic(\"{%1}\", \"{%2}\");").arg(ipv4Address).arg(subnetMask));
    argumentList.append(QString("$WMI.SetGateways(%1);").arg(gateway.isEmpty() || dhcpEnabled  ? "" : QString("\"{%1}\"").arg(gateway)));
    QString dnsServers;
    if (!dhcpEnabled) {
        for (const auto &dnsServer : {preferredDns, alternateDns}) {
            if (dnsServer.isEmpty())
                continue;
            if (!dnsServers.isEmpty())
                dnsServers.append(",");
            dnsServers.append(QString("\"{%1}\"").arg(dnsServer));
        }
    }
    if (!dnsServers.isEmpty())
        argumentList.append(QString(" $dnsServers = %1;").arg(dnsServers));
    argumentList.append(QString(" $WMI.SetDNSServerSearchOrder(%1);").arg(dnsServers.isEmpty() ? "" : "$dnsServers"));

    query.append(" -ArgumentList { " + argumentList + " }");

    writeInfoProcess.start("PowerShell", {"Start-Process", query});
    writeInfoProcess.waitForStarted();
    if (writeInfoProcess.error() != QProcess::UnknownError) {
        qCDebug(proofUtilsNetworkConfigurationLog) << "PowerShell can't be started:" << writeInfoProcess.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);

        return;
    }
    writeInfoProcess.waitForFinished();
    QThread::msleep(1000);
#else

    QProcess networkingProcess;
    networkingProcess.setProcessChannelMode(QProcess::MergedChannels);
    networkingProcess.start("sudo -S -k /sbin/ifdown " + networkAdapterDescription);
    networkingProcess.waitForStarted();
    if (networkingProcess.error() != QProcess::UnknownError) {
        qCDebug(proofUtilsNetworkConfigurationLog) << "service networking can't be stopped:" << networkingProcess.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }

    if (!enterPassword(networkingProcess, password)) {
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }

    QFile settingsFile(NETWORK_SETTINGS_FILE);
    if (!settingsFile.open(QIODevice::ReadOnly)) {
        qCDebug(proofUtilsNetworkConfigurationLog) << NETWORK_SETTINGS_FILE <<  "can't be openned:" << settingsFile.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }

    QByteArray newInterfaces;
    bool hideIpsForCurrentInterface = false;
    bool interfaceUpdated = false;
    while (!settingsFile.atEnd()) {
        QString line = settingsFile.readLine();
        bool currentInterfaceFound = line.trimmed().startsWith("iface " + networkAdapterDescription);

        if (settingsFile.atEnd() && !currentInterfaceFound && !interfaceUpdated) {
            newInterfaces.append(line);
            line = QString("iface %1 inet %2\n").arg(networkAdapterDescription).arg(dhcpEnabled ? DYNAMIC_IP : STATIC_IP);
            currentInterfaceFound = true;
        }

        if (currentInterfaceFound) {
            hideIpsForCurrentInterface = true;
            if (line.contains(STATIC_IP) == dhcpEnabled)
                line.replace(dhcpEnabled ? STATIC_IP : DYNAMIC_IP, dhcpEnabled ? DYNAMIC_IP : STATIC_IP);
            if (!dhcpEnabled) {
                if (!ipv4Address.isEmpty())
                    line.append(ADDRESS + " " + ipv4Address + "\n");
                if (!subnetMask.isEmpty())
                    line.append(NETMASK + " " + subnetMask + "\n");
                if (!gateway.isEmpty())
                    line.append(GATEWAY + " " + gateway + "\n");
                if (!preferredDns.isEmpty() || !alternateDns.isEmpty())
                    line.append(DNS_NAMESERVERS + " " + QStringList({preferredDns, alternateDns}).join(" ") + "\n");
            }
            newInterfaces.append(line);
            interfaceUpdated = true;
        } else if (line.startsWith("iface") || line.startsWith("auto") || line == "\n") {
            hideIpsForCurrentInterface = false;
        }

        if (!hideIpsForCurrentInterface)
            newInterfaces.append(line);
    }
    settingsFile.close();

    QFile settingsFileTmp(NETWORK_SETTINGS_FILE_TMP);
    if (!settingsFileTmp.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qCWarning(proofUtilsNetworkConfigurationLog) << NETWORK_SETTINGS_FILE <<  "can't be openned:" << settingsFileTmp.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }
    settingsFileTmp.write(newInterfaces);
    settingsFileTmp.close();


    networkingProcess.start("sudo -S -k /bin/cp \"" + NETWORK_SETTINGS_FILE_TMP + "\" \"" + NETWORK_SETTINGS_FILE + "\"");
    networkingProcess.waitForStarted();
    if (networkingProcess.error() != QProcess::UnknownError) {
        qCDebug(proofUtilsNetworkConfigurationLog) << NETWORK_SETTINGS_FILE + " can't be rewritten:" << networkingProcess.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }

    if (!enterPassword(networkingProcess, password)) {
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        settingsFileTmp.remove();
        return;
    }
    settingsFileTmp.remove();

    networkingProcess.start("sudo -S -k /sbin/ifup " + networkAdapterDescription);
    networkingProcess.waitForStarted();
    if (networkingProcess.error() != QProcess::UnknownError) {
        qCDebug(proofUtilsNetworkConfigurationLog) << "service networking can't be started:" << networkingProcess.errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }

    if (!enterPassword(networkingProcess, password)) {
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
        return;
    }
#endif

    NetworkConfiguration networkConfiguration = fetchNetworkConfigurationPrivate(networkAdapterDescription);
    if (networkConfiguration.index == adapterIndex
            && networkConfiguration.dhcpEnabled == dhcpEnabled
            && (dhcpEnabled || (networkConfiguration.ipv4Address == ipv4Address && networkConfiguration.subnetMask == subnetMask && networkConfiguration.gateway == gateway))
            && (networkConfiguration.preferredDns == preferredDns || networkConfiguration.preferredDns == alternateDns)
            && (networkConfiguration.alternateDns == alternateDns || networkConfiguration.alternateDns == preferredDns)) {
        emit q->networkConfigurationWrote();
    } else {
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NetworkConfigurationCannotBeWritten, "Can't write network configuration", true);
    }
}

} // namespace Proof

WorkerThread::WorkerThread(Proof::NetworkConfigurationManagerPrivate *networkConfigurationManager)
    : QThread(), networkConfigurationManager(networkConfigurationManager)
{
    moveToThread(this);
}

QStringList WorkerThread::networkInterfaces()
{
    return networkConfigurationManager->networkInterfaces();
}

void WorkerThread::checkPassword(const QString &password)
{
    networkConfigurationManager->checkPassword(password);
}

void WorkerThread::fetchNetworkConfiguration(const QString &networkAdapterDescription)
{
    networkConfigurationManager->fetchNetworkConfiguration(networkAdapterDescription);
}

void WorkerThread::writeNetworkConfiguration(const QString &networkAdapterDescription, bool dhcpEnabled, const QString &ipv4Address, const QString &subnetMask,
                                             const QString &gateway, const QString &preferredDns, const QString &alternateDns, const QString &password)
{
    networkConfigurationManager->writeNetworkConfiguration(networkAdapterDescription, dhcpEnabled, ipv4Address, subnetMask,
                                                           gateway, preferredDns, alternateDns, password);
}

#include "networkconfigurationmanager.moc"
