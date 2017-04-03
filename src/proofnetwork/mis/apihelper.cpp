#include "apihelper.h"

namespace Proof {
namespace Mis {

/*!
 * \class Proof::Mis::ApiHelper apihelper.h "proofnetwork/mis/apihelper.h"
 * Helper class for Mis API
 */

static const QHash<QString, EntityStatus> ENTITY_STATUSES = {
    {"invalid", EntityStatus::InvalidEntity},
    {"not ready", EntityStatus::NotReadyEntity},
    {"valid", EntityStatus::ValidEntity},
    {"deleted", EntityStatus::DeletedEntity}
};

static const QHash<QString, WorkflowStatus> WORKFLOW_STATUSES = {
    {"needs", WorkflowStatus::NeedsStatus},
    {"is ready for", WorkflowStatus::IsReadyForStatus},
    {"in progress", WorkflowStatus::InProgressStatus},
    {"suspended", WorkflowStatus::SuspendedStatus},
    {"done", WorkflowStatus::DoneStatus},
    {"halted", WorkflowStatus::HaltedStatus}
};

static const QHash<QString, WorkflowAction> WORKFLOW_ACTIONS = {
    {"binding", WorkflowAction::BindingAction},
    {"binning", WorkflowAction::BinningAction},
    {"boxing", WorkflowAction::BoxingAction},
    {"blister packing", WorkflowAction::BlisterPackingAction},
    {"color optimizing", WorkflowAction::ColorOptimizingAction},
    {"component boxing", WorkflowAction::ComponentBoxingAction},
    {"container packing", WorkflowAction::ContainerPackingAction},
    {"cutting", WorkflowAction::CuttingAction},
    {"die cutting", WorkflowAction::DieCuttingAction},
    {"distribute", WorkflowAction::DistributeAction},
    {"envelope adding", WorkflowAction::EnvelopeAddingAction},
    {"folder making", WorkflowAction::FolderMakingAction},
    {"folding", WorkflowAction::FoldingAction},
    {"gluing", WorkflowAction::GluingAction},
    {"inspection", WorkflowAction::InspectionAction},
    {"laminating", WorkflowAction::LaminatingAction},
    {"magnetize", WorkflowAction::MagnetizeAction},
    {"mailing", WorkflowAction::MailingAction},
    {"mounting", WorkflowAction::MountingAction},
    {"outsource binding", WorkflowAction::OutsourceBindingAction},
    {"outsource cutting", WorkflowAction::OutsourceCuttingAction},
    {"pdf building", WorkflowAction::PdfBuildingAction},
    {"pdf vt building", WorkflowAction::PdfVtBuildingAction},
    {"plate making", WorkflowAction::PlateMakingAction},
    {"pocket adding", WorkflowAction::PocketAddingAction},
    {"printing", WorkflowAction::PrintingAction},
    {"qc", WorkflowAction::QcAction},
    {"rounding", WorkflowAction::RoundingAction},
    {"scoring", WorkflowAction::ScoringAction},
    {"screen imaging", WorkflowAction::ScreenImagingAction},
    {"screen mounting", WorkflowAction::ScreenMountingAction},
    {"screen preparation", WorkflowAction::ScreenPreparationAction},
    {"screen washing", WorkflowAction::ScreenWashingAction},
    {"ship boxing", WorkflowAction::ShipBoxingAction},
    {"ship label", WorkflowAction::ShipLabelAction},
    {"shipping", WorkflowAction::ShippingAction},
    {"splitting", WorkflowAction::SplittingAction},
    {"staging", WorkflowAction::StagingAction},
    {"stuffing", WorkflowAction::StuffingAction},
    {"truck loading", WorkflowAction::TruckLoadingAction},
    {"uv coating", WorkflowAction::UvCoatingAction},
    {"uv pdf building", WorkflowAction::UvPdfBuildingAction}
};

static const QHash<QString, TransitionEvent> TRANSITION_EVENTS = {
    {"start", TransitionEvent::StartEvent},
    {"stop", TransitionEvent::StopEvent},
    {"abort", TransitionEvent::AbortEvent},
    {"suspend", TransitionEvent::SuspendEvent},
    {"resume", TransitionEvent::ResumeEvent},
    {"perform", TransitionEvent::PerformEvent},
    {"revert", TransitionEvent::RevertEvent},
    {"request", TransitionEvent::RequestEvent}
};

static const QHash<QString, PaperSide> PAPER_SIDES = {
    {"", PaperSide::NotSetSide},
    {"front", PaperSide::FrontSide},
    {"back", PaperSide::BackSide}
};

QString entityStatusToString(EntityStatus status)
{
    return ENTITY_STATUSES.key(status, "");
}

QString workflowStatusToString(WorkflowStatus status)
{
    return WORKFLOW_STATUSES.key(status, "");
}

QString workflowActionToString(WorkflowAction action)
{
    return WORKFLOW_ACTIONS.key(action, "");
}

QString transitionEventToString(TransitionEvent event)
{
    return TRANSITION_EVENTS.key(event, "");
}

QString paperSideToString(PaperSide side)
{
    return PAPER_SIDES.key(side, "");
}

EntityStatus entityStatusFromString(QString statusString, bool *ok)
{
    statusString = statusString.toLower();
    if (ok != nullptr)
        *ok = ENTITY_STATUSES.contains(statusString);
    return ENTITY_STATUSES.value(statusString, EntityStatus::InvalidEntity);
}

WorkflowStatus workflowStatusFromString(QString statusString, bool *ok)
{
    statusString = statusString.toLower();
    if (ok != nullptr)
        *ok = WORKFLOW_STATUSES.contains(statusString);
    return WORKFLOW_STATUSES.value(statusString, WorkflowStatus::UnknownStatus);
}

WorkflowAction workflowActionFromString(QString actionString, bool *ok)
{
    actionString = actionString.toLower();
    if (ok != nullptr)
        *ok = WORKFLOW_ACTIONS.contains(actionString);
    return WORKFLOW_ACTIONS.value(actionString, WorkflowAction::UnknownAction);
}

TransitionEvent transitionEventFromString(QString eventString, bool *ok)
{
    eventString = eventString.toLower();
    if (ok != nullptr)
        *ok = TRANSITION_EVENTS.contains(eventString);
    return TRANSITION_EVENTS.value(eventString, TransitionEvent::UnknownEvent);
}

PaperSide paperSideFromString(QString sideString, bool *ok)
{
    sideString = sideString.toLower();
    if (ok != nullptr)
        *ok = PAPER_SIDES.contains(sideString);
    return PAPER_SIDES.value(sideString, PaperSide::NotSetSide);
}

WorkflowStatus workflowStatusAfterTransitionEvent(Proof::Mis::TransitionEvent event)
{
    switch (event) {
    case Proof::Mis::TransitionEvent::StartEvent:
        return Proof::Mis::WorkflowStatus::InProgressStatus;
    case Proof::Mis::TransitionEvent::StopEvent:
        return Proof::Mis::WorkflowStatus::DoneStatus;
    case Proof::Mis::TransitionEvent::AbortEvent:
    case Proof::Mis::TransitionEvent::RevertEvent:
        return Proof::Mis::WorkflowStatus::IsReadyForStatus;
    case Proof::Mis::TransitionEvent::SuspendEvent:
        return Proof::Mis::WorkflowStatus::SuspendedStatus;
    case Proof::Mis::TransitionEvent::ResumeEvent:
        return Proof::Mis::WorkflowStatus::InProgressStatus;
    default:
        return Proof::Mis::WorkflowStatus::UnknownStatus;
    }
}

uint qHash(WorkflowAction arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(TransitionEvent arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(WorkflowStatus arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(PaperSide arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint qHash(EntityStatus arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

}
}
