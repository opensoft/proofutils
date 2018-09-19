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
#ifndef PROOF_MIS_JOBQMLWRAPPER_H
#define PROOF_MIS_JOBQMLWRAPPER_H

#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"
#include "proofnetwork/mis/proofnetworkmis_types.h"
#include "proofnetwork/qmlwrappers/networkdataentityqmlwrapper.h"

namespace Proof {
namespace Mis {

class Job;

class JobQmlWrapperPrivate;
class PROOF_NETWORK_MIS_EXPORT JobQmlWrapper : public NetworkDataEntityQmlWrapper
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id NOTIFY idChanged)
    Q_PROPERTY(Proof::Mis::EntityStatus status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(qlonglong quantity READ quantity NOTIFY quantityChanged)
    Q_PROPERTY(double width READ width NOTIFY widthChanged)
    Q_PROPERTY(double height READ height NOTIFY heightChanged)
    Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)
    Q_PROPERTY(bool hasPreview READ hasPreview NOTIFY hasPreviewChanged)
    Q_DECLARE_PRIVATE(JobQmlWrapper)
public:
    explicit JobQmlWrapper(const JobSP &job, QObject *parent = nullptr);
    ~JobQmlWrapper();
    PROOF_NDE_WRAPPER_TOOLS(Job)

    QString id() const;
    Proof::Mis::EntityStatus status() const;
    QString name() const;
    qlonglong quantity() const;
    double width() const;
    double height() const;
    QString source() const;
    int pageCount() const;
    bool hasPreview() const;
    Q_INVOKABLE Proof::Mis::WorkflowStatus
    workflowStatus(Proof::Mis::WorkflowAction action,
                   Proof::Mis::PaperSide paperSide = Proof::Mis::PaperSide::NotSetSide) const;

signals:
    void idChanged(const QString &id);
    void statusChanged(Proof::Mis::EntityStatus status);
    void nameChanged(const QString &name);
    void quantityChanged(qlonglong quantity);
    void widthChanged(double width);
    void heightChanged(double height);
    void sourceChanged(const QString &source);
    void pageCountChanged(int pageCount);
    void hasPreviewChanged(bool hasPreview);
    void workflowChanged();

protected:
    void setupEntity(const QSharedPointer<NetworkDataEntity> &old = QSharedPointer<NetworkDataEntity>()) override;
};

} // namespace Mis
} // namespace Proof

#endif // PROOF_MIS_JOBQMLWRAPPER_H
