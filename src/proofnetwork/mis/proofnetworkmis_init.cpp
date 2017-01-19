#include "data/job.h"
#include "data/qmlwrappers/jobqmlwrapper.h"
#include "apihelper.h"

#include "proofnetworkmis_global.h"
#include "proofcore/proofglobal.h"

Q_LOGGING_CATEGORY(proofNetworkMisDataLog, "proof.network.mis.data")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    qRegisterMetaType<Proof::Mis::JobQmlWrapper *>("Proof::Mis::JobQmlWrapper *");

    qRegisterMetaType<Proof::Mis::JobSP>("Proof::Mis::JobSP");
    qRegisterMetaType<Proof::Mis::JobWP>("Proof::Mis::JobWP");
    qRegisterMetaType<QList<Proof::Mis::JobSP>>("QList<Proof::Mis::JobSP>");

    qRegisterMetaType<Proof::Mis::ApiHelper::WorkflowStatus>("Proof::Mis::ApiHelper::WorkflowStatus");
    qRegisterMetaType<Proof::Mis::ApiHelper::TransitionEvent>("Proof::Mis::ApiHelper::TransitionEvent");
    qRegisterMetaType<Proof::Mis::ApiHelper::WorkflowAction>("Proof::Mis::ApiHelper::WorkflowAction");
    qRegisterMetaType<Proof::Mis::ApiHelper::PaperSide>("Proof::Mis::ApiHelper::PaperSide");
}
