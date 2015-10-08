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

    static QString workflowStatusToString(WorkflowStatus status);
    static WorkflowStatus workflowStatusFromString(const QString &statusString, bool *ok = nullptr);
    static QString transitionEventToString(TransitionEvent event);
    static TransitionEvent transitionEventFromString(const QString &eventString, bool *ok = nullptr);

private:
    explicit ApiHelper() : ProofObject(0) {}
};

PROOF_NETWORK_MIS_EXPORT uint qHash(ApiHelper::WorkflowStatus arg, uint seed = 0);
PROOF_NETWORK_MIS_EXPORT uint qHash(ApiHelper::TransitionEvent arg, uint seed = 0);

}
}

#endif // MISAPIHELPER_H
