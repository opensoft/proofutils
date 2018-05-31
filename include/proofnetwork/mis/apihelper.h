#ifndef MISAPIHELPER_H
#define MISAPIHELPER_H

#include "proofcore/proofobject.h"

#include "proofnetwork/mis/proofnetworkmis_global.h"

namespace Proof {
namespace Mis {
PROOF_NETWORK_MIS_EXPORT Q_NAMESPACE;

enum class EntityStatus
{
    NotReadyEntity,
    ValidEntity,
    DeletedEntity,
    InvalidEntity
};

enum class WorkflowStatus
{
    NeedsStatus,
    IsReadyForStatus,
    InProgressStatus,
    SuspendedStatus,
    DoneStatus,
    HaltedStatus,
    UnknownStatus
};

enum class WorkflowAction
{
    BindingAction,
    BinningAction,
    BlisterPackingAction,
    BoxingAction,
    ChipBoardingAction,
    ClampingAction,
    ColorOptimizingAction,
    ComponentBoxingAction,
    ContainerPackingAction,
    CuttingAction,
    DieCuttingAction,
    DistributeAction,
    EnvelopeAddingAction,
    FolderMakingAction,
    FoldingAction,
    GluingAction,
    InspectionAction,
    LaminatingAction,
    MagnetizeAction,
    MailingAction,
    MountingAction,
    OutsourceBindingAction,
    OutsourceCuttingAction,
    PdfBuildingAction,
    PdfVtBuildingAction,
    PlateMakingAction,
    PocketAddingAction,
    PrintingAction,
    QcAction,
    RoundingAction,
    ScoringAction,
    ScreenImagingAction,
    ScreenMountingAction,
    ScreenPreparationAction,
    ScreenWashingAction,
    ShipBoxingAction,
    ShipLabelAction,
    ShippingAction,
    SplittingAction,
    StagingAction,
    StrippingAction,
    StuffingAction,
    TruckLoadingAction,
    UvCoatingAction,
    UvPdfBuildingAction,
    UnknownAction
};

enum class TransitionEvent
{
    StartEvent,
    StopEvent,
    AbortEvent,
    SuspendEvent,
    ResumeEvent,
    PerformEvent,
    RevertEvent,
    RequestEvent,
    UnknownEvent
};

enum class PaperSide
{
    NotSetSide,
    FrontSide,
    BackSide
};

Q_ENUM_NS(EntityStatus)
Q_ENUM_NS(WorkflowStatus)
Q_ENUM_NS(WorkflowAction)
Q_ENUM_NS(TransitionEvent)
Q_ENUM_NS(PaperSide)

PROOF_NETWORK_MIS_EXPORT QString entityStatusToString(EntityStatus status);
PROOF_NETWORK_MIS_EXPORT QString workflowStatusToString(WorkflowStatus status);
PROOF_NETWORK_MIS_EXPORT QString workflowActionToString(WorkflowAction action);
PROOF_NETWORK_MIS_EXPORT QString transitionEventToString(TransitionEvent event);
PROOF_NETWORK_MIS_EXPORT QString paperSideToString(PaperSide side);

PROOF_NETWORK_MIS_EXPORT EntityStatus entityStatusFromString(QString statusString, bool *ok = nullptr);
PROOF_NETWORK_MIS_EXPORT WorkflowStatus workflowStatusFromString(QString statusString, bool *ok = nullptr);
PROOF_NETWORK_MIS_EXPORT WorkflowAction workflowActionFromString(QString actionString, bool *ok = nullptr);
PROOF_NETWORK_MIS_EXPORT TransitionEvent transitionEventFromString(QString eventString, bool *ok = nullptr);
PROOF_NETWORK_MIS_EXPORT PaperSide paperSideFromString(QString sideString, bool *ok = nullptr);

PROOF_NETWORK_MIS_EXPORT WorkflowStatus workflowStatusAfterTransitionEvent(Proof::Mis::TransitionEvent event);

PROOF_NETWORK_MIS_EXPORT uint qHash(EntityStatus arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(WorkflowAction arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(TransitionEvent arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(WorkflowStatus arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(PaperSide arg, uint seed = 0);

} // namespace Mis
} // namespace Proof

#endif // MISAPIHELPER_H
