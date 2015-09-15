#include "powermanager.h"

#include "proofcore/proofobject_p.h"

#include <QProcess>

namespace {

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkerThread(Proof::PowerManagerPrivate *powerManager);
    void shutdowd(const QString &password, bool restart);

private:
    Proof::PowerManagerPrivate *powerManager;
};

}

namespace Proof {

class PowerManagerPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(PowerManager)
public:
    void shutdowd(const QString &password, bool restart);

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
    d->shutdowd(password, true);
}

void PowerManager::powerOff(const QString &password)
{
    Q_D(PowerManager);
    d->shutdowd(password, false);
}

void PowerManagerPrivate::shutdowd(const QString &password, bool restart)
{
    Q_Q(PowerManager);
    if (ProofObject::call(thread, &WorkerThread::shutdowd, password, restart))
        return;
#ifdef Q_OS_UNIX
    QScopedPointer<QProcess> shutdownProcess(new QProcess);
    shutdownProcess->setProcessChannelMode(QProcess::MergedChannels);
    shutdownProcess->start(QString("sudo -S -k shutdown -%1 now").arg(restart ? "r" : "h"));
    shutdownProcess->waitForStarted();
    if (shutdownProcess->error() == QProcess::UnknownError) {
        if (!shutdownProcess->waitForReadyRead()) {
            qCDebug(proofUtilsUpdatesLog) << "No answer from sudo. Returning";
            emit q->errorOccurred(QObject::tr("No answer from OS"));
            return;
        }
        QByteArray readBuffer;
        QByteArray currentRead;

        currentRead = shutdownProcess->readAll();
        readBuffer.append(currentRead);
        currentRead = currentRead.trimmed();
        if (currentRead.contains("[sudo]") || currentRead.contains("password for")) {
            shutdownProcess->write(QString("%1\n").arg(password).toLatin1());
            if (!shutdownProcess->waitForReadyRead()) {
                qCDebug(proofUtilsUpdatesLog) << "No answer from sudo. Returning";
                emit q->errorOccurred(QObject::tr("No answer from OS"));
                return;
            }

            currentRead = shutdownProcess->readAll();
            readBuffer.append(currentRead);
            currentRead = currentRead.trimmed();

            if (currentRead.contains("is not in the sudoers")) {
                qCDebug(proofUtilsUpdatesLog) << "User not in sudoers list; log:\n" << readBuffer;
                emit q->errorOccurred(QObject::tr("User not in sudoers list"));
                return;
            }
            if (currentRead.contains("Sorry, try again")) {
                qCDebug(proofUtilsUpdatesLog) << "Sudo rejected the password; log:\n" << readBuffer;
                emit q->errorOccurred(QObject::tr("Incorrect password"));
                return;
            }
        }
        shutdownProcess->waitForFinished(-1);
        qCDebug(proofUtilsUpdatesLog) << "Shutdown exitcode =" << shutdownProcess->exitCode() << "; log:\n" << readBuffer + shutdownProcess->readAll().trimmed();
        if (shutdownProcess->exitCode())
            emit q->errorOccurred(QObject::tr("Unknown error"));
    } else {
        qCDebug(proofUtilsUpdatesLog) << "Process couldn't be started" << shutdownProcess->error() << shutdownProcess->errorString();
        emit q->errorOccurred(QObject::tr("Unknown error"));
    }
#else
    if (restart) {
        if (QProcess::startDetached(qApp->applicationFilePath())) {
            qApp->quit();
        } else {
            qCDebug(proofUtilsUpdatesLog) << "Can't restart application";
            emit q->errorOccurred(QObject::tr("Can't restart application"));
        }
    } else {
        qApp->quit();
    }
#endif
}

} // namespace Proof

WorkerThread::WorkerThread(Proof::PowerManagerPrivate *powerManager)
    : QThread(), powerManager(powerManager)
{
    moveToThread(this);
}

void WorkerThread::shutdowd(const QString &password, bool restart)
{
    powerManager->shutdowd(password, restart);
}

#include "powermanager.moc"
