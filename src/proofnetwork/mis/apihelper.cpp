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

static const QHash<QString, ApiHelper::WorkflowAction> WORKFLOW_ACTIONS = {
    {"binding", ApiHelper::WorkflowAction::BindingAction},
    {"binning", ApiHelper::WorkflowAction::BinningAction},
    {"boxing", ApiHelper::WorkflowAction::BoxingAction},
    {"color optimizing", ApiHelper::WorkflowAction::ColorOptimizingAction},
    {"component boxing", ApiHelper::WorkflowAction::ComponentBoxingAction},
    {"container packing", ApiHelper::WorkflowAction::ContainerPackingAction},
    {"cutting", ApiHelper::WorkflowAction::CuttingAction},
    {"distribute", ApiHelper::WorkflowAction::DistributeAction},
    {"folder making", ApiHelper::WorkflowAction::FolderMakingAction},
    {"magnetize", ApiHelper::WorkflowAction::MagnetizeAction},
    {"mailing", ApiHelper::WorkflowAction::MailingAction},
    {"mounting", ApiHelper::WorkflowAction::MountingAction},
    {"outsource cutting", ApiHelper::WorkflowAction::OutsourceCuttingAction},
    {"pdf building", ApiHelper::WorkflowAction::PdfBuildingAction},
    {"plate making", ApiHelper::WorkflowAction::PlateMakingAction},
    {"printing", ApiHelper::WorkflowAction::PrintingAction},
    {"qc", ApiHelper::WorkflowAction::QcAction},
    {"rounding", ApiHelper::WorkflowAction::RoundingAction},
    {"screen imaging", ApiHelper::WorkflowAction::ScreenImagingAction},
    {"screen mounting", ApiHelper::WorkflowAction::ScreenMountingAction},
    {"screen preparation", ApiHelper::WorkflowAction::ScreenPreparationAction},
    {"screen washing", ApiHelper::WorkflowAction::ScreenWashingAction},
    {"ship boxing", ApiHelper::WorkflowAction::ShipBoxingAction},
    {"ship label", ApiHelper::WorkflowAction::ShipLabelAction},
    {"shipping", ApiHelper::WorkflowAction::ShippingAction},
    {"staging", ApiHelper::WorkflowAction::StagingAction},
    {"truck loading", ApiHelper::WorkflowAction::TruckLoadingAction},
    {"uv coating", ApiHelper::WorkflowAction::UvCoatingAction},
    {"uv pdf building", ApiHelper::WorkflowAction::UvPdfBuildingAction}
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

static const QHash<QString, ApiHelper::PaperSide> PAPER_SIDES = {
    {"", ApiHelper::PaperSide::NotSetSide},
    {"front", ApiHelper::PaperSide::FrontSide},
    {"back", ApiHelper::PaperSide::BackSide}
};

QString ApiHelper::workflowStatusToString(ApiHelper::WorkflowStatus status)
{
    return WORKFLOW_STATUSES.key(status, "");
}

ApiHelper::WorkflowStatus ApiHelper::workflowStatusFromString(QString statusString, bool *ok)
{
    statusString = statusString.toLower();
    if (ok != nullptr)
        *ok = WORKFLOW_STATUSES.contains(statusString);
    return WORKFLOW_STATUSES.value(statusString, WorkflowStatus::UnknownStatus);
}

QString ApiHelper::transitionEventToString(ApiHelper::TransitionEvent event)
{
    return TRANSITION_EVENTS.key(event, "");
}

ApiHelper::TransitionEvent ApiHelper::transitionEventFromString(QString eventString, bool *ok)
{
    eventString = eventString.toLower();
    if (ok != nullptr)
        *ok = TRANSITION_EVENTS.contains(eventString);
    return TRANSITION_EVENTS.value(eventString, TransitionEvent::UnknownEvent);
}

QString ApiHelper::workflowActionToString(ApiHelper::WorkflowAction action)
{
    return WORKFLOW_ACTIONS.key(action, "");
}

ApiHelper::WorkflowAction ApiHelper::workflowActionFromString(QString actionString, bool *ok)
{
    actionString = actionString.toLower();
    if (ok != nullptr)
        *ok = WORKFLOW_ACTIONS.contains(actionString);
    return WORKFLOW_ACTIONS.value(actionString, WorkflowAction::UnknownAction);
}

QString ApiHelper::paperSideToString(ApiHelper::PaperSide side)
{
    return PAPER_SIDES.key(side, "");
}

ApiHelper::PaperSide ApiHelper::paperSideFromString(QString sideString, bool *ok)
{
    sideString = sideString.toLower();
    if (ok != nullptr)
        *ok = PAPER_SIDES.contains(sideString);
    return PAPER_SIDES.value(sideString, PaperSide::NotSetSide);
}

namespace Proof {
namespace Mis {

uint qHash(ApiHelper::WorkflowStatus arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(ApiHelper::WorkflowAction arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(ApiHelper::TransitionEvent arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(ApiHelper::PaperSide arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

}
}
