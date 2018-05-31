#include "mobileutils.h"

#include "proofcore/proofobject_p.h"

#include <QProcess>

#ifdef Q_OS_ANDROID
#    include <QtAndroid>
#    include <QtAndroidExtras/QAndroidJniEnvironment>
#endif

namespace {

class WorkerThread : public QThread // clazy:exclude=ctor-missing-parent-argument
{
    Q_OBJECT
public:
    explicit WorkerThread(Proof::MobileUtilsPrivate *mobileUtils);
    void callPhoneNumber(const QString &phoneNumber);
    QString imei();
    QString controlCenterFirebaseToken();

private:
    Proof::MobileUtilsPrivate *mobileUtils;
};

} // namespace

namespace Proof {

class MobileUtilsPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(MobileUtils)
public:
    void callPhoneNumber(const QString &phoneNumber);
    QString imei();
    QString controlCenterFirebaseToken();

    WorkerThread *thread = nullptr;
};

MobileUtils::MobileUtils(QObject *parent) : ProofObject(*new MobileUtilsPrivate, parent)
{
    Q_D(MobileUtils);
    d->thread = new WorkerThread(d);
    d->thread->start();
}

MobileUtils::~MobileUtils()
{
    Q_D(MobileUtils);
    d->thread->quit();
    d->thread->wait(1000);
    delete d->thread;
}

void MobileUtils::callPhoneNumber(const QString &phoneNumber)
{
    Q_D(MobileUtils);
    d->callPhoneNumber(phoneNumber);
}

QString MobileUtils::imei()
{
    Q_D(MobileUtils);
    return d->imei();
}

QString MobileUtils::controlCenterFirebaseToken()
{
    Q_D(MobileUtils);
    return d->controlCenterFirebaseToken();
}

void MobileUtilsPrivate::callPhoneNumber(const QString &phoneNumber)
{
    if (ProofObject::call(thread, &WorkerThread::callPhoneNumber, phoneNumber))
        return;

#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniEnvironment env;
    jstring jPhoneNumber = env->NewStringUTF(phoneNumber.toLatin1().constData());
    activity.callMethod<void>("callPhoneNumber", "(Ljava/lang/String;)V", jPhoneNumber);
    env->DeleteLocalRef(jPhoneNumber);
#else
    qCDebug(proofUtilsMiscLog) << "Dialing not support on this platform!";
#endif
}

QString MobileUtilsPrivate::imei()
{
    QString result;
    if (ProofObject::call(thread, &WorkerThread::imei, Proof::Call::Block, result))
        return result;

#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniEnvironment env;
    QAndroidJniObject jImei = activity.callObjectMethod<jstring>("getImei");
    result = jImei.toString();
#else
    qCDebug(proofUtilsMiscLog) << "IMEI is not supported on this platform!";
#endif
    return result;
}

QString MobileUtilsPrivate::controlCenterFirebaseToken()
{
    QString result;
    if (ProofObject::call(thread, &WorkerThread::controlCenterFirebaseToken, Proof::Call::Block, result))
        return result;

#ifdef Q_OS_ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();
    QAndroidJniEnvironment env;
    QAndroidJniObject jToken = activity.callObjectMethod<jstring>("getControlCenterFirebaseToken");
    result = jToken.toString();
#else
    qCDebug(proofUtilsMiscLog) << "Control center is not supported on this platform!";
#endif
    return result;
}

} // namespace Proof

WorkerThread::WorkerThread(Proof::MobileUtilsPrivate *mobileUtils) : QThread(), mobileUtils(mobileUtils)
{
    moveToThread(this);
}

void WorkerThread::callPhoneNumber(const QString &phoneNumber)
{
    mobileUtils->callPhoneNumber(phoneNumber);
}

QString WorkerThread::imei()
{
    return mobileUtils->imei();
}

QString WorkerThread::controlCenterFirebaseToken()
{
    return mobileUtils->controlCenterFirebaseToken();
}

#include "mobileutils.moc"
