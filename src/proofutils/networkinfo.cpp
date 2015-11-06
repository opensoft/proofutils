#include "networkinfo.h"

#include "proofcore/proofobject_p.h"

#include <QNetworkInterface>

namespace Proof {

class NetworkInfoPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(NetworkInfo)
};

NetworkInfo::NetworkInfo(QObject *parent)
    : ProofObject(*new NetworkInfoPrivate, parent)
{
}

QList<QString> NetworkInfo::addresses() const
{
    QList<QString> addresses;
    for (const auto &interface : QNetworkInterface::allInterfaces()) {
        for (const auto &address : interface.addressEntries()) {
            auto ip = address.ip();
            if (!ip.isLoopback())
                addresses << QString("%1 - %2").arg(interface.humanReadableName(), ip.toString());
        }
    }
    return addresses;
}

} // namespace Proof

