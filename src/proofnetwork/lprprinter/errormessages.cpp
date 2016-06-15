#include "errormessages_p.h"

#include "proofnetworklprprinter_global.h"

#include <QSharedPointer>

QSharedPointer<Proof::ErrorMessagesRegistry> Proof::NetworkServices::lprPrinterServiceErrors()
{
    static const auto errorMessages = QSharedPointer<Proof::ErrorMessagesRegistry>(
        new Proof::ErrorMessagesRegistry{
            ErrorInfo{NETWORK_LPR_PRINTER_MODULE_CODE, NetworkErrorCode::InvalidRequest, "Incorrect request body", false}
    });
    return errorMessages;
}
