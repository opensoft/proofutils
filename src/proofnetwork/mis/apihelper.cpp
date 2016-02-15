#include "apihelper.h"

using namespace Proof::Mis;

/*!
 * \class Proof::Mis::ApiHelper apihelper.h "proofnetwork/mis/apihelper.h"
 * Helper class for Mis API
 */

ApiHelper::WorkflowStatus ApiHelper::workflowStatusAfterTransitionEvent(Proof::Mis::ApiHelper::TransitionEvent event)
{
    switch (event) {
    case Proof::Mis::ApiHelper::TransitionEvent::StartEvent:
        return Proof::Mis::ApiHelper::WorkflowStatus::InProgressStatus;
    case Proof::Mis::ApiHelper::TransitionEvent::StopEvent:
        return Proof::Mis::ApiHelper::WorkflowStatus::DoneStatus;
    case Proof::Mis::ApiHelper::TransitionEvent::AbortEvent:
        return Proof::Mis::ApiHelper::WorkflowStatus::IsReadyForStatus;
    case Proof::Mis::ApiHelper::TransitionEvent::SuspendEvent:
        return Proof::Mis::ApiHelper::WorkflowStatus::SuspendedStatus;
    case Proof::Mis::ApiHelper::TransitionEvent::ResumeEvent:
        return Proof::Mis::ApiHelper::WorkflowStatus::InProgressStatus;
    default:
        return Proof::Mis::ApiHelper::WorkflowStatus::UnknownStatus;
    }
}
