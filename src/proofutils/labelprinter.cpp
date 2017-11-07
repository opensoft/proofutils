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

    QString printerName;
};

} // namespace Proof

using namespace Proof;

LabelPrinter::LabelPrinter(const QString &printerHost, const QString &printerName, int printerPort,
                           bool forceServiceUsage, bool strictHardwareCheck, QObject *parent)
    : ProofObject(*new LabelPrinterPrivate, parent)
{
    Q_D(LabelPrinter);
    d->printerName = printerName;
#ifndef Q_OS_ANDROID
    if (!forceServiceUsage && !printerName.isEmpty()) {
        d->hardwareLabelPrinter = new Proof::Hardware::LprPrinter(printerHost, printerName, strictHardwareCheck, this);
        connect(d->hardwareLabelPrinter, &Proof::Hardware::LprPrinter::errorOccurred,
                this, &Proof::LabelPrinter::errorOccurred);
        return;
    }
#else
    Q_UNUSED(forceServiceUsage)
    Q_UNUSED(strictHardwareCheck)
#endif

    d->restClient = Proof::RestClientSP::create();
    d->restClient->setAuthType(Proof::RestAuthType::NoAuth);
    d->restClient->setScheme(QStringLiteral("http"));
    d->restClient->setHost(printerHost.isEmpty() ? QStringLiteral("127.0.0.1") : printerHost);
    d->restClient->setPort(printerPort);
    d->labelPrinterApi = new Proof::NetworkServices::LprPrinterApi(d->restClient, this);
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

bool LabelPrinter::event(QEvent *ev)
{
    Q_D(LabelPrinter);
    if (ev->type() == QEvent::ThreadChange) {
        QCoreApplication::postEvent(this, new QEvent(QEvent::User));
    } else if (ev->type() == (QEvent::User)) {
        if (d->restClient)
            call(d->restClient.data(), &QObject::moveToThread, Proof::Call::BlockEvents, thread());
    }
    return QObject::event(ev);
}
