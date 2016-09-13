#ifndef PROOFUTILS_GLOBAL_H
#define PROOFUTILS_GLOBAL_H

#ifdef PROOF_UTILS_LIB
#  define PROOF_UTILS_EXPORT Q_DECL_EXPORT
#else
#  define PROOF_UTILS_EXPORT Q_DECL_IMPORT
#endif

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(proofUtilsMiscLog)
Q_DECLARE_LOGGING_CATEGORY(proofUtilsEplGeneratorLog)
Q_DECLARE_LOGGING_CATEGORY(proofUtilsQrCodeGeneratorLog)
Q_DECLARE_LOGGING_CATEGORY(proofUtilsNetworkConfigurationLog)

namespace Proof {
namespace UtilsErrorCode {
enum Code {
    IncorrectPassword = 1,
    NoAnswerFromSystem = 2,
    UserNotASudoer = 3,
    ApplicationCannotBeRestarted = 4,
    PlatformNotSupported = 5,
    UnknownError = 6,
    NetworkConfigurationCannotBeWritten = 7
};
}
constexpr long UTILS_MODULE_CODE = 200;
}
#endif // PROOFUTILITIES_GLOBAL_H
