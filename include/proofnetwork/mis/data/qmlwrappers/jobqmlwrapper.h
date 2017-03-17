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
    Q_PROPERTY(Proof::Mis::ApiHelper::EntityStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(qlonglong quantity READ quantity NOTIFY quantityChanged)
    Q_PROPERTY(double width READ width NOTIFY widthChanged)
    Q_PROPERTY(double height READ height NOTIFY heightChanged)
    Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
    Q_DECLARE_PRIVATE(JobQmlWrapper)
public:
    explicit JobQmlWrapper(const JobSP &job, QObject *parent = nullptr);
    ~JobQmlWrapper();
    PROOF_NDE_WRAPPER_TOOLS(Job)

    QString id() const;
    Proof::Mis::ApiHelper::EntityStatus status() const;
    QString name() const;
    qlonglong quantity() const;
    double width() const;
    double height() const;
    QString source() const;
    Q_INVOKABLE Proof::Mis::ApiHelper::WorkflowStatus workflowStatus(Proof::Mis::ApiHelper::WorkflowAction action,
                                                                     Proof::Mis::ApiHelper::PaperSide paperSide = Proof::Mis::ApiHelper::PaperSide::NotSetSide) const;

signals:
    void idChanged(const QString &id);
    void statusChanged(Proof::Mis::ApiHelper::EntityStatus status);
    void nameChanged(const QString &name);
    void quantityChanged(qlonglong quantity);
    void widthChanged(double width);
    void heightChanged(double height);
    void sourceChanged(const QString &source);
    void workflowChanged();

protected:
    void setupEntity(const QSharedPointer<NetworkDataEntity> &old = QSharedPointer<NetworkDataEntity>()) override;

};

}
}

#endif // PROOF_MIS_JOBQMLWRAPPER_H
