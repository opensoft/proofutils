#include "workflowelement.h"

#include <QRegExp>

namespace Proof {
namespace Mis {

class WorkflowElementPrivate
{
    Q_DECLARE_PUBLIC(WorkflowElement)
    WorkflowElement *q_ptr;

    WorkflowAction action = WorkflowAction::UnknownAction;
    WorkflowStatus status = WorkflowStatus::UnknownStatus;
    PaperSide paperSide = PaperSide::NotSetSide;
};
} // namespace Mis
} // namespace Proof

using namespace Proof;
using namespace Proof::Mis;

WorkflowElement::WorkflowElement(const QString &string) : d_ptr(new WorkflowElementPrivate)
{
    d_ptr->q_ptr = this;

    if (!string.isEmpty()) {
        QRegExp re("([^:]*)\\:([^:]*)(?:\\:([^:]*))?");
        if (re.indexIn(string) != -1) {
            d_ptr->status = workflowStatusFromString(re.cap(1));
            d_ptr->action = workflowActionFromString(re.cap(2));
            if (re.captureCount() > 2)
                d_ptr->paperSide = paperSideFromString(re.cap(3));
        }
    }
}

WorkflowElement::WorkflowElement(WorkflowAction action, WorkflowStatus status, PaperSide paperSide)
    : d_ptr(new WorkflowElementPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->action = action;
    d_ptr->status = status;
    d_ptr->paperSide = paperSide;
}

WorkflowElement::WorkflowElement(const WorkflowElement &other) : d_ptr(new WorkflowElementPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->action = other.action();
    d_ptr->paperSide = other.paperSide();
    d_ptr->status = other.status();
}

WorkflowElement::WorkflowElement(WorkflowElement &&other) : d_ptr(other.d_ptr.take())
{
    d_ptr->q_ptr = this;
}

WorkflowElement::~WorkflowElement()
{}

WorkflowAction WorkflowElement::action() const
{
    return d_ptr->action;
}

void WorkflowElement::setAction(WorkflowAction arg)
{
    d_ptr->action = arg;
}

WorkflowStatus WorkflowElement::status() const
{
    return d_ptr->status;
}

void WorkflowElement::setStatus(WorkflowStatus arg)
{
    d_ptr->status = arg;
}

PaperSide WorkflowElement::paperSide() const
{
    return d_ptr->paperSide;
}

void WorkflowElement::setPaperSide(PaperSide arg)
{
    d_ptr->paperSide = arg;
}

QString WorkflowElement::toString() const
{
    return QStringLiteral("%1:%2%3").arg(workflowStatusToString(d_ptr->status), workflowActionToString(d_ptr->action),
                                         d_ptr->paperSide == PaperSide::NotSetSide
                                             ? QLatin1String("")
                                             : QStringLiteral(":%1").arg(paperSideToString(d_ptr->paperSide)));
}

WorkflowElement &WorkflowElement::operator=(const WorkflowElement &other)
{
    d_ptr->action = other.action();
    d_ptr->paperSide = other.paperSide();
    d_ptr->status = other.status();
    return *this;
}

WorkflowElement &WorkflowElement::operator=(WorkflowElement &&other)
{
    d_ptr.reset(other.d_ptr.take());
    return *this;
}

bool WorkflowElement::operator==(const WorkflowElement &other) const
{
    return (d_ptr->status == other.d_ptr->status && d_ptr->action == other.d_ptr->action
            && d_ptr->paperSide == other.d_ptr->paperSide);
}

bool WorkflowElement::operator!=(const WorkflowElement &other) const
{
    return !(*this == other);
}

void WorkflowElement::swap(WorkflowElement &other)
{
    WorkflowElementPrivate *otherPrivate = other.d_ptr.take();
    other.d_ptr.reset(d_ptr.take());
    d_ptr.reset(otherPrivate);
    d_ptr->q_ptr = this;
    other.d_ptr->q_ptr = &other;
}
