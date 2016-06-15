#ifndef PROOFNETWORKLPRPRINTER_GLOBAL_H
#define PROOFNETWORKLPRPRINTER_GLOBAL_H

#ifdef PROOF_NETWORK_LPRPRINTER_LIB
#  define PROOF_NETWORK_LPRPRINTER_EXPORT Q_DECL_EXPORT
#else
#  define PROOF_NETWORK_LPRPRINTER_EXPORT Q_DECL_IMPORT
#endif

#include "proofnetwork/proofnetwork_global.h"

Q_DECLARE_LOGGING_CATEGORY(proofNetworkLprPrinterLog)

namespace Proof {
namespace NetworkErrorCode {
enum FieryCode {
    FileError = Code::MinCustomError
};
}
constexpr long NETWORK_LPR_PRINTER_MODULE_CODE = 307;
}

#endif // PROOFNETWORKLPRPRINTER_GLOBAL_H
