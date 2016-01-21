#include "updatemanager.h"
#include "proofcore/proofobject_p.h"

#include <QProcess>
#include <QTimer>
#include <QSet>
#include <QRegExp>

namespace {
class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(Proof::UpdateManagerPrivate *updater);

    template<class Method, class... Args>
    void callUpdater(Method method, Args... args);

    template<class Method, class... Args>
    auto callUpdaterWithResult(Method method, Args &&... args)
        -> decltype((std::declval<Proof::UpdateManagerPrivate&>().*method)(std::forward<Args>(args)...));

private:
    template<class Result, class Method, class... Args>
    typename std::enable_if<!std::is_same<Result, void>::value, Result>::type
    doTheCall(Method method, Args &&... args);

    template<class Result, class Method, class... Args>
    typename std::enable_if<std::is_same<Result, void>::value>::type
    doTheCall(Method method, Args &&... args);

private:
    Proof::UpdateManagerPrivate *updater;
};
}

namespace Proof {
class UpdateManagerPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(UpdateManager)

    void checkPassword(const QString &password);
    void checkForUpdates();
    void installVersion(const QString &version, const QString &password);
    void fetchRollbackVersions();

    bool enabled() const;
    int timeout() const;
    QString currentVersion() const;
    QString packageName() const;
    QStringList rollbackVersions() const;

    void setEnabled(bool arg);
    void setTimeout(int arg);
    void setCurrentVersion(const QString &arg);
    void setPackageName(const QString &arg);
    void setRollbackVersions(const QStringList &arg);

    void updateTimerState();

    static quint64 versionFromString(const QStringList &version);
    static QString versionToString(quint64 version);

    QString packageNameValue;
    quint64 currentVersionValue = 0x0;
    int currentVersionMajor = 0;
    bool enabledValue = false;
    QStringList rollbackVersionsValue;
    WorkerThread *thread = nullptr;
    QTimer *timer = nullptr;
};

} // namespace Proof

using namespace Proof;

UpdateManager::UpdateManager(QObject *parent)
    : ProofObject(*new UpdateManagerPrivate, parent)
{
    Q_D(UpdateManager);
    d->thread = new WorkerThread(d);
    d->timer = new QTimer();
    d->timer->moveToThread(d->thread);
    d->timer->setTimerType(Qt::VeryCoarseTimer);
    d->timer->setInterval(4 * 60 * 60 * 1000); // 4 hours
    connect(d->timer, &QTimer::timeout, d->timer, [d] {d->checkForUpdates();});
    d->thread->start();
}

UpdateManager::~UpdateManager()
{
    Q_D(UpdateManager);
    d->thread->quit();
    d->thread->wait(1000);
    delete d->thread;
    delete d->timer;
}

void UpdateManager::update(const QString &password)
{
    Q_D(UpdateManager);
    d->thread->callUpdater(&UpdateManagerPrivate::installVersion, QString(), password);
}

void UpdateManager::installVersion(const QString &version, const QString &password)
{
    Q_D(UpdateManager);
    d->thread->callUpdater(&UpdateManagerPrivate::installVersion, version, password);
}

void UpdateManager::checkPassword(const QString &password)
{
    Q_D(UpdateManager);
    d->thread->callUpdater(&UpdateManagerPrivate::checkPassword, password);
}

bool UpdateManager::enabled() const
{
    Q_D(const UpdateManager);
    return d->thread->callUpdaterWithResult(&UpdateManagerPrivate::enabled);
}

int UpdateManager::timeout() const
{
    Q_D(const UpdateManager);
    return d->thread->callUpdaterWithResult(&UpdateManagerPrivate::timeout);
}

QString UpdateManager::currentVersion() const
{
    Q_D(const UpdateManager);
    return d->thread->callUpdaterWithResult(&UpdateManagerPrivate::currentVersion);
}

QString UpdateManager::packageName() const
{
    Q_D(const UpdateManager);
    return d->thread->callUpdaterWithResult(&UpdateManagerPrivate::packageName);
}

QStringList UpdateManager::rollbackVersions() const
{
    Q_D(const UpdateManager);
    return d->thread->callUpdaterWithResult(&UpdateManagerPrivate::rollbackVersions);
}

void UpdateManager::setEnabled(bool arg)
{
    Q_D(UpdateManager);
    d->thread->callUpdaterWithResult(&UpdateManagerPrivate::setEnabled, arg);
}

void UpdateManager::setTimeout(int arg)
{
    Q_D(UpdateManager);
    d->thread->callUpdaterWithResult(&UpdateManagerPrivate::setTimeout, arg);
}

void UpdateManager::setCurrentVersion(const QString &arg)
{
    Q_D(UpdateManager);
    d->thread->callUpdaterWithResult(&UpdateManagerPrivate::setCurrentVersion, arg);
}

void UpdateManager::setPackageName(const QString &arg)
{
    Q_D(UpdateManager);
    d->thread->callUpdaterWithResult(&UpdateManagerPrivate::setPackageName, arg);
}

void UpdateManagerPrivate::checkPassword(const QString &password)
{
#ifdef Q_OS_LINUX
    Q_Q(UpdateManager);
    QScopedPointer<QProcess> checker(new QProcess);
    checker->setProcessChannelMode(QProcess::MergedChannels);
    checker->start(QString("sudo -S -k pwd"));
    if (checker->error() == QProcess::UnknownError) {
        if (!checker->waitForReadyRead()) {
            qCDebug(proofUtilsUpdatesLog) << "No answer from command. Returning";
            emit q->passwordChecked(false);
            return;
        }
        QByteArray readBuffer;
        QByteArray currentRead;

        currentRead = checker->readAll();
        readBuffer.append(currentRead);
        currentRead = currentRead.trimmed();
        if (currentRead.contains("[sudo]") || currentRead.contains("password for")) {
            checker->write(QString("%1\n").arg(password).toLatin1());
            if (!checker->waitForReadyRead()) {
                qCDebug(proofUtilsUpdatesLog) << "No answer from command. Returning";
                emit q->passwordChecked(false);
                return;
            }

            currentRead = checker->readAll();
            readBuffer.append(currentRead);
            currentRead = currentRead.trimmed();

            if (currentRead.contains("is not in the sudoers")) {
                qCDebug(proofUtilsUpdatesLog) << "User not in sudoers list; log:\n" << readBuffer;
                emit q->passwordChecked(false);
                return;
            }
            if (currentRead.contains("Sorry, try again")) {
                qCDebug(proofUtilsUpdatesLog) << "Sudo rejected the password; log:\n" << readBuffer;
                emit q->passwordChecked(false);
                return;
            }
        }
        checker->waitForFinished(-1);
        qCDebug(proofUtilsUpdatesLog) << "Exitcode =" << checker->exitCode();
        emit q->passwordChecked(checker->exitCode() == 0);
    } else {
        qCDebug(proofUtilsUpdatesLog) << "Process couldn't be started" << checker->error() << checker->errorString();
    }
#else
    Q_UNUSED(password);
    qCDebug(proofUtilsUpdatesLog) << "Password check is not supported for this platform";
    emit q->updateFailed();
#endif
}

void UpdateManagerPrivate::checkForUpdates()
{
#ifdef Q_OS_LINUX
    Q_Q(UpdateManager);
    QScopedPointer<QProcess> checker(new QProcess);
    checker->start(QString("apt-cache --no-all-versions show %1").arg(packageNameValue));
    checker->waitForStarted();
    if (checker->error() == QProcess::UnknownError) {
        checker->waitForFinished();
        QList<QByteArray> lines = checker->readAll().trimmed().split('\n');
        QString version;
        for (const QByteArray &line : lines) {
            if (line.startsWith("Version: "))
                version = line;
            version.remove("Version: ");
        }
        QStringList splittedVersion = version.split(".");
        if (splittedVersion.count() < 4) {
            qCDebug(proofUtilsUpdatesLog) << "Strange version found" << version << ". Returning.";
            return;
        }
        int foundVersionMajor = splittedVersion[0].toInt();
        quint64 foundVersion = versionFromString(splittedVersion);
        qCDebug(proofUtilsUpdatesLog) << "Version found:" << QString("0x%1").arg(foundVersion, 16, 16, QLatin1Char('0'));
        if (foundVersionMajor > currentVersionMajor) {
            qCDebug(proofUtilsUpdatesLog) << "Manual update needed because of different major version";
            emit q->manualUpdateNeeded(version);
        } else if (foundVersion > currentVersionValue) {
            qCDebug(proofUtilsUpdatesLog) << "Update from app is possible";
            emit q->newVersionFound(version);
        }
    } else {
        qCDebug(proofUtilsUpdatesLog) << "process couldn't be started" << checker->error() << checker->errorString();
    }
    fetchRollbackVersions();
#endif
}

void UpdateManagerPrivate::installVersion(const QString &version, const QString &password)
{
    Q_Q(UpdateManager);
#ifdef Q_OS_LINUX
    QScopedPointer<QProcess> updater(new QProcess);
    updater->setProcessChannelMode(QProcess::MergedChannels);
    bool isUpdate = version.isEmpty();
    QString package = isUpdate ? packageNameValue : QString("%1=%2").arg(packageNameValue, version);
    auto successSignal = isUpdate ? &UpdateManager::updateSucceeded : &UpdateManager::installationSucceeded;
    auto failSignal = isUpdate ? &UpdateManager::updateFailed : &UpdateManager::installationFailed;
    updater->start(QString("sudo -S -k apt-get --quiet --assume-yes --force-yes --allow-unauthenticated install %1").arg(package));
    updater->waitForStarted();
    if (updater->error() == QProcess::UnknownError) {
        if (!updater->waitForReadyRead()) {
            qCDebug(proofUtilsUpdatesLog) << "No answer from apt-get. Returning";
            emit (q->*failSignal)();
            return;
        }
        QByteArray readBuffer;
        QByteArray currentRead;

        currentRead = updater->readAll();
        readBuffer.append(currentRead);
        currentRead = currentRead.trimmed();
        if (currentRead.contains("[sudo]") || currentRead.contains("password for")) {
            updater->write(QString("%1\n").arg(password).toLatin1());
            if (!updater->waitForReadyRead()) {
                qCDebug(proofUtilsUpdatesLog) << "No answer from apt-get. Returning";
                emit (q->*failSignal)();
                return;
            }

            currentRead = updater->readAll();
            readBuffer.append(currentRead);
            currentRead = currentRead.trimmed();

            if (currentRead.contains("is not in the sudoers")) {
                qCDebug(proofUtilsUpdatesLog) << "User not in sudoers list; log:\n" << readBuffer;
                emit (q->*failSignal)();
                return;
            }
            if (currentRead.contains("Sorry, try again")) {
                qCDebug(proofUtilsUpdatesLog) << "Sudo rejected the password; log:\n" << readBuffer;
                emit (q->*failSignal)();
                return;
            }
        }
        updater->waitForFinished(-1);
        qCDebug(proofUtilsUpdatesLog) << "Updated with exitcode =" << updater->exitCode() << "; log:\n" << readBuffer + updater->readAll().trimmed();
        if (updater->exitCode())
            emit (q->*failSignal)();
        else
            emit (q->*successSignal)();
    } else {
        qCDebug(proofUtilsUpdatesLog) << "process couldn't be started" << updater->error() << updater->errorString();
    }
#else
    Q_UNUSED(version);
    Q_UNUSED(password);
    qCDebug(proofUtilsUpdatesLog) << "Update is not supported for this platform";
    emit q->updateFailed();
#endif
}

void UpdateManagerPrivate::fetchRollbackVersions()
{
#ifdef Q_OS_LINUX
    if (packageNameValue.isEmpty()) {
        setRollbackVersions(QStringList{});
        return;
    }

    QScopedPointer<QProcess> checker(new QProcess);
    checker->start(QString("apt-cache showpkg %1").arg(packageNameValue));
    checker->waitForStarted();
    if (checker->error() == QProcess::UnknownError) {
        if (checker->waitForFinished()) {
            if (checker->exitStatus() == QProcess::NormalExit && checker->exitCode() == 0) {
                QString output = checker->readAll().trimmed();
                QRegExp versionRegExp("\\n(\\d?\\d\\.\\d?\\d\\.\\d?\\d\\.\\d?\\d)");
                QSet<QString> versions;
                int position = 0;
                while ((position = versionRegExp.indexIn(output, position)) != -1) {
                    QString version = versionRegExp.cap(1);
                    if (currentVersionValue > versionFromString(version.split('.')))
                        versions << version;
                    position += versionRegExp.matchedLength();
                }
                QStringList result = versions.values();
                std::sort(result.begin(), result.end());
                setRollbackVersions(result);
            } else {
                qCDebug(proofUtilsUpdatesLog) << "Process failed" << checker->error() << checker->errorString();
                setRollbackVersions(QStringList{});
            }
        } else {
            qCDebug(proofUtilsUpdatesLog) << "Process timed out";
            setRollbackVersions(QStringList{});
        }
    } else {
        qCDebug(proofUtilsUpdatesLog) << "Process couldn't be started" << checker->error() << checker->errorString();
        setRollbackVersions(QStringList{});
    }
#else
    setRollbackVersions(QStringList{});
#endif
}

bool UpdateManagerPrivate::enabled() const
{
    return enabledValue;
}

int UpdateManagerPrivate::timeout() const
{
    return timer->interval();
}

QString UpdateManagerPrivate::currentVersion() const
{
    return versionToString(currentVersionValue);
}

QString UpdateManagerPrivate::packageName() const
{
    return packageNameValue;
}

QStringList UpdateManagerPrivate::rollbackVersions() const
{
    return rollbackVersionsValue;
}

void UpdateManagerPrivate::setEnabled(bool arg)
{
    Q_Q(UpdateManager);
    if (enabledValue != arg) {
        enabledValue = arg;
        emit q->enabledChanged(enabledValue);
        updateTimerState();
    }
}

void UpdateManagerPrivate::setTimeout(int arg)
{
    Q_Q(UpdateManager);
    if (timer->interval() != arg) {
        timer->setInterval(arg);
        emit q->timeoutChanged(timer->interval());
    }
}

void UpdateManagerPrivate::setCurrentVersion(const QString &arg)
{
    Q_Q(UpdateManager);
    QStringList splittedVersion = arg.split(".");
    if (splittedVersion.count() < 4)
        return;
    quint64 version = versionFromString(splittedVersion);
    if (currentVersionValue != version) {
        currentVersionMajor = splittedVersion[0].toInt();
        currentVersionValue = version;
        qCDebug(proofUtilsUpdatesLog) << "Current version:" << QString("0x%1").arg(currentVersionValue, 16, 16, QLatin1Char('0'));
        emit q->currentVersionChanged(currentVersion());
        updateTimerState();
    }
}

void UpdateManagerPrivate::setPackageName(const QString &arg)
{
    Q_Q(UpdateManager);
    if (packageNameValue != arg) {
        packageNameValue = arg;
        emit q->packageNameChanged(packageNameValue);
        updateTimerState();
    }
}

void UpdateManagerPrivate::setRollbackVersions(const QStringList &arg)
{
    Q_Q(UpdateManager);
    if (rollbackVersionsValue != arg) {
        rollbackVersionsValue = arg;
        emit q->rollbackVersionsChanged(rollbackVersionsValue);
    }
}

void UpdateManagerPrivate::updateTimerState()
{
    if (currentVersionValue != 0 && !packageNameValue.isEmpty() && enabledValue) {
        if (!timer->isActive()) {
            checkForUpdates();
            timer->start();
        }
    } else {
        timer->stop();
    }
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

QString UpdateManagerPrivate::versionToString(quint64 version)
{
    return QString("%1.%2.%3.%4")
            .arg(version >> 48)
            .arg((version >> 32) & 0xFFFF)
            .arg((version >> 16) & 0xFFFF)
            .arg(version & 0xFFFF);
}



WorkerThread::WorkerThread(UpdateManagerPrivate *updater)
    : updater(updater)
{
    moveToThread(this);
}

template<class Method, class... Args>
void WorkerThread::callUpdater(Method method, Args... args)
{
    if (!ProofObject::call(this, &WorkerThread::callUpdater<Method, Args...>, method, args...))
        (updater->*method)(args...);
}

template<class Method, class... Args>
auto WorkerThread::callUpdaterWithResult(Method method, Args &&... args)
    -> decltype((std::declval<Proof::UpdateManagerPrivate&>().*method)(std::forward<Args>(args)...))
{
    using Result = decltype((std::declval<Proof::UpdateManagerPrivate&>().*method)(std::forward<Args>(args)...));
    return doTheCall<Result>(method, std::forward<Args>(args)...);
}

template<class Result, class Method, class... Args>
typename std::enable_if<!std::is_same<Result, void>::value, Result>::type
WorkerThread::doTheCall(Method method, Args &&... args)
{
    Result result;
    if (!ProofObject::call(this, &WorkerThread::doTheCall<Result, Method, Args &&...>,
                           Call::Block, result, method, std::forward<Args>(args)...)) {
        result = (updater->*method)(std::forward<Args>(args)...);
    }
    return result;
}

template<class Result, class Method, class... Args>
typename std::enable_if<std::is_same<Result, void>::value>::type
WorkerThread::doTheCall(Method method, Args &&... args)
{
    if (!ProofObject::call(this, &WorkerThread::doTheCall<Result, Method, Args &&...>,
                           Call::Block, method, std::forward<Args>(args)...)) {
        (updater->*method)(std::forward<Args>(args)...);
    }
}

#include "updatemanager.moc"
