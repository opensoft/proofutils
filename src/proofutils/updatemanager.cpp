#include "updatemanager.h"
#include "proofcore/proofobject_p.h"

#include <QProcess>
#include <QTimer>

namespace {
class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(Proof::UpdateManagerPrivate *updater);

    template<class Method, class... Args>
    void callUpdater(Method method, Args &&... args);

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

    void checkForUpdates();
    void update(const QString &password);

    bool enabled() const;
    int timeout() const;
    QString currentVersion() const;
    QString packageName() const;

    void setEnabled(bool arg);
    void setTimeout(int arg);
    void setCurrentVersion(const QString &arg);
    void setPackageName(const QString &arg);

    void updateTimerState();

    static quint64 versionFromString(const QStringList &version);
    static QString versionToString(quint64 version);

    QString packageNameValue;
    quint64 currentVersionValue = 0x0;
    int currentVersionMajor = 0;
    bool enabledValue = false;
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
    d->thread->callUpdater(&UpdateManagerPrivate::update, password);
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
#endif
}

void UpdateManagerPrivate::update(const QString &password)
{
    Q_Q(UpdateManager);
#ifdef Q_OS_LINUX
    QScopedPointer<QProcess> updater(new QProcess);
    updater->start(QString("sudo -S -k apt-get --quiet --assume-yes --allow-unauthenticated install %1").arg(packageNameValue));
    updater->waitForStarted();
    if (updater->error() == QProcess::UnknownError) {
        updater->write(QString("%1\n").arg(password).toLatin1());
        updater->waitForFinished();
        qCDebug(proofUtilsUpdatesLog) << "Updated with exitcode =" << updater->exitCode() << "; log:\n" << updater->readAll().trimmed();
        if (updater->exitCode())
            emit q->updateFailed();
        else
            emit q->updateSucceeded();
    } else {
        qCDebug(proofUtilsUpdatesLog) << "process couldn't be started" << updater->error() << updater->errorString();
    }
#else
    Q_UNUSED(password);
    qCDebug(proofUtilsUpdatesLog) << "Update is not supported for this platform";
    emit q->updateFailed();
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

void UpdateManagerPrivate::updateTimerState()
{
    if (currentVersionValue != 0 && !packageNameValue.isEmpty() && enabledValue) {
        checkForUpdates();
        timer->start();
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
void WorkerThread::callUpdater(Method method, Args &&... args)
{
    if (!ProofObject::call(this, &WorkerThread::callUpdater<Method, Args &&...>, method, std::forward<Args>(args)...))
        (updater->*method)(std::forward<Args>(args)...);
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
                           Call::Block, result, method, std::forward<Args>(args)...))
        result = (updater->*method)(std::forward<Args>(args)...);
    return result;
}

template<class Result, class Method, class... Args>
typename std::enable_if<std::is_same<Result, void>::value>::type
WorkerThread::doTheCall(Method method, Args &&... args)
{
    if (!ProofObject::call(this, &WorkerThread::doTheCall<Result, Method, Args &&...>,
                           Call::Block, method, std::forward<Args>(args)...))
        (updater->*method)(std::forward<Args>(args)...);
}

#include "updatemanager.moc"
