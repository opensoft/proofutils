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
#include "proofnetwork/mis/data/job.h"

#include "proofnetwork/networkdataentity_p.h"

#include <QJsonArray>

namespace Proof {
namespace Mis {

class JobPrivate : NetworkDataEntityPrivate
{
    Q_DECLARE_PUBLIC(Job)

    void setId(const QString &id);

    QString id;
    EntityStatus status = EntityStatus::ValidEntity;
    QString name;
    qlonglong quantity = 0;
    double width = 0.0;
    double height = 0.0;
    QString source;
    int pageCount = 0;
    bool hasPreview = false;
    QVector<WorkflowElement> workflow;
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
    Q_D_CONST(Job);
    return d->id;
}

EntityStatus Job::status() const
{
    Q_D_CONST(Job);
    return d->status;
}

QString Job::name() const
{
    Q_D_CONST(Job);
    return d->name;
}

qlonglong Job::quantity() const
{
    Q_D_CONST(Job);
    return d->quantity;
}

double Job::width() const
{
    Q_D_CONST(Job);
    return d->width;
}

double Job::height() const
{
    Q_D_CONST(Job);
    return d->height;
}

QString Job::source() const
{
    Q_D_CONST(Job);
    return d->source;
}

int Job::pageCount() const
{
    Q_D_CONST(Job);
    return d->pageCount;
}

bool Job::hasPreview() const
{
    Q_D_CONST(Job);
    return d->hasPreview;
}

void JobPrivate::setId(const QString &arg)
{
    Q_Q(Job);
    if (id != arg) {
        id = arg;
        emit q->idChanged(arg);
    }
}

void Job::setStatus(EntityStatus status)
{
    Q_D(Job);
    if (d->status != status) {
        d->status = status;
        emit statusChanged(status);
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

void Job::setPageCount(int pageCount)
{
    Q_D(Job);
    if (d->pageCount != pageCount) {
        d->pageCount = pageCount;
        emit pageCountChanged(pageCount);
    }
}

void Job::setHasPreview(bool hasPreview)
{
    Q_D(Job);
    if (d->hasPreview != hasPreview) {
        d->hasPreview = hasPreview;
        emit hasPreviewChanged(hasPreview);
    }
}

void Job::setWorkflow(const QVector<WorkflowElement> &arg)
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

void Job::setWorkflowStatus(WorkflowAction action, WorkflowStatus status, PaperSide paperSide)
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

WorkflowStatus Job::workflowStatus(WorkflowAction action, PaperSide paperSide) const
{
    Q_D_CONST(Job);
    WorkflowStatus fallbackStatus = WorkflowStatus::UnknownStatus;
    for (const auto &element : qAsConst(d->workflow)) {
        if (element.action() != action)
            continue;
        if (element.paperSide() == paperSide) {
            return element.status();
        } else if (paperSide == PaperSide::NotSetSide) {
            if (fallbackStatus == WorkflowStatus::UnknownStatus)
                fallbackStatus = element.status();
            else if ((element.status() == WorkflowStatus::SuspendedStatus
                      || fallbackStatus == WorkflowStatus::SuspendedStatus))
                fallbackStatus = WorkflowStatus::SuspendedStatus;
            else if ((element.status() == WorkflowStatus::InProgressStatus
                      || fallbackStatus == WorkflowStatus::InProgressStatus))
                fallbackStatus = WorkflowStatus::InProgressStatus;
            else if ((element.status() == WorkflowStatus::IsReadyForStatus
                      || fallbackStatus == WorkflowStatus::IsReadyForStatus))
                fallbackStatus = WorkflowStatus::IsReadyForStatus;
            else if ((element.status() == WorkflowStatus::NeedsStatus || fallbackStatus == WorkflowStatus::NeedsStatus))
                fallbackStatus = WorkflowStatus::NeedsStatus;
        } else if (element.paperSide() == PaperSide::NotSetSide) {
            if (paperSide == PaperSide::FrontSide && fallbackStatus == WorkflowStatus::UnknownStatus)
                fallbackStatus = element.status();
        }
    }
    return fallbackStatus;
}

JobQmlWrapper *Job::toQmlWrapper(QObject *parent) const
{
    JobSP castedSelf = castedSelfPtr<Job>();
    Q_ASSERT(castedSelf);
    return new JobQmlWrapper(castedSelf, parent);
}

QJsonObject Job::toJson() const
{
    Q_D_CONST(Job);
    QJsonObject json;
    json.insert(QStringLiteral("id"), id());
    json.insert(QStringLiteral("status"), entityStatusToString(status()));
    json.insert(QStringLiteral("name"), name());
    json.insert(QStringLiteral("quantity"), quantity());
    json.insert(QStringLiteral("width"), width());
    json.insert(QStringLiteral("height"), height());
    json.insert(QStringLiteral("source"), source());
    json.insert(QStringLiteral("page_count"), pageCount());
    json.insert(QStringLiteral("has_preview"), hasPreview());

    QJsonArray workflowArray;
    for (const auto &workflowElement : qAsConst(d->workflow))
        workflowArray << workflowElement.toString();

    json.insert(QStringLiteral("workflow"), workflowArray);
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
    if (!json.contains(QLatin1String("id")))
        return JobSP();

    QString id = json.value(QStringLiteral("id")).toString();
    JobSP job = create(id);
    job->setFetched(true);
    job->setStatus(entityStatusFromString(json.value(QStringLiteral("status")).toString(QStringLiteral("valid"))));
    job->setName(json.value(QStringLiteral("name")).toString());
    job->setQuantity(json.value(QStringLiteral("quantity")).toInt());
    job->setWidth(json.value(QStringLiteral("width")).toDouble());
    job->setHeight(json.value(QStringLiteral("height")).toDouble());
    job->setSource(json.value(QStringLiteral("source")).toString());
    job->setPageCount(json.value(QStringLiteral("page_count")).toInt());
    job->setHasPreview(json.value(QStringLiteral("has_preview")).toBool());
    job->setWorkflow(algorithms::map(json.value(QStringLiteral("workflow")).toArray(),
                                     [](const auto &value) { return WorkflowElement(value.toString()); },
                                     QVector<WorkflowElement>()));
    return job;
}

Job::Job(const QString &id, const QString &source) : NetworkDataEntity(*new JobPrivate)
{
    Q_D(Job);
    d->setId(id);
    setSource(source);
    setName(id);
}

void Job::updateSelf(const Proof::NetworkDataEntitySP &other)
{
    Q_D(Job);
    JobSP castedOther = qSharedPointerCast<Job>(other);
    Q_ASSERT(castedOther);
    d->setId(castedOther->id());
    setName(castedOther->name());
    setStatus(castedOther->status());
    setQuantity(castedOther->quantity());
    setWidth(castedOther->width());
    setHeight(castedOther->height());
    setSource(castedOther->source());
    setWorkflow(castedOther->d_func()->workflow);
    setPageCount(castedOther->pageCount());
    setHasPreview(castedOther->hasPreview());

    NetworkDataEntity::updateSelf(other);
}
