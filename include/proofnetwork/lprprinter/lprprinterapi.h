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
#ifndef PROOF_NETWORKSERVICES_LPRPRINTERAPI_H
#define PROOF_NETWORKSERVICES_LPRPRINTERAPI_H

#include "proofnetworklprprinter_global.h"

#include "proofnetwork/proofservicerestapi.h"

#include <QByteArray>
#include <QString>

namespace Proof {
namespace NetworkServices {

struct PROOF_NETWORK_LPRPRINTER_EXPORT LprPrinterStatus
{
    bool isReady = false;
    QString reason;
};

struct PROOF_NETWORK_LPRPRINTER_EXPORT LprPrinterInfo
{
    QString printer;
    bool acceptsRaw = false;
    bool acceptsFiles = false;
};

class LprPrinterApiPrivate;
class PROOF_NETWORK_LPRPRINTER_EXPORT LprPrinterApi : public ProofServiceRestApi
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LprPrinterApi)
public:
    explicit LprPrinterApi(const RestClientSP &restClient, QObject *parent = nullptr);

    CancelableFuture<LprPrinterStatus> fetchStatus(const QString &printer = QString());
    CancelableFuture<bool> printLabel(const QByteArray &label, const QString &printer = QString());
    CancelableFuture<bool> printFile(const QString &fileName, const QString &printer = QString(),
                                     unsigned int copies = 1);
    CancelableFuture<QVector<LprPrinterInfo>> fetchPrintersList();
};

} // namespace NetworkServices
} // namespace Proof

Q_DECLARE_METATYPE(Proof::NetworkServices::LprPrinterStatus)
Q_DECLARE_METATYPE(Proof::NetworkServices::LprPrinterInfo)

#endif // PROOF_NETWORKSERVICES_LPRPRINTERAPI_H
