#include "powermanager.h"

#include "proofcore/proofobject_p.h"

#include <QProcess>

#ifdef Q_OS_ANDROID
# include <QtAndroid>
#endif

namespace {

class WorkerThread : public QThread // clazy:exclude=ctor-missing-parent-argument
{
    Q_OBJECT
public:
    explicit WorkerThread(Proof::PowerManagerPrivate *powerManager);
    void shutdown(const QString &password, bool restart);

private:
    Proof::PowerManagerPrivate *powerManager;
};

}

namespace Proof {

class PowerManagerPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(PowerManager)
public:
    void shutdown(const QString &password, bool restart);
    void restartApp();

    WorkerThread *thread;
};

PowerManager::PowerManager(QObject *parent)
    : ProofObject(*new PowerManagerPrivate, parent)
{
    Q_D(PowerManager);
    d->thread = new WorkerThread(d);
    d->thread->start();
}

PowerManager::~PowerManager()
{
    Q_D(PowerManager);
    d->thread->quit();
    d->thread->wait(1000);
    delete d->thread;
}

void PowerManager::restart(const QString &password)
{
    Q_D(PowerManager);
    d->shutdown(password, true);
}

void PowerManager::powerOff(const QString &password)
{
    Q_D(PowerManager);
    d->shutdown(password, false);
}

void PowerManager::restartApp()
{
    Q_D(PowerManager);
    d->restartApp();
}

void PowerManagerPrivate::shutdown(const QString &password, bool restart)
{
    if (ProofObject::call(thread, &WorkerThread::shutdown, password, restart))
        return;

#ifdef Q_OS_UNIX
    Q_Q(PowerManager);
# ifdef Q_OS_ANDROID
    qCDebug(proofUtilsMiscLog) << "Shutdown is not supported for Android.";
    emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::PlatformNotSupported, QObject::tr("This action is not supported for current platform"), true);
# else
    QScopedPointer<QProcess> shutdownProcess(new QProcess);
    shutdownProcess->setProcessChannelMode(QProcess::MergedChannels);
    shutdownProcess->start(QStringLiteral("sudo -S -k shutdown -%1 now")
                           .arg(restart ? QStringLiteral("r") : QStringLiteral("h")));
    shutdownProcess->waitForStarted();
    if (shutdownProcess->error() == QProcess::UnknownError) {
        if (!shutdownProcess->waitForReadyRead()) {
            qCDebug(proofUtilsMiscLog) << "No answer from sudo. Returning";
            emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NoAnswerFromSystem, QStringLiteral("No answer from OS"), false);
            return;
        }
        QByteArray readBuffer;
        QByteArray currentRead;

        currentRead = shutdownProcess->readAll();
        readBuffer.append(currentRead);
        currentRead = currentRead.trimmed();
        if (currentRead.contains("[sudo]") || currentRead.contains("password for")) {
            shutdownProcess->write(QStringLiteral("%1\n").arg(password).toLatin1());
            if (!shutdownProcess->waitForReadyRead()) {
                qCDebug(proofUtilsMiscLog) << "No answer from sudo. Returning";
                emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::NoAnswerFromSystem, QStringLiteral("No answer from OS"), false);
                return;
            }

            currentRead = shutdownProcess->readAll();
            readBuffer.append(currentRead);
            currentRead = currentRead.trimmed();

            if (currentRead.contains("is not in the sudoers")) {
                qCDebug(proofUtilsMiscLog) << "User not in sudoers list; log:\n" << readBuffer;
                emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::UserNotASudoer, QStringLiteral("User not in sudoers list"), false);
                return;
            }
            if (currentRead.contains("Sorry, try again")) {
                qCDebug(proofUtilsMiscLog) << "Sudo rejected the password; log:\n" << readBuffer;
                emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::IncorrectPassword, QObject::tr("Incorrect password"), true);
                return;
            }

            if (!currentRead.contains("The system is going down for")) {
                qCDebug(proofUtilsMiscLog) << "Sudo doesn't do nothing; log:\n" << readBuffer;
                emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::IncorrectPassword, QObject::tr("Something went wrong during shutdown. Please, contact IT department."), true);
                return;
            }
        }

        shutdownProcess->waitForFinished(-1);
        qCDebug(proofUtilsMiscLog) << "Shutdown exitcode =" << shutdownProcess->exitCode() << "; log:\n" << readBuffer + shutdownProcess->readAll().trimmed();
        if (shutdownProcess->exitCode())
            emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::UnknownError, QStringLiteral("Unknown error"), false);
    } else {
        qCDebug(proofUtilsMiscLog) << "Process couldn't be started" << shutdownProcess->error() << shutdownProcess->errorString();
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::UnknownError, QStringLiteral("Unknown error"), false);
    }
# endif
#else
    if (restart)
        restartApp();
    else
        qApp->quit();
#endif
}

void PowerManagerPrivate::restartApp()
{
#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();
    activity.callMethod<void>("restartApp", "()V");
#else
    Q_Q(PowerManager);
    if (QProcess::startDetached(qApp->applicationFilePath())) {
        qApp->quit();
    } else {
        qCDebug(proofUtilsMiscLog) << "Can't restart application";
        emit q->errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::UnknownError, QStringLiteral("Can't restart application"), false);
    }
#endif
}

} // namespace Proof

WorkerThread::WorkerThread(Proof::PowerManagerPrivate *powerManager)
    : QThread(), powerManager(powerManager)
{
    moveToThread(this);
}

void WorkerThread::shutdown(const QString &password, bool restart)
{
    powerManager->shutdown(password, restart);
}

#include "powermanager.moc"
