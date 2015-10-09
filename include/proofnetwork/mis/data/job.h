#ifndef PROOF_MIS_JOB_H
#define PROOF_MIS_JOB_H

#include "proofnetwork/networkdataentity.h"
#include "proofcore/objectscache.h"
#include "proofnetwork/mis/proofnetworkmis_types.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"
#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h"
#include "proofnetwork/mis/data/workflowelement.h"

namespace Proof {
namespace Mis {

class JobPrivate;
class PROOF_NETWORK_MIS_EXPORT Job : public NetworkDataEntity
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Job)
public:
    QString id() const;
    QString name() const;
    qlonglong quantity() const;
    void setWorkflowStatus(ApiHelper::WorkflowAction action,
                           ApiHelper::WorkflowStatus status,
                           ApiHelper::PaperSide paperSide = ApiHelper::PaperSide::NotSetSide);
    ApiHelper::WorkflowStatus workflowStatus(ApiHelper::WorkflowAction action,
                                             ApiHelper::PaperSide paperSide = ApiHelper::PaperSide::NotSetSide) const;

    void setName(const QString &name);
    void setQuantity(qlonglong quantity);
    void setWorkflow(const QList<WorkflowElement> &workflow);

    JobQmlWrapper *toQmlWrapper(QObject *parent = 0) const override;

    static JobSP create(const QString &id = QString());

signals:
    void idChanged(const QString &arg);
    void nameChanged(const QString &arg);
    void quantityChanged(qlonglong arg);
    void workflowChanged();

protected:
    explicit Job(const QString &id);

};

PROOF_NETWORK_MIS_EXPORT ObjectsCache<QString, Job> &jobsCache();

}
}

#endif // PROOF_MIS_JOB_H
