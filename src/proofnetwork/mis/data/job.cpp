#include "job.h"

#include "proofnetwork/networkdataentity_p.h"

namespace Proof {
namespace Mis {

class JobPrivate : NetworkDataEntityPrivate
{
    Q_DECLARE_PUBLIC(Job)

    void updateFrom(const Proof::NetworkDataEntitySP &other) override;

    void setId(const QString &id);

    QString id;
    QString name;
    qlonglong quantity = 0;
    ApiHelper::WorkflowStatus workflowStatus = ApiHelper::WorkflowStatus::NeedsStatus;
};

ObjectsCache<QString, Job> &jobsCache()
{
    return WeakObjectsCache<QString, Job>::instance();
}

} // namespace Mis
} // namespace Proof

using namespace Proof::Mis;

QString Job::id() const
{
    Q_D(const Job);
    return d->id;
}

QString Job::name() const
{
    Q_D(const Job);
    return d->name;
}

qlonglong Job::quantity() const
{
    Q_D(const Job);
    return d->quantity;
}

void JobPrivate::setId(const QString &arg)
{
    Q_Q(Job);
    if (id != arg) {
        id = arg;
        emit q->idChanged(arg);
    }
}

void Job::setName(const QString &arg)
{
    Q_D(Job);
    if (d->name != arg) {
        d->name = arg;
        emit nameChanged(arg);
    }
}

void Job::setQuantity(qlonglong arg)
{
    Q_D(Job);
    if (d->quantity != arg) {
        d->quantity = arg;
        emit quantityChanged(arg);
    }
}

void Job::setWorkflowStatus(ApiHelper::WorkflowStatus arg)
{
    Q_D(Job);
    if (d->workflowStatus != arg) {
        d->workflowStatus = arg;
        emit workflowStatusChanged(arg);
    }
}

ApiHelper::WorkflowStatus Job::workflowStatus() const
{
    Q_D(const Job);
    return d->workflowStatus;
}

JobQmlWrapper *Job::toQmlWrapper(QObject *parent) const
{
    Q_D(const Job);
    JobSP castedSelf = qSharedPointerCast<Job>(d->weakSelf);
    Q_ASSERT(castedSelf);
    return new JobQmlWrapper(castedSelf, parent);
}

JobSP Job::create(const QString &id)
{
    JobSP result(new Job(id));
    initSelfWeakPtr(result);
    return result;
}

Job::Job(const QString &id)
    : NetworkDataEntity(*new JobPrivate)
{
    Q_D(Job);
    d->setId(id);
    setName(id);
}

void JobPrivate::updateFrom(const NetworkDataEntitySP &other)
{
    Q_Q(Job);
    JobSP castedOther = qSharedPointerCast<Job>(other);

    setId(castedOther->id());
    q->setName(castedOther->name());
    q->setQuantity(castedOther->quantity());
    q->setWorkflowStatus(castedOther->workflowStatus());

    NetworkDataEntityPrivate::updateFrom(other);
}

