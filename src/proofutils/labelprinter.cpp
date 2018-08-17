#include "proofutils/labelprinter.h"

#include "proofseed/tasks.h"

#include "proofcore/proofobject_p.h"

#include "proofnetwork/lprprinter/lprprinterapi.h"

#ifndef Q_OS_ANDROID
#    include "proofutils/lprprinter.h"
#endif

namespace Proof {
class LabelPrinterPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(LabelPrinter)

#ifndef Q_OS_ANDROID
    Proof::Hardware::LprPrinter *hardwareLabelPrinter = nullptr;
#endif
    Proof::NetworkServices::LprPrinterApi *labelPrinterApi = nullptr;

    LabelPrinterParams params;
};

} // namespace Proof

using namespace Proof;

LabelPrinter::LabelPrinter(const LabelPrinterParams &params, QObject *parent)
    : ProofObject(*new LabelPrinterPrivate, parent)
{
    Q_D(LabelPrinter);
    d->params = params;
#ifndef Q_OS_ANDROID
    if (!params.forceServiceUsage && !params.printerName.isEmpty()) {
        d->hardwareLabelPrinter = new Proof::Hardware::LprPrinter(params.printerHost, params.printerName,
                                                                  params.strictHardwareCheck, this);
        return;
    }
#endif

    auto restClient = Proof::RestClientSP::create();
    restClient->setAuthType(Proof::RestAuthType::NoAuth);
    restClient->setScheme(QStringLiteral("http"));
    restClient->setHost(params.printerHost.isEmpty() ? QStringLiteral("127.0.0.1") : params.printerHost);
    restClient->setPort(params.printerPort);
    d->labelPrinterApi = new Proof::NetworkServices::LprPrinterApi(restClient, this);
}

LabelPrinter::~LabelPrinter()
{}

FutureSP<bool> LabelPrinter::printLabel(const QByteArray &label, bool ignorePrinterState) const
{
    Q_D_CONST(LabelPrinter);
#ifndef Q_OS_ANDROID
    if (d->hardwareLabelPrinter)
        return d->hardwareLabelPrinter->printRawData(label, ignorePrinterState);
#else
    Q_UNUSED(ignorePrinterState)
#endif
    return d->labelPrinterApi->printLabel(label, d->params.printerName);
}

FutureSP<bool> LabelPrinter::printerIsReady() const
{
    Q_D_CONST(LabelPrinter);
#ifndef Q_OS_ANDROID
    if (d->hardwareLabelPrinter)
        return d->hardwareLabelPrinter->printerIsReady();
#endif
    return d->labelPrinterApi->fetchStatus(d->params.printerName)->map([](const auto &status) -> bool {
        if (status.isReady)
            return true;
        else
            return WithFailure(status.reason, UTILS_MODULE_CODE, UtilsErrorCode::LabelPrinterError);
    });
}

QString LabelPrinter::title() const
{
    Q_D_CONST(LabelPrinter);
    return d->params.printerTitle;
}
