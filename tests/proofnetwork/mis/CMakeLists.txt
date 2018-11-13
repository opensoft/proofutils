cmake_minimum_required(VERSION 3.12.0)
project(ProofNetworkMisTest LANGUAGES CXX)

proof_add_target_sources(network-mis_test
    job_test.cpp
)
proof_add_target_resources(network-mis_test tests_resources.qrc)

proof_add_test(network-mis_test
    PROOF_LIBS NetworkMis
)
