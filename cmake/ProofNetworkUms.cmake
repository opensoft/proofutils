proof_add_target_sources(NetworkUms
    src/proofnetwork/ums/proofnetworkums_init.cpp
    src/proofnetwork/ums/data/umsuser.cpp
    src/proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.cpp
    src/proofnetwork/ums/data/qmlwrappers/umstokeninfoqmlwrapper.cpp
    src/proofnetwork/ums/tokensapi.cpp
    src/proofnetwork/ums/data/umstokeninfo.cpp
)

proof_add_target_headers(NetworkUms
    include/proofnetwork/ums/proofnetworkums_global.h
    include/proofnetwork/ums/proofnetworkums_types.h
    include/proofnetwork/ums/data/umsuser.h
    include/proofnetwork/ums/data/qmlwrappers/umsuserqmlwrapper.h
    include/proofnetwork/ums/data/qmlwrappers/umstokeninfoqmlwrapper.h
    include/proofnetwork/ums/tokensapi.h
    include/proofnetwork/ums/data/umstokeninfo.h
)

proof_add_target_private_headers(NetworkUms
    include/private/proofnetwork/ums/data/umsuser_p.h
)

proof_add_module(NetworkUms
    QT_LIBS Core Network
    PROOF_LIBS Core Network
)
