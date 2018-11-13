proof_add_target_sources(NetworkMis
    src/proofnetwork/mis/proofnetworkmis_init.cpp
    src/proofnetwork/mis/data/job.cpp
    src/proofnetwork/mis/data/workflowelement.cpp
    src/proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.cpp
    src/proofnetwork/mis/apihelper.cpp
)

proof_add_target_headers(NetworkMis
    include/proofnetwork/mis/proofnetworkmis_global.h
    include/proofnetwork/mis/proofnetworkmis_types.h
    include/proofnetwork/mis/apihelper.h
    include/proofnetwork/mis/data/job.h
    include/proofnetwork/mis/data/workflowelement.h
    include/proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h
)

proof_force_moc(NetworkMis include/proofnetwork/mis/apihelper.h)

proof_add_module(NetworkMis
    QT_LIBS Core Network
    PROOF_LIBS Core Network
)
