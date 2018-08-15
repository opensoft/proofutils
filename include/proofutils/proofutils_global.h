#ifndef PROOFUTILS_GLOBAL_H
#define PROOFUTILS_GLOBAL_H

#ifdef PROOF_UTILS_LIB
#    define PROOF_UTILS_EXPORT Q_DECL_EXPORT
#else
#    define PROOF_UTILS_EXPORT Q_DECL_IMPORT
#endif

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(proofUtilsMiscLog);
Q_DECLARE_LOGGING_CATEGORY(proofUtilsEplGeneratorLog);
Q_DECLARE_LOGGING_CATEGORY(proofUtilsQrCodeGeneratorLog);
Q_DECLARE_LOGGING_CATEGORY(proofUtilsLprPrinterInfoLog);
Q_DECLARE_LOGGING_CATEGORY(proofUtilsLprPrinterDataLog);

namespace Proof {
namespace UtilsErrorCode {
enum Code
{
    UnknownError = 6,
    LabelPrinterError = 13,
    LprCannotBeStarted = 100,
    LpqCannotBeStarted = 101,
    LpoptionsCannotBeStarted = 102,
    LprProcessNonZeroExitCode = 103,
    PrinterInfoCannotBeQueried = 104,
    PrinterInfoError = 105,
    PrinterOptionsCannotBeQueried = 106,
    PrinterNotReady = 107,
    TemporaryFileError = 108,
    PrinterOffline = 109
};
}
constexpr long UTILS_MODULE_CODE = 200;
} // namespace Proof
#endif // PROOFUTILITIES_GLOBAL_H
