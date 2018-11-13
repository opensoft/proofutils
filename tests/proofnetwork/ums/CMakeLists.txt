cmake_minimum_required(VERSION 3.10.3)
project(ProofNetworkUmsTest LANGUAGES CXX)

proof_add_target_sources(network-ums_test
    umsuser_test.cpp
    tokensapi_test.cpp
)
proof_add_target_resources(network-ums_test tests_resources.qrc)

proof_add_test(network-ums_test
    PROOF_LIBS NetworkUms
)
