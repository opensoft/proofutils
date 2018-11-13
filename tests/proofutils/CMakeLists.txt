cmake_minimum_required(VERSION 3.10.3)
project(ProofUtilsTest LANGUAGES CXX)

proof_add_target_sources(utils_test
    epllabelgenerator_test.cpp
)
proof_add_target_resources(utils_test tests_resources.qrc)

proof_add_test(utils_test
    PROOF_LIBS Utils
)
