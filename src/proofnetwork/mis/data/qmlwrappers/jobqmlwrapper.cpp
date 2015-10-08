#include "jobqmlwrapper.h"

#include "proofnetwork/mis/data/job.h"
#include "proofnetwork/qmlwrappers/networkdataentityqmlwrapper_p.h"
#include "proofnetwork/jdf/data/qmlwrappers/bundleitemqmlwrapper.h"

namespace Proof {
namespace Mis {

class JobQmlWrapperPrivate : public NetworkDataEntityQmlWrapperPrivate
{
    Q_DECLARE_PUBLIC(JobQmlWrapper)

};

JobQmlWrapper::JobQmlWrapper(const JobSP &job, QObject *parent)
    : NetworkDataEntityQmlWrapper(job, *new JobQmlWrapperPrivate, parent)
{
    setupEntity();
}

JobQmlWrapper::~JobQmlWrapper()
{
}

PROOF_NDE_WRAPPER_TOOLS_IMPL(Job)

PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Job, QString, id)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Job, QString, name)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Job, qlonglong, quantity)
PROOF_NDE_WRAPPER_PROPERTY_IMPL_R(Job, ApiHelper::WorkflowStatus, workflowStatus)


void JobQmlWrapper::setupEntity(const QSharedPointer<NetworkDataEntity> &old)
{
    Q_D(JobQmlWrapper);
    JobSP job = d->entity<Job>();
    Q_ASSERT(job);

    connect(job.data(), &Job::idChanged, this, &JobQmlWrapper::idChanged);
    connect(job.data(), &Job::nameChanged, this, &JobQmlWrapper::nameChanged);
    connect(job.data(), &Job::quantityChanged, this, &JobQmlWrapper::quantityChanged);
    connect(job.data(), &Job::workflowStatusChanged, this, &JobQmlWrapper::workflowStatusChanged);

    JobSP oldJob = qSharedPointerCast<Job>(old);
    if (oldJob) {
        if (job->id() != oldJob->id())
            emit idChanged(job->id());
        if (job->name() != oldJob->name())
            emit nameChanged(job->name());
        if (job->quantity() != oldJob->quantity())
            emit quantityChanged(job->quantity());
        if (job->workflowStatus() != oldJob->workflowStatus())
            emit workflowStatusChanged(job->workflowStatus());
    }

}

}
}


