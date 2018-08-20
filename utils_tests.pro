PROOF_PRI_PATH = $$PWD/../proofboot
!exists($$PROOF_PRI_PATH/proof_tests.pri):PROOF_PRI_PATH = $$(PROOF_PATH)
include($$PROOF_PRI_PATH/proof_tests.pri)

QT += gui
CONFIG += proofutils

SOURCES += \
    tests/proofutils/main.cpp \
    tests/proofutils/epllabelgenerator_test.cpp

RESOURCES += \
    tests/proofutils/tests_resources.qrc

DISTFILES += \
    tests/proofutils/data/emptylabel.epl
