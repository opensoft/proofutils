TEMPLATE = lib
TARGET = ProofNetworkMis
PROOF_PRI_PATH = $$PWD/../proofboot
!exists($$PROOF_PRI_PATH/proof.pri):PROOF_PRI_PATH = $$(PROOF_PATH)
include($$PROOF_PRI_PATH/proof.pri)

DEFINES += PROOF_NETWORK_MIS_LIB

CONFIG += proofcore proofnetwork

HEADERS += \
    include/proofnetwork/mis/proofnetworkmis_global.h \
    include/proofnetwork/mis/proofnetworkmis_types.h \
    include/proofnetwork/mis/apihelper.h \
    include/proofnetwork/mis/data/job.h \
    include/proofnetwork/mis/data/workflowelement.h \
    include/proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h

SOURCES += \
    src/proofnetwork/mis/proofnetworkmis_init.cpp \
    src/proofnetwork/mis/data/job.cpp \
    src/proofnetwork/mis/data/workflowelement.cpp \
    src/proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.cpp \
    src/proofnetwork/mis/apihelper.cpp


include($$PROOF_PRI_PATH/proof_translation.pri)
