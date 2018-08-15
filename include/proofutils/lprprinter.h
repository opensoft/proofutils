#ifndef PROOF_UTILS_LPRPRINTER_H
#define PROOF_UTILS_LPRPRINTER_H

#include "proofseed/future.h"

#include "proofcore/proofobject.h"

#include "proofutils/proofutils_global.h"

namespace Proof {
namespace Hardware {
class LprPrinterPrivate;
class PROOF_UTILS_EXPORT LprPrinter : public ProofObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LprPrinter)
public:
    explicit LprPrinter(const QString &printerHost, const QString &printerName, bool strictPrinterCheck = false,
                        QObject *parent = nullptr);

    FutureSP<bool> printRawData(const QByteArray &data, bool ignorePrinterState = false) const;
    FutureSP<bool> printFile(const QString &fileName, unsigned int quantity = 1, bool ignorePrinterState = false) const;
    FutureSP<bool> printerIsReady() const;
};
} // namespace Hardware
} // namespace Proof

#endif // PROOF_UTILS_LPRPRINTER_H
