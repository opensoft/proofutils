TEMPLATE = lib
TARGET = ProofNetworkUms
include(../proof.pri)
DESTDIR = $$BUILDPATH/lib

DEFINES += PROOF_NETWORK_UMS_LIB

CONFIG += proofcore proofnetwork

HEADERS += \
    include/proofnetwork/ums/proofnetworkums_global.h \
    include/proofnetwork/ums/proofnetworkums_types.h \
    include/proofnetwork/ums/data/umsuser.h \
    include/proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.h \
    include/proofnetwork/ums/data/qmlwrappers/umstokeninfoqmlwrapper.h \
    include/proofnetwork/ums/tokensapi.h \
    include/proofnetwork/ums/data/umstokeninfo.h \
    include/private/proofnetwork/ums/data/umsuser_p.h

SOURCES += \
    src/proofnetwork/ums/proofnetworkums_init.cpp \
    src/proofnetwork/ums/data/umsuser.cpp \
    src/proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.cpp \
    src/proofnetwork/ums/data/qmlwrappers/umstokeninfoqmlwrapper.cpp \
    src/proofnetwork/ums/tokensapi.cpp \
    src/proofnetwork/ums/data/umstokeninfo.cpp

include(../proof_translation.pri)
