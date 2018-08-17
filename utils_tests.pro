include (../tests/tests.pri)

QT += gui
CONFIG += proofutils

SOURCES += \
    tests/proofutils/main.cpp \
    tests/proofutils/epllabelgenerator_test.cpp

RESOURCES += \
    tests/proofutils/tests_resources.qrc

DISTFILES += \
    tests/proofutils/data/emptylabel.epl
