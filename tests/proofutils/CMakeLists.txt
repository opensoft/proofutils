cmake_minimum_required(VERSION 3.12.0)
project(ProofUtilsTest LANGUAGES CXX)

proof_add_target_sources(utils_tests
    epllabelgenerator_test.cpp
)
proof_add_target_resources(utils_tests tests_resources.qrc)

proof_add_test(utils_tests
    PROOF_LIBS Utils
)
