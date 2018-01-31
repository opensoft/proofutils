#include "labelprinter.h"

#include "proofcore/proofobject_p.h"
#include "proofcore/taskchain.h"
#ifndef Q_OS_ANDROID
# include "proofhardware/lprprinter/lprprinter.h"
#endif
#include "proofnetwork/lprprinter/lprprinterapi.h"
#include "proofnetwork/apicall.h"

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

FutureSP<bool> LabelPrinter::printLabel(const QByteArray &label, bool ignorePrinterState)
{
    Q_D(LabelPrinter);
#ifndef Q_OS_ANDROID
    if (d->hardwareLabelPrinter)
        return Future<bool>::successful(d->hardwareLabelPrinter->printRawData(label, ignorePrinterState));
#else
    Q_UNUSED(ignorePrinterState)
#endif
    return ApiCall<void>::exec(d->labelPrinterApi,
                               &Proof::NetworkServices::LprPrinterApi::printLabel,
                               &Proof::NetworkServices::LprPrinterApi::labelPrinted,
                               label, d->printerName);
}

FutureSP<bool> LabelPrinter::printerIsReady()
{
    Q_D(LabelPrinter);
#ifndef Q_OS_ANDROID
    if (d->hardwareLabelPrinter)
        return Future<bool>::successful(d->hardwareLabelPrinter->printerIsReady());
#endif
    auto result = ApiCall<Proof::NetworkServices::LprPrinterStatus>::exec(d->labelPrinterApi,
                                                                          &Proof::NetworkServices::LprPrinterApi::fetchStatus,
                                                                          &Proof::NetworkServices::LprPrinterApi::statusFetched,
                                                                          d->printerName);
    return result->map([](const auto &status) -> bool {
        if (status.isReady)
            return true;
        else
            return WithFailure(status.reason, UTILS_MODULE_CODE, UtilsErrorCode::LabelPrinterError);
    });
}
