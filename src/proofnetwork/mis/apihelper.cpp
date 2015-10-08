#include "apihelper.h"

#include <QHash>

using namespace Proof::Mis;

static const QHash<QString, ApiHelper::WorkflowStatus> WORKFLOW_STATUSES = {
    {"needs", ApiHelper::WorkflowStatus::NeedsStatus},
    {"is ready for", ApiHelper::WorkflowStatus::IsReadyForStatus},
    {"in progress", ApiHelper::WorkflowStatus::InProgressStatus},
    {"suspended", ApiHelper::WorkflowStatus::SuspendedStatus},
    {"done", ApiHelper::WorkflowStatus::DoneStatus},
    {"halted", ApiHelper::WorkflowStatus::HaltedStatus}
};

static const QHash<QString, ApiHelper::TransitionEvent> TRANSITION_EVENTS = {
    {"start", ApiHelper::TransitionEvent::StartEvent},
    {"stop", ApiHelper::TransitionEvent::StopEvent},
    {"abort", ApiHelper::TransitionEvent::AbortEvent},
    {"suspend", ApiHelper::TransitionEvent::SuspendEvent},
    {"resume", ApiHelper::TransitionEvent::ResumeEvent},
    {"perform", ApiHelper::TransitionEvent::PerformEvent},
    {"revert", ApiHelper::TransitionEvent::RevertEvent},
    {"request", ApiHelper::TransitionEvent::RequestEvent}
};

QString ApiHelper::workflowStatusToString(ApiHelper::WorkflowStatus status)
{
    return WORKFLOW_STATUSES.key(status, "");
}

ApiHelper::WorkflowStatus ApiHelper::workflowStatusFromString(const QString &statusString, bool *ok)
{
    if (ok != nullptr)
        *ok = WORKFLOW_STATUSES.contains(statusString);
    return WORKFLOW_STATUSES.value(statusString.toLower(), WorkflowStatus::UnknownStatus);
}

QString ApiHelper::transitionEventToString(ApiHelper::TransitionEvent event)
{
    return TRANSITION_EVENTS.key(event, "");
}

ApiHelper::TransitionEvent ApiHelper::transitionEventFromString(const QString &eventString, bool *ok)
{
    if (ok != nullptr)
        *ok = TRANSITION_EVENTS.contains(eventString);
    return TRANSITION_EVENTS.value(eventString.toLower(), TransitionEvent::UnknownEvent);
}

namespace Proof {
namespace Mis {

uint qHash(ApiHelper::WorkflowStatus arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(ApiHelper::TransitionEvent arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

}
}
