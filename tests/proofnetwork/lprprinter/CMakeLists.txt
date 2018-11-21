cmake_minimum_required(VERSION 3.12.0)
project(ProofNetworkLprPrinterTest LANGUAGES CXX)

proof_add_target_sources(network-lprprinter_tests
    lprprinterapi_test.cpp
)
proof_add_target_resources(network-lprprinter_tests test_resources.qrc)

proof_add_test(network-lprprinter_tests
    PROOF_LIBS NetworkLprPrinter
)
