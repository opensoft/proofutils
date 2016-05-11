#ifndef MISAPIHELPER_H
#define MISAPIHELPER_H

#include "proofcore/proofobject.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"

namespace Proof {
namespace Mis {

class PROOF_NETWORK_MIS_EXPORT ApiHelper : public ProofObject
{
    Q_OBJECT
    Q_ENUMS(WorkflowStatus)
    Q_ENUMS(TransitionEvent)
    Q_ENUMS(WorkflowAction)
    Q_ENUMS(PaperSide)
public:
    enum class WorkflowStatus {
        NeedsStatus,
        IsReadyForStatus,
        InProgressStatus,
        SuspendedStatus,
        DoneStatus,
        HaltedStatus,
        UnknownStatus
    };

    enum class WorkflowAction {
        BindingAction,
        BinningAction,
        BoxingAction,
        ColorOptimizingAction,
        ComponentBoxingAction,
        ContainerPackingAction,
        CuttingAction,
        DistributeAction,
        FolderMakingAction,
        MagnetizeAction,
        MailingAction,
        MountingAction,
        OutsourceCuttingAction,
        PdfBuildingAction,
        PlateMakingAction,
        PrintingAction,
        QcAction,
        RoundingAction,
        ScreenImagingAction,
        ScreenMountingAction,
        ScreenPreparationAction,
        ScreenWashingAction,
        ShipBoxingAction,
        ShipLabelAction,
        ShippingAction,
        StagingAction,
        TruckLoadingAction,
        UvCoatingAction,
        UvPdfBuildingAction,
        UnknownAction
    };

    enum class TransitionEvent {
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

    enum class PaperSide {
        NotSetSide,
        FrontSide,
        BackSide
    };

// TODO: 1.0: think about proper abstract job interface
    static QString workflowStatusToString(WorkflowStatus status);
    static QString workflowActionToString(WorkflowAction action);
    static QString transitionEventToString(TransitionEvent event);
    static QString paperSideToString(PaperSide side);

    static WorkflowStatus workflowStatusFromString(QString statusString, bool *ok = nullptr);
    static WorkflowAction workflowActionFromString(QString actionString, bool *ok = nullptr);
    static TransitionEvent transitionEventFromString(QString eventString, bool *ok = nullptr);
    static PaperSide paperSideFromString(QString sideString, bool *ok = nullptr);

    static WorkflowStatus workflowStatusAfterTransitionEvent(Proof::Mis::ApiHelper::TransitionEvent event);

private:
    explicit ApiHelper() : ProofObject(0) {}
};

PROOF_NETWORK_MIS_EXPORT uint qHash(ApiHelper::WorkflowAction arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(ApiHelper::TransitionEvent arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(ApiHelper::WorkflowStatus arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(ApiHelper::PaperSide arg, uint seed = 0);

}
}

#endif // MISAPIHELPER_H
