#include "proofnetworklprprinter_global.h"
#include "lprprinterapi.h"

Q_LOGGING_CATEGORY(proofNetworkLprPrinterLog, "proof.network.lpr-printer")

__attribute__((constructor))
static void libraryInit()
{
    qRegisterMetaType<Proof::NetworkServices::LprPrinterStatus>("Proof::NetworkServices::LprPrinterStatus");
    qRegisterMetaType<Proof::NetworkServices::LprPrinterInfo>("Proof::NetworkServices::LprPrinterInfo");
    qRegisterMetaType<QList<Proof::NetworkServices::LprPrinterInfo>>("QList<Proof::NetworkServices::LprPrinterInfo>");
}

