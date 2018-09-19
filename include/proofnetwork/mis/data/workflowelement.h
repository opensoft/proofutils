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
#ifndef PROOF_MIS_WORKFLOWELEMENT_H
#define PROOF_MIS_WORKFLOWELEMENT_H

#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/proofnetworkmis_global.h"

namespace Proof {
namespace Mis {

class WorkflowElementPrivate;
class PROOF_NETWORK_MIS_EXPORT WorkflowElement
{
    Q_DECLARE_PRIVATE(WorkflowElement)
public:
    explicit WorkflowElement(const QString &string = QString());
    WorkflowElement(WorkflowAction action, WorkflowStatus status, PaperSide paperSide = PaperSide::NotSetSide);
    WorkflowElement(const WorkflowElement &other);
    WorkflowElement(WorkflowElement &&other);
    ~WorkflowElement();

    WorkflowAction action() const;
    void setAction(WorkflowAction arg);
    WorkflowStatus status() const;
    void setStatus(WorkflowStatus arg);
    PaperSide paperSide() const;
    void setPaperSide(PaperSide arg);

    QString toString() const;

    WorkflowElement &operator=(const WorkflowElement &other);
    WorkflowElement &operator=(WorkflowElement &&other);
    bool operator==(const WorkflowElement &other) const;
    bool operator!=(const WorkflowElement &other) const;
    void swap(WorkflowElement &other);

private:
    QScopedPointer<WorkflowElementPrivate> d_ptr;
};

inline void swap(Proof::Mis::WorkflowElement &lhs, Proof::Mis::WorkflowElement &rhs)
{
    lhs.swap(rhs);
}

} // namespace Mis
} // namespace Proof

#endif // PROOF_MIS_WORKFLOWELEMENT_H
