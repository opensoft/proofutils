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
    bool hasPreview() const;
    void setWorkflowStatus(WorkflowAction action, WorkflowStatus status, PaperSide paperSide = PaperSide::NotSetSide);
    WorkflowStatus workflowStatus(WorkflowAction action, PaperSide paperSide = PaperSide::NotSetSide) const;

    void setStatus(EntityStatus status);
    void setName(const QString &name);
    void setQuantity(qlonglong quantity);
    void setWidth(double width);
    void setHeight(double height);
    void setSource(const QString &source);
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
