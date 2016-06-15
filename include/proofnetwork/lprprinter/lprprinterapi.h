#ifndef PROOF_NETWORKSERVICES_LPRPRINTERAPI_H
#define PROOF_NETWORKSERVICES_LPRPRINTERAPI_H

#include "proofnetworklprprinter_global.h"
#include "proofnetwork/proofservicerestapi.h"
#include "proofnetwork/jdf/proofnetworkjdf_types.h"

#include <QString>
#include <QByteArray>

namespace Proof {
namespace NetworkServices {

struct PROOF_NETWORK_LPRPRINTER_EXPORT LprPrinterStatus
{
    bool isReady;
    QString reason;
};

struct PROOF_NETWORK_LPRPRINTER_EXPORT LprPrinterInfo
{
    QString printer;
    bool acceptsRaw;
    bool acceptsFiles;
};

class LprPrinterApiPrivate;
class PROOF_NETWORK_LPRPRINTER_EXPORT LprPrinterApi : public ProofServiceRestApi
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LprPrinterApi)
public:
    explicit LprPrinterApi(const RestClientSP &restClient, QObject *parent = 0);

    qulonglong fetchStatus(const QString &printer = QString());
    qulonglong printLabel(const QByteArray &label, const QString &printer = QString());
    qulonglong printFile(const QString &fileName, const QString &printer = QString(), unsigned int copies = 1);
    qulonglong fetchPrintersList();

signals:
    void statusFetched(qulonglong operationId, const Proof::NetworkServices::LprPrinterStatus &status);
    void labelPrinted(qulonglong operationId);
    void filePrinted(qulonglong operationId);
    void printersListFetched(qulonglong operationId, const QList<Proof::NetworkServices::LprPrinterInfo> &printers);
};

}
}

Q_DECLARE_METATYPE(Proof::NetworkServices::LprPrinterStatus)
Q_DECLARE_METATYPE(Proof::NetworkServices::LprPrinterInfo)

#endif // PROOF_NETWORKSERVICES_LPRPRINTERAPI_H
