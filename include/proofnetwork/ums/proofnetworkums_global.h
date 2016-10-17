#ifndef PROOFNETWORKUMS_GLOBAL_H
#define PROOFNETWORKUMS_GLOBAL_H

#ifdef PROOF_NETWORK_UMS_LIB
#  define PROOF_NETWORK_UMS_EXPORT Q_DECL_EXPORT
#else
#  define PROOF_NETWORK_UMS_EXPORT Q_DECL_IMPORT
#endif

#include "proofnetwork/proofnetwork_global.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(proofNetworkUmsApiLog)
Q_DECLARE_LOGGING_CATEGORY(proofNetworkUmsDataLog)


namespace Proof {
namespace NetworkErrorCode {
enum UmsCode {
    InvalidTokenSignature = Code::MinCustomError
};
}
constexpr long NETWORK_UMS_MODULE_CODE = 318;
}

#endif // PROOFNETWORKUMS_GLOBAL_H
