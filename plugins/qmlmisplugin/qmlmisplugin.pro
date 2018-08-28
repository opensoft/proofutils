PLUGIN_PATH = Network/Mis
TARGET  = qmlmisplugin

PROOF_PRI_PATH = $$PWD/../../../proofboot
!exists($$PROOF_PRI_PATH/proof_qmlplugin.pri):PROOF_PRI_PATH = $$(PROOF_PATH)
include($$PROOF_PRI_PATH/proof_qmlplugin.pri)

CONFIG += proofnetworkmis

HEADERS += qmlmisplugin.h
SOURCES += qmlmisplugin.cpp
DISTFILES += qmldir
