proof_add_target_sources(NetworkLprPrinter
    src/proofnetwork/lprprinter/errormessages.cpp
    src/proofnetwork/lprprinter/lprprinterapi.cpp
    src/proofnetwork/lprprinter/proofnetworklprprinter_init.cpp
)

proof_add_target_headers(NetworkLprPrinter
    include/proofnetwork/lprprinter/lprprinterapi.h
    include/proofnetwork/lprprinter/proofnetworklprprinter_global.h
    include/proofnetwork/lprprinter/proofnetworklprprinter_types.h
)

proof_add_target_private_headers(NetworkLprPrinter
    include/private/proofnetwork/lprprinter/errormessages_p.h
)

proof_add_module(NetworkLprPrinter
    QT_LIBS Core Network
    PROOF_LIBS Core Network
)
