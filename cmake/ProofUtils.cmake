proof_add_target_sources(Utils
    src/proofutils/proofutils_init.cpp
    src/proofutils/epllabelgenerator.cpp
    src/proofutils/qrcodegenerator.cpp
    src/proofutils/labelprinter.cpp
)

proof_add_target_headers(Utils
    include/proofutils/proofutils_global.h
    include/proofutils/epllabelgenerator.h
    include/proofutils/qrcodegenerator.h
    include/proofutils/labelprinter.h
    include/proofutils/basic_package.h
)

if (NOT ANDROID)
    proof_add_target_sources(Utils src/proofutils/lprprinter.cpp)
    proof_add_target_headers(Utils include/proofutils/lprprinter.h)
endif()

find_package(QRencode REQUIRED)
proof_add_module(Utils
    QT_LIBS Core Network Gui
    PROOF_LIBS Core NetworkLprPrinter
    OTHER_LIBS QRencode::QRencode
)
