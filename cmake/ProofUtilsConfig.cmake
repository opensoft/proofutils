include(CMakeFindDependencyMacro)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/3rdparty")
find_dependency(Qt5Core CONFIG REQUIRED)
find_dependency(Qt5Network CONFIG REQUIRED)
find_dependency(Qt5Gui CONFIG REQUIRED)
find_dependency(QRencode REQUIRED)
find_dependency(ProofCore CONFIG REQUIRED)
find_dependency(ProofNetworkLprPrinter CONFIG REQUIRED)
list(REMOVE_AT CMAKE_PREFIX_PATH -1)

if(NOT TARGET Proof::Utils)
    include("${CMAKE_CURRENT_LIST_DIR}/ProofUtilsTargets.cmake")
endif()
