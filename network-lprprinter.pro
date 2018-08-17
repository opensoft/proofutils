TEMPLATE = lib
TARGET = ProofNetworkLprPrinter
include(../proof.pri)
DESTDIR = $$BUILDPATH/lib

DEFINES += PROOF_NETWORK_LPRPRINTER_LIB

CONFIG += proofcore proofnetwork

HEADERS += \
    include/private/proofnetwork/lprprinter/errormessages_p.h \
    include/proofnetwork/lprprinter/lprprinterapi.h \
    include/proofnetwork/lprprinter/proofnetworklprprinter_global.h \
    include/proofnetwork/lprprinter/proofnetworklprprinter_types.h

SOURCES += \
    src/proofnetwork/lprprinter/errormessages.cpp \
    src/proofnetwork/lprprinter/lprprinterapi.cpp \
    src/proofnetwork/lprprinter/proofnetworklprprinter_init.cpp

include(../proof_translation.pri)
