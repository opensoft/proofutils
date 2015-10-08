#ifndef PROOF_MIS_JOB_H
#define PROOF_MIS_JOB_H

#include "proofnetwork/networkdataentity.h"
#include "proofcore/objectscache.h"
#include "proofnetwork/mis/proofnetworkmis_types.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"
#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h"

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
    ApiHelper::WorkflowStatus workflowStatus() const;

    void setName(const QString &name);
    void setQuantity(qlonglong quantity);
    void setWorkflowStatus(ApiHelper::WorkflowStatus workflowStatus);

    JobQmlWrapper *toQmlWrapper(QObject *parent = 0) const override;

    static JobSP create(const QString &id = QString());

signals:
    void idChanged(const QString &arg);
    void nameChanged(const QString &arg);
    void quantityChanged(qlonglong arg);
    void workflowStatusChanged(Proof::Mis::ApiHelper::WorkflowStatus arg);

protected:
    explicit Job(const QString &id);

};

PROOF_NETWORK_MIS_EXPORT ObjectsCache<QString, Job> &jobsCache();

}
}

#endif // PROOF_MIS_JOB_H
