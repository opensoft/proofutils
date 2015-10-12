#include "workflowelement.h"

#include <QRegExp>

namespace Proof {
namespace Mis {

class WorkflowElementPrivate
{
    Q_DECLARE_PUBLIC(WorkflowElement)
    WorkflowElement *q_ptr;

    ApiHelper::WorkflowAction action = ApiHelper::WorkflowAction::UnknownAction;
    ApiHelper::WorkflowStatus status = ApiHelper::WorkflowStatus::UnknownStatus;
    ApiHelper::PaperSide paperSide = ApiHelper::PaperSide::NotSetSide;
};
}
}

using namespace Proof;
using namespace Proof::Mis;

WorkflowElement::WorkflowElement()
    : d_ptr(new WorkflowElementPrivate)
{
    d_ptr->q_ptr = this;
}

WorkflowElement::WorkflowElement(ApiHelper::WorkflowAction action, ApiHelper::WorkflowStatus status, ApiHelper::PaperSide paperSide)
    : d_ptr(new WorkflowElementPrivate)
{
    d_ptr->q_ptr = this;
    d_ptr->action = action;
    d_ptr->status = status;
    d_ptr->paperSide = paperSide;
}

WorkflowElement::WorkflowElement(const WorkflowElement &other)
    : d_ptr(new WorkflowElementPrivate)
{
    d_ptr->q_ptr = this;

    d_ptr->action = other.action();
    d_ptr->paperSide = other.paperSide();
    d_ptr->status = other.status();
}

WorkflowElement::WorkflowElement(WorkflowElement &&other)
    : d_ptr(other.d_ptr.take())
{
}

WorkflowElement::~WorkflowElement()
{
}

ApiHelper::WorkflowAction WorkflowElement::action() const
{
    return d_ptr->action;
}

void WorkflowElement::setAction(ApiHelper::WorkflowAction arg)
{
    d_ptr->action = arg;
}

ApiHelper::WorkflowStatus WorkflowElement::status() const
{
    return d_ptr->status;
}

void WorkflowElement::setStatus(ApiHelper::WorkflowStatus arg)
{
    d_ptr->status = arg;
}

ApiHelper::PaperSide WorkflowElement::paperSide() const
{
    return d_ptr->paperSide;
}

void WorkflowElement::setPaperSide(ApiHelper::PaperSide arg)
{
    d_ptr->paperSide = arg;
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
    return (d_ptr->status == other.d_ptr->status
            && d_ptr->action == other.d_ptr->action
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
}
