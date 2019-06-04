/* Copyright 2018, OpenSoft Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *     * Neither the name of OpenSoft Inc. nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
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

PROOF_NETWORK_MIS_EXPORT EntityStatus entityStatusFromString(const QString &statusString);
PROOF_NETWORK_MIS_EXPORT WorkflowStatus workflowStatusFromString(const QString &statusString);
PROOF_NETWORK_MIS_EXPORT WorkflowAction workflowActionFromString(const QString &actionString);
PROOF_NETWORK_MIS_EXPORT TransitionEvent transitionEventFromString(const QString &eventString);
PROOF_NETWORK_MIS_EXPORT PaperSide paperSideFromString(const QString &sideString);

PROOF_NETWORK_MIS_EXPORT WorkflowStatus workflowStatusAfterTransitionEvent(Proof::Mis::TransitionEvent event);

PROOF_NETWORK_MIS_EXPORT uint qHash(EntityStatus arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(WorkflowAction arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(TransitionEvent arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(WorkflowStatus arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(PaperSide arg, uint seed = 0);

} // namespace Mis
} // namespace Proof

#endif // MISAPIHELPER_H
