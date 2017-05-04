#ifndef PROOF_MIS_WORKFLOWELEMENT_H
#define PROOF_MIS_WORKFLOWELEMENT_H

#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"

namespace Proof {
namespace Mis {

class WorkflowElementPrivate;
class PROOF_NETWORK_MIS_EXPORT WorkflowElement
{
    Q_DECLARE_PRIVATE(WorkflowElement)
public:
    explicit WorkflowElement(const QString &string = QLatin1String(""));
    WorkflowElement(WorkflowAction action, WorkflowStatus status,
                    PaperSide paperSide = PaperSide::NotSetSide);
    WorkflowElement(const WorkflowElement &other);
    WorkflowElement(WorkflowElement &&other);
    ~WorkflowElement();

    WorkflowAction action() const;
    void setAction(WorkflowAction arg);
    WorkflowStatus status() const;
    void setStatus(WorkflowStatus arg);
    PaperSide paperSide() const;
    void setPaperSide(PaperSide arg);

    QString toString() const;

    WorkflowElement &operator=(const WorkflowElement &other);
    WorkflowElement &operator=(WorkflowElement &&other);
    bool operator==(const WorkflowElement &other) const;
    bool operator!=(const WorkflowElement &other) const;
    void swap(WorkflowElement &other);

private:
    QScopedPointer<WorkflowElementPrivate> d_ptr;

};

inline void swap(Proof::Mis::WorkflowElement &lhs, Proof::Mis::WorkflowElement &rhs) { lhs.swap(rhs); }

}
}

#endif // PROOF_MIS_WORKFLOWELEMENT_H
