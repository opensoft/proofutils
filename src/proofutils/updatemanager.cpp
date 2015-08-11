#include "updatemanager.h"
#include "proofcore/proofobject_p.h"

#include <QProcess>

namespace Proof {
class UpdateManagerPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(UpdateManager)

    void setCurrentVersion(const QString &version);
    quint64 versionFromString(const QStringList &version);

    QString packageName;
    quint64 currentVersion = 0x0;
    int currentVersionMajor = 0;
};

} // namespace Proof

using namespace Proof;

UpdateManager::UpdateManager(const QString &packageName, const QString &currentVersion, QObject *parent)
    : ProofObject(*new UpdateManagerPrivate, parent)
{
    Q_D(UpdateManager);
    d->packageName = packageName;
    d->setCurrentVersion(currentVersion);
}

void UpdateManager::checkForUpdates()
{
    Q_D(UpdateManager);
    if (call(this, &UpdateManager::checkForUpdates))
        return;
#ifdef Q_OS_LINUX
    QScopedPointer<QProcess> checker(new QProcess);
    checker->start(QString("apt-cache --no-all-versions show %1 | grep ^Version:\\s").arg(d->packageName));
    checker->waitForStarted();
    if (checker->error() == QProcess::UnknownError) {
        checker->waitForReadyRead();
        checker->waitForFinished();
        QString version = checker->readAll().trimmed();
        version.remove("Version: ");
        QStringList splittedVersion = version.split(".");
        if (splittedVersion.count() < 4)
            return;
        int foundVersionMajor = splittedVersion[0].toInt();
        quint64 foundVersion = d->versionFromString(splittedVersion);
        qCDebug(proofUtilsUpdatesLog) << "Version found:" << QString("0x%1").arg(foundVersion, 0, 16);
        if (foundVersionMajor > d->currentVersionMajor) {
            qCDebug(proofUtilsUpdatesLog) << "Manual update needed because of different major version";
            emit manualUpdateNeeded();
        } else if (foundVersion > d->currentVersion) {
            qCDebug(proofUtilsUpdatesLog) << "Update from app is possible";
            emit newVersionFound(version);
        }
    }
#endif
}

void UpdateManager::update(const QString &password)
{
    Q_D(UpdateManager);
    if (call(this, &UpdateManager::update, password))
        return;
#ifdef Q_OS_LINUX
        QScopedPointer<QProcess> updater(new QProcess);
        updater->start(QString("sudo -S -k apt-get -q -y install %1").arg(d->packageName));
        updater->waitForStarted();
        if (updater->error() == QProcess::UnknownError) {
            updater->waitForReadyRead();
            updater->write(QString("%1").arg(password).toLatin1());
            updater->waitForFinished();
            qCDebug(proofUtilsUpdatesLog) << "Updated with exitcode =" << updater->exitCode() << "; log:\n" << updater->readAll().trimmed();
            if (!updater->exitCode())
                emit updateFailed();
            else
                emit updateSucceeded();
        }
#else
    qCDebug(proofUtilsUpdatesLog) << "Update is not supported for this platform";
    emit updateFailed();
#endif
}

void UpdateManagerPrivate::setCurrentVersion(const QString &version)
{
    QStringList splittedVersion = version.split(".");
    if (splittedVersion.count() < 4)
        return;
    currentVersionMajor = splittedVersion[0].toInt();
    currentVersion = versionFromString(splittedVersion);
    qCDebug(proofUtilsUpdatesLog) << "Current version:" << QString("0x%1").arg(currentVersion, 0, 16);
}

quint64 UpdateManagerPrivate::versionFromString(const QStringList &version)
{
    if (version.count() < 4)
        return 0x0;
    return ((quint64)version[0].toShort() << 48)
            | ((quint64)version[1].toShort() << 32)
            | ((quint64)version[2].toShort() << 16)
            | ((quint64)version[3].toShort());
}

