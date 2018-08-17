include (../tests/tests.pri)

QT += network
CONFIG += proofnetworkmis

SOURCES += \
    tests/proofnetwork/mis/main.cpp \
    tests/proofnetwork/mis/job_test.cpp

RESOURCES += \
    tests/proofnetwork/mis/tests_resources.qrc

DISTFILES += \
    tests/proofnetwork/mis/data/job.json \
    tests/proofnetwork/mis/data/job2.json

