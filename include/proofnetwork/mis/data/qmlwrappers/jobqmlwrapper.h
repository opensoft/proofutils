#ifndef PROOF_MIS_JOBQMLWRAPPER_H
#define PROOF_MIS_JOBQMLWRAPPER_H

#include "proofnetwork/qmlwrappers/networkdataentityqmlwrapper.h"
#include "proofnetwork/mis/proofnetworkmis_types.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"
#include "proofnetwork/mis/apihelper.h"

namespace Proof {
namespace Mis {

class Job;

class JobQmlWrapperPrivate;
class PROOF_NETWORK_MIS_EXPORT JobQmlWrapper : public NetworkDataEntityQmlWrapper
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(qlonglong quantity READ quantity NOTIFY quantityChanged)
    Q_DECLARE_PRIVATE(JobQmlWrapper)
public:
    explicit JobQmlWrapper(const JobSP &job, QObject *parent = 0);
    ~JobQmlWrapper();
    PROOF_NDE_WRAPPER_TOOLS(Job)

    QString id() const;
    QString name() const;
    qlonglong quantity() const;
    Q_INVOKABLE ApiHelper::WorkflowStatus workflowStatus(ApiHelper::WorkflowAction action,
                                                         ApiHelper::PaperSide paperSide = ApiHelper::PaperSide::NotSetSide) const;

signals:
    void idChanged(const QString &arg);
    void nameChanged(const QString &arg);
    void quantityChanged(qlonglong arg);
    void workflowChanged();

protected:
    void setupEntity(const QSharedPointer<NetworkDataEntity> &old = QSharedPointer<NetworkDataEntity>()) override;

};

}
}

#endif // PROOF_MIS_JOBQMLWRAPPER_H
