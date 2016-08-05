#include "job.h"

#include "proofnetwork/networkdataentity_p.h"

#include <QJsonArray>

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
    double width = 0.0;
    double height = 0.0;
    QString source;
    QList<WorkflowElement> workflow;
};

ObjectsCache<JobCacheKey, Job> &jobsCache()
{
    return WeakObjectsCache<JobCacheKey, Job>::instance();
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

double Job::width() const
{
    Q_D(const Job);
    return d->width;
}

double Job::height() const
{
    Q_D(const Job);
    return d->height;
}

QString Job::source() const
{
    Q_D(const Job);
    return d->source;
}

void JobPrivate::setId(const QString &arg)
{
    Q_Q(Job);
    if (id != arg) {
        id = arg;
        emit q->idChanged(arg);
    }
}

void Job::setName(const QString &name)
{
    Q_D(Job);
    if (d->name != name) {
        d->name = name;
        emit nameChanged(name);
    }
}

void Job::setQuantity(qlonglong quantity)
{
    Q_D(Job);
    if (d->quantity != quantity) {
        d->quantity = quantity;
        emit quantityChanged(quantity);
    }
}

void Job::setWidth(double width)
{
    Q_D(Job);
    if (!qFuzzyCompare(d->width + 1.0, width + 1.0)) {
        d->width = width;
        emit widthChanged(width);
    }
}

void Job::setHeight(double height)
{
    Q_D(Job);
    if (!qFuzzyCompare(d->height + 1.0, height + 1.0)) {
        d->height = height;
        emit heightChanged(height);
    }
}

void Job::setSource(const QString &source)
{
    Q_D(Job);
    if (d->source != source) {
        d->source = source;
        emit sourceChanged(source);
    }
}

void Job::setWorkflow(const QList<WorkflowElement> &arg)
{
    Q_D(Job);
    bool emitNeeded = arg.count() != d->workflow.count();
    for (int i = 0; i < arg.count() && !emitNeeded; ++i)
        emitNeeded = arg[i] != d->workflow[i];
    if (emitNeeded) {
        d->workflow = arg;
        emit workflowChanged();
    }
}

void Job::setWorkflowStatus(ApiHelper::WorkflowAction action,
                            ApiHelper::WorkflowStatus status,
                            ApiHelper::PaperSide paperSide)
{
    Q_D(Job);
    bool found = false;
    for (auto &element : d->workflow) {
        if (element.action() == action && element.paperSide() == paperSide) {
            element.setStatus(status);
            found = true;
            break;
        }
    }
    if (!found)
        d->workflow.append(WorkflowElement(action, status, paperSide));
}

ApiHelper::WorkflowStatus Job::workflowStatus(ApiHelper::WorkflowAction action, ApiHelper::PaperSide paperSide) const
{
    Q_D(const Job);
    ApiHelper::WorkflowStatus fallbackStatus = ApiHelper::WorkflowStatus::UnknownStatus;
    foreach (const WorkflowElement &element, d->workflow) {
        if (element.action() != action)
            continue;
        if (element.paperSide() == paperSide) {
            return element.status();
        } else if (paperSide == ApiHelper::PaperSide::NotSetSide) {
            if (fallbackStatus == ApiHelper::WorkflowStatus::UnknownStatus)
                fallbackStatus = element.status();
            else if ((element.status() == ApiHelper::WorkflowStatus::SuspendedStatus || fallbackStatus == ApiHelper::WorkflowStatus::SuspendedStatus))
                fallbackStatus = ApiHelper::WorkflowStatus::SuspendedStatus;
            else if ((element.status() == ApiHelper::WorkflowStatus::InProgressStatus || fallbackStatus == ApiHelper::WorkflowStatus::InProgressStatus))
                fallbackStatus = ApiHelper::WorkflowStatus::InProgressStatus;
            else if ((element.status() == ApiHelper::WorkflowStatus::IsReadyForStatus || fallbackStatus == ApiHelper::WorkflowStatus::IsReadyForStatus))
                fallbackStatus = ApiHelper::WorkflowStatus::IsReadyForStatus;
            else if ((element.status() == ApiHelper::WorkflowStatus::NeedsStatus || fallbackStatus == ApiHelper::WorkflowStatus::NeedsStatus))
                fallbackStatus = ApiHelper::WorkflowStatus::NeedsStatus;
        } else if (element.paperSide() == ApiHelper::PaperSide::NotSetSide) {
            if (paperSide == ApiHelper::PaperSide::FrontSide && fallbackStatus == ApiHelper::WorkflowStatus::UnknownStatus)
                fallbackStatus = element.status();
        }
    }
    return fallbackStatus;
}

JobQmlWrapper *Job::toQmlWrapper(QObject *parent) const
{
    Q_D(const Job);
    JobSP castedSelf = qSharedPointerCast<Job>(d->weakSelf);
    Q_ASSERT(castedSelf);
    return new JobQmlWrapper(castedSelf, parent);
}

QJsonObject Job::toJson() const
{
    Q_D(const Job);
    QJsonObject json;
    json.insert("id", id());
    json.insert("name", name());
    json.insert("quantity", quantity());
    json.insert("width", width());
    json.insert("height", height());
    json.insert("source", source());

    QJsonArray workflowArray;

    for(const auto &workflowElement : d->workflow)
        workflowArray << workflowElement.toString();

    json.insert("workflow", workflowArray);
    return json;
}

JobCacheKey Job::cacheKey()
{
    return JobCacheKey(id(), source());
}

JobSP Job::create(const QString &id, const QString &source)
{
    JobSP result(new Job(id, source));
    initSelfWeakPtr(result);
    return result;
}

JobSP Job::fromJson(const QJsonObject &json)
{
    if (!json.contains("id"))
        return JobSP();

    QString id = json.value("id").toString("");
    JobSP job = create(id);
    job->setFetched(true);
    job->setName(json.value("name").toString(""));
    job->setQuantity(json.value("quantity").toInt());
    job->setWidth(json.value("width").toDouble());
    job->setHeight(json.value("height").toDouble());
    job->setSource(json.value("source").toString(""));

    QList<WorkflowElement> workflow;

    QJsonArray workflowArray = json.value("workflow").toArray();

    for (const auto &elementValue : workflowArray)
        workflow << WorkflowElement(elementValue.toString());

    job->setWorkflow(workflow);
    return job;
}

Job::Job(const QString &id, const QString &source)
    : NetworkDataEntity(*new JobPrivate)
{
    Q_D(Job);
    d->setId(id);
    setSource(source);
    setName(id);
}

void JobPrivate::updateFrom(const NetworkDataEntitySP &other)
{
    Q_Q(Job);
    JobSP castedOther = qSharedPointerCast<Job>(other);
    Q_ASSERT(castedOther);
    setId(castedOther->id());
    q->setName(castedOther->name());
    q->setQuantity(castedOther->quantity());
    q->setWidth(castedOther->width());
    q->setHeight(castedOther->height());
    q->setSource(castedOther->source());
    q->setWorkflow(castedOther->d_func()->workflow);

    NetworkDataEntityPrivate::updateFrom(other);
}

