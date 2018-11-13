cmake_minimum_required(VERSION 3.10.3)
project(ProofNetworkMisQmlPlugin VERSION ${PROOF_VERSION} LANGUAGES CXX)

proof_add_target_sources(qmlmisplugin qmlmisplugin.cpp)
proof_add_target_headers(qmlmisplugin qmlmisplugin.h)

proof_add_qml_plugin(qmlmisplugin
    PLUGIN_PATH Network/Mis
    PROOF_LIBS NetworkMis
)

