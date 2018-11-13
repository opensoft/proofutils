cmake_minimum_required(VERSION 3.10.3)
if(NOT DEFINED PROOF_FULL_BUILD)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_INSTALL_PREFIX}/lib/cmake/modules")
    include(ProofInternal)
    proof_init()
endif()

project(ProofUtils VERSION ${PROOF_VERSION} LANGUAGES CXX)

find_package(Qt5Core CONFIG REQUIRED)
if(NOT DEFINED PROOF_FULL_BUILD)
    find_package(ProofNetwork CONFIG REQUIRED)
    find_package(proof-gtest CONFIG REQUIRED)
    enable_testing()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(ProofNetworkMis)
include(ProofNetworkUms)
include(ProofNetworkLprPrinter)
include(ProofUtils)

add_subdirectory(plugins/qmlmisplugin)

add_subdirectory(tests/proofutils)
add_subdirectory(tests/proofnetwork/mis)
add_subdirectory(tests/proofnetwork/ums)
add_subdirectory(tests/proofnetwork/lprprinter)
