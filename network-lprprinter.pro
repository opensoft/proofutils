TEMPLATE = lib
TARGET = ProofNetworkLprPrinter
PROOF_PRI_PATH = $$PWD/../proofboot
!exists($$PROOF_PRI_PATH/proof.pri):PROOF_PRI_PATH = $$(PROOF_PATH)
include($$PROOF_PRI_PATH/proof.pri)

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

include($$PROOF_PRI_PATH/proof_translation.pri)
