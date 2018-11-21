cmake_minimum_required(VERSION 3.12.0)
project(ProofNetworkUmsTest LANGUAGES CXX)

proof_add_target_sources(network-ums_tests
    umsuser_test.cpp
    tokensapi_test.cpp
)
proof_add_target_resources(network-ums_tests tests_resources.qrc)

proof_add_test(network-ums_tests
    PROOF_LIBS NetworkUms
)
