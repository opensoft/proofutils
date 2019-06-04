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
#include "proofnetwork/mis/apihelper.h"

template <typename T>
using StringDict = QHash<QString, T>;

namespace Proof {
namespace Mis {

/*!
 * \class Proof::Mis::ApiHelper apihelper.h "proofnetwork/mis/apihelper.h"
 * Helper class for Mis API
 */

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(StringDict<EntityStatus>, ENTITY_STATUSES,
                          ({{"invalid", EntityStatus::InvalidEntity},
                            {"not ready", EntityStatus::NotReadyEntity},
                            {"valid", EntityStatus::ValidEntity},
                            {"deleted", EntityStatus::DeletedEntity}}));

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(StringDict<WorkflowStatus>, WORKFLOW_STATUSES,
                          ({{"needs", WorkflowStatus::NeedsStatus},
                            {"is ready for", WorkflowStatus::IsReadyForStatus},
                            {"in progress", WorkflowStatus::InProgressStatus},
                            {"suspended", WorkflowStatus::SuspendedStatus},
                            {"done", WorkflowStatus::DoneStatus},
                            {"halted", WorkflowStatus::HaltedStatus}}));

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(StringDict<WorkflowAction>, WORKFLOW_ACTIONS,
                          ({{"binding", WorkflowAction::BindingAction},
                            {"binning", WorkflowAction::BinningAction},
                            {"blister packing", WorkflowAction::BlisterPackingAction},
                            {"boxing", WorkflowAction::BoxingAction},
                            {"chip boarding", WorkflowAction::ChipBoardingAction},
                            {"clamping", WorkflowAction::ClampingAction},
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
                            {"stripping", WorkflowAction::StrippingAction},
                            {"stuffing", WorkflowAction::StuffingAction},
                            {"truck loading", WorkflowAction::TruckLoadingAction},
                            {"uv coating", WorkflowAction::UvCoatingAction},
                            {"uv pdf building", WorkflowAction::UvPdfBuildingAction}}));

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(StringDict<TransitionEvent>, TRANSITION_EVENTS,
                          ({{"start", TransitionEvent::StartEvent},
                            {"stop", TransitionEvent::StopEvent},
                            {"abort", TransitionEvent::AbortEvent},
                            {"suspend", TransitionEvent::SuspendEvent},
                            {"resume", TransitionEvent::ResumeEvent},
                            {"perform", TransitionEvent::PerformEvent},
                            {"revert", TransitionEvent::RevertEvent},
                            {"request", TransitionEvent::RequestEvent}}));

// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(StringDict<PaperSide>, PAPER_SIDES,
                          ({{"", PaperSide::NotSetSide}, {"front", PaperSide::FrontSide}, {"back", PaperSide::BackSide}}));

QString entityStatusToString(EntityStatus status)
{
    return ENTITY_STATUSES->key(status, QString());
}

QString workflowStatusToString(WorkflowStatus status)
{
    return WORKFLOW_STATUSES->key(status, QString());
}

QString workflowActionToString(WorkflowAction action)
{
    return WORKFLOW_ACTIONS->key(action, QString());
}

QString transitionEventToString(TransitionEvent event)
{
    return TRANSITION_EVENTS->key(event, QString());
}

QString paperSideToString(PaperSide side)
{
    return PAPER_SIDES->key(side, QString());
}

EntityStatus entityStatusFromString(const QString &statusString)
{
    return ENTITY_STATUSES->value(statusString.toLower(), EntityStatus::InvalidEntity);
}

WorkflowStatus workflowStatusFromString(const QString &statusString)
{
    return WORKFLOW_STATUSES->value(statusString.toLower(), WorkflowStatus::UnknownStatus);
}

WorkflowAction workflowActionFromString(const QString &actionString)
{
    return WORKFLOW_ACTIONS->value(actionString.toLower(), WorkflowAction::UnknownAction);
}

TransitionEvent transitionEventFromString(const QString &eventString)
{
    return TRANSITION_EVENTS->value(eventString, TransitionEvent::UnknownEvent);
}

PaperSide paperSideFromString(const QString &sideString)
{
    return PAPER_SIDES->value(sideString.toLower(), PaperSide::NotSetSide);
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

} // namespace Mis
} // namespace Proof
