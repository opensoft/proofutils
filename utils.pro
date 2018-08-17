TEMPLATE = lib
TARGET = ProofUtils
QT += network
include(../proof.pri)
DESTDIR = $$BUILDPATH/lib

DEFINES += PROOF_UTILS_LIB

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

include(../proof_translation.pri)
