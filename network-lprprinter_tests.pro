include (../tests/tests.pri)

QT += network gui
CONFIG += proofnetwork proofnetworklprprinter

SOURCES += \
    tests/proofnetwork/lprprinter/main.cpp \
    tests/proofnetwork/lprprinter/lprprinterapi_test.cpp

RESOURCES += \
    tests/proofnetwork/lprprinter/test_resources.qrc

DISTFILES += \
    tests/proofnetwork/lprprinter/data/status.json \
    tests/proofnetwork/lprprinter/data/failed_status.json \
    tests/proofnetwork/lprprinter/data/file.txt \
    tests/proofnetwork/lprprinter/data/printers.json
