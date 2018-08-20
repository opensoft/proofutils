PROOF_PRI_PATH = $$PWD/../proofboot
!exists($$PROOF_PRI_PATH/proof_tests.pri):PROOF_PRI_PATH = $$(PROOF_PATH)
include($$PROOF_PRI_PATH/proof_tests.pri)

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

