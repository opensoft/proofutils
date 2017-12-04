#include "labelprinter.h"

#include "proofcore/proofobject_p.h"
#include "proofcore/taskchain.h"
#ifndef Q_OS_ANDROID
# include "proofhardware/lprprinter/lprprinter.h"
#endif
#include "proofnetwork/lprprinter/lprprinterapi.h"

namespace Proof {
class LabelPrinterPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(LabelPrinter)

#ifndef Q_OS_ANDROID
    Proof::Hardware::LprPrinter *hardwareLabelPrinter = nullptr;
#endif
    Proof::NetworkServices::LprPrinterApi *labelPrinterApi = nullptr;
    Proof::RestClientSP restClient;
//Think about moving this object back to one thread
//bool LabelPrinter::event(QEvent *ev)
//{
//    Q_D(LabelPrinter);
//    if (ev->type() == QEvent::ThreadChange) {
//        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
//    } else if (ev->type() == (QEvent::User)) {
//        if (d->restClient)
//            call(d->restClient.data(), &QObject::moveToThread, Proof::Call::BlockEvents, thread());
//    }
//    return QObject::event(ev);
//}
// This approach works fine, except one race when it is possible to call something between these two events which will cause the deadlock.
// If having extra thread for each label printer will affect us by any way sometime we need to return back to it and fix the race.
    QThread *thread = nullptr;

    QString printerName;
};

} // namespace Proof

using namespace Proof;

LabelPrinter::LabelPrinter(const LabelPrinterParams &params, QObject *parent)
    : ProofObject(*new LabelPrinterPrivate, parent)
{
    Q_D(LabelPrinter);
    d->printerName = params.printerName;
#ifndef Q_OS_ANDROID
    if (!params.forceServiceUsage && !params.printerName.isEmpty()) {
        d->hardwareLabelPrinter = new Proof::Hardware::LprPrinter(params.printerHost, params.printerName, params.strictHardwareCheck, this);
        connect(d->hardwareLabelPrinter, &Proof::Hardware::LprPrinter::errorOccurred,
                this, &Proof::LabelPrinter::errorOccurred);
        return;
    }
#endif

    d->restClient = Proof::RestClientSP::create();
    d->restClient->setAuthType(Proof::RestAuthType::NoAuth);
    d->restClient->setScheme(QStringLiteral("http"));
    d->restClient->setHost(params.printerHost.isEmpty() ? QStringLiteral("127.0.0.1") : params.printerHost);
    d->restClient->setPort(params.printerPort);
    d->labelPrinterApi = new Proof::NetworkServices::LprPrinterApi(d->restClient);

    d->thread = new QThread(this);
    d->labelPrinterApi->moveToThread(d->thread);
    d->restClient->moveToThread(d->thread);
    d->thread->start();
}

LabelPrinter::~LabelPrinter()
{
    Q_D(LabelPrinter);
    if (d->thread) {
        d->thread->quit();
        d->thread->wait(1000);
    }
    if (d->labelPrinterApi)
        delete d->labelPrinterApi;
}

bool LabelPrinter::printLabel(const QByteArray &label, bool ignorePrinterState)
{
    Q_D(LabelPrinter);
    bool result = false;

#ifndef Q_OS_ANDROID
    if (d->hardwareLabelPrinter)
        return d->hardwareLabelPrinter->printRawData(label, ignorePrinterState);
#else
    Q_UNUSED(ignorePrinterState)
#endif

    auto taskChain = Proof::TaskChain::createChain();
    taskChain->waitForTask(taskChain->addTask([this, taskChain, &result, label, d](){
        auto apiResult = Proof::AbstractRestApi::chainedApiCallWithoutResult(taskChain, d->labelPrinterApi,
                                                                             &Proof::NetworkServices::LprPrinterApi::printLabel,
                                                                             &Proof::NetworkServices::LprPrinterApi::labelPrinted,
                                                                             label, d->printerName);
        result = (apiResult.level == Proof::RestApiError::Level::NoError);
        if (apiResult.level != Proof::RestApiError::Level::NoError)
            emit errorOccurred(apiResult.proofModuleCode, apiResult.proofErrorCode, apiResult.toString(), apiResult.userFriendly);
    }));
    return result;
}

bool LabelPrinter::printerIsReady()
{
    Q_D(LabelPrinter);
    bool result = false;

#ifndef Q_OS_ANDROID
    if (d->hardwareLabelPrinter)
        return d->hardwareLabelPrinter->printerIsReady();
#endif

    auto taskChain = Proof::TaskChain::createChain();
    taskChain->waitForTask(taskChain->addTask([this, taskChain, &result, d](){
        Proof::NetworkServices::LprPrinterStatus status;
        auto apiResult = Proof::AbstractRestApi::chainedApiCall(taskChain, d->labelPrinterApi,
                                                                &Proof::NetworkServices::LprPrinterApi::fetchStatus,
                                                                &Proof::NetworkServices::LprPrinterApi::statusFetched,
                                                                status, QString());
        result = (apiResult.level == Proof::RestApiError::Level::NoError) && status.isReady;
        if (apiResult.level != Proof::RestApiError::Level::NoError)
            emit errorOccurred(apiResult.proofModuleCode, apiResult.proofErrorCode, apiResult.toString(), apiResult.userFriendly);
        else if (!status.isReady)
            emit errorOccurred(UTILS_MODULE_CODE, UtilsErrorCode::LabelPrinterError, status.reason, false);
    }));
    return result;
}
