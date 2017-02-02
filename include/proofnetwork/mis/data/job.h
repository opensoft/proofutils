#ifndef PROOF_MIS_JOB_H
#define PROOF_MIS_JOB_H

#include "proofnetwork/networkdataentity.h"
#include "proofcore/objectscache.h"
#include "proofnetwork/mis/proofnetworkmis_types.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"
#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h"
#include "proofnetwork/mis/data/workflowelement.h"

#include <QJsonObject>

namespace Proof {
namespace Mis {

typedef QPair<QString, QString> JobCacheKey;

class JobPrivate;
class PROOF_NETWORK_MIS_EXPORT Job : public NetworkDataEntity
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Job)
public:
    QString id() const;
    ApiHelper::EntityStatus status() const;
    QString name() const;
    qlonglong quantity() const;
    double width() const;
    double height() const;
    QString source() const;
    void setWorkflowStatus(ApiHelper::WorkflowAction action,
                           ApiHelper::WorkflowStatus status,
                           ApiHelper::PaperSide paperSide = ApiHelper::PaperSide::NotSetSide);
    ApiHelper::WorkflowStatus workflowStatus(ApiHelper::WorkflowAction action,
                                             ApiHelper::PaperSide paperSide = ApiHelper::PaperSide::NotSetSide) const;

    void setStatus(ApiHelper::EntityStatus status);
    void setName(const QString &name);
    void setQuantity(qlonglong quantity);
    void setWidth(double width);
    void setHeight(double height);
    void setSource(const QString &source);
    void setWorkflow(const QList<WorkflowElement> &workflow);

    JobQmlWrapper *toQmlWrapper(QObject *parent = 0) const override;

    QJsonObject toJson() const;

    JobCacheKey cacheKey();

    static JobSP create(const QString &id = QString(),  const QString &source = QString());

    static JobSP fromJson(const QJsonObject &json);

signals:
    void idChanged(const QString &arg);
    void statusChanged(Proof::Mis::ApiHelper::EntityStatus arg);
    void nameChanged(const QString &arg);
    void quantityChanged(qlonglong arg);
    void widthChanged(double arg);
    void heightChanged(double arg);
    void sourceChanged(const QString &arg);
    void workflowChanged();

protected:
    explicit Job(const QString &id, const QString &source);

};

PROOF_NETWORK_MIS_EXPORT ObjectsCache<JobCacheKey, Job> &jobsCache();

}
}

#endif // PROOF_MIS_JOB_H
