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
#ifndef PROOF_MIS_JOB_H
#define PROOF_MIS_JOB_H

#include "proofcore/objectscache.h"

#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h"
#include "proofnetwork/mis/data/workflowelement.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"
#include "proofnetwork/mis/proofnetworkmis_types.h"
#include "proofnetwork/networkdataentity.h"

#include <QJsonObject>

namespace Proof {
namespace Mis {

using JobCacheKey = QPair<QString, QString>;

class JobPrivate;
class PROOF_NETWORK_MIS_EXPORT Job : public NetworkDataEntity
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Job)
public:
    QString id() const;
    EntityStatus status() const;
    QString name() const;
    qlonglong quantity() const;
    double width() const;
    double height() const;
    QString source() const;
    int pageCount() const;
    bool hasPreview() const;
    void setWorkflowStatus(WorkflowAction action, WorkflowStatus status, PaperSide paperSide = PaperSide::NotSetSide);
    WorkflowStatus workflowStatus(WorkflowAction action, PaperSide paperSide = PaperSide::NotSetSide) const;

    void setStatus(EntityStatus status);
    void setName(const QString &name);
    void setQuantity(qlonglong quantity);
    void setWidth(double width);
    void setHeight(double height);
    void setSource(const QString &source);
    void setPageCount(int pageCount);
    void setHasPreview(bool hasPreview);
    void setWorkflow(const QVector<WorkflowElement> &workflow);

    JobQmlWrapper *toQmlWrapper(QObject *parent = nullptr) const override;

    QJsonObject toJson() const;

    JobCacheKey cacheKey();

    static JobSP create(const QString &id = QString(), const QString &source = QString());

    static JobSP fromJson(const QJsonObject &json);

signals:
    void idChanged(const QString &arg);
    void statusChanged(Proof::Mis::EntityStatus arg);
    void nameChanged(const QString &arg);
    void quantityChanged(qlonglong arg);
    void widthChanged(double arg);
    void heightChanged(double arg);
    void sourceChanged(const QString &arg);
    void pageCountChanged(int arg);
    void hasPreviewChanged(bool arg);
    void workflowChanged();

protected:
    explicit Job(const QString &id, const QString &source);
    void updateSelf(const Proof::NetworkDataEntitySP &other) override;
};

PROOF_NETWORK_MIS_EXPORT ObjectsCache<JobCacheKey, Job> &jobsCache();

} // namespace Mis
} // namespace Proof

#endif // PROOF_MIS_JOB_H
