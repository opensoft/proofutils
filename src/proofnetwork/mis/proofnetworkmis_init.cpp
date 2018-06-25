#include "apihelper.h"
#include "data/job.h"
#include "data/qmlwrappers/jobqmlwrapper.h"
#include "proofnetworkmis_global.h"

#include "proofcore/proofglobal.h"

Q_LOGGING_CATEGORY(proofNetworkMisDataLog, "proof.network.mis.data")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    // clang-format off
    qRegisterMetaType<Proof::Mis::JobQmlWrapper *>("Proof::Mis::JobQmlWrapper *");

    qRegisterMetaType<Proof::Mis::JobSP>("Proof::Mis::JobSP");
    qRegisterMetaType<Proof::Mis::JobWP>("Proof::Mis::JobWP");
    qRegisterMetaType<QVector<Proof::Mis::JobSP>>("QVector<Proof::Mis::JobSP>");

    qRegisterMetaType<Proof::Mis::EntityStatus>("Proof::Mis::EntityStatus");
    qRegisterMetaType<Proof::Mis::WorkflowStatus>("Proof::Mis::WorkflowStatus");
    qRegisterMetaType<Proof::Mis::TransitionEvent>("Proof::Mis::TransitionEvent");
    qRegisterMetaType<Proof::Mis::WorkflowAction>("Proof::Mis::WorkflowAction");
    qRegisterMetaType<Proof::Mis::PaperSide>("Proof::Mis::PaperSide");
    // clang-format on
}
