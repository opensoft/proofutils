TEMPLATE = lib
TARGET = ProofUtils
QT += network
PROOF_PRI_PATH = $$PWD/../proofboot
!exists($$PROOF_PRI_PATH/proof.pri):PROOF_PRI_PATH = $$(PROOF_PATH)
include($$PROOF_PRI_PATH/proof.pri)

DEFINES += Proof_Utils_EXPORTS

CONFIG += proofcore proofnetwork proofnetworklprprinter
msvc {
    CONFIG(debug, debug|release):LIBS += -lqrencoded
    else:LIBS += -lqrencode
} else {
    LIBS += -lqrencode
}

HEADERS += \
    include/proofutils/proofutils_global.h \
    include/proofutils/epllabelgenerator.h \
    include/proofutils/qrcodegenerator.h \
    include/proofutils/labelprinter.h \
    include/proofutils/basic_package.h

SOURCES += \
    src/proofutils/proofutils_init.cpp \
    src/proofutils/epllabelgenerator.cpp \
    src/proofutils/qrcodegenerator.cpp \
    src/proofutils/labelprinter.cpp

!android {
HEADERS += \
    include/proofutils/lprprinter.h

SOURCES += \
    src/proofutils/lprprinter.cpp
}

include($$PROOF_PRI_PATH/proof_translation.pri)
