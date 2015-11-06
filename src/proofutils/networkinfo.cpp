#include "networkinfo.h"

#include "proofcore/proofobject_p.h"

#include <QNetworkInterface>

namespace Proof {

class NetworkInfoPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(NetworkInfo)

    QList<QString> addresses;
};

NetworkInfo::NetworkInfo(QObject *parent)
    : ProofObject(*new NetworkInfoPrivate, parent)
{
    Q_D(NetworkInfo);
    for (const auto &interface : QNetworkInterface::allInterfaces()) {
        for (const auto &address : interface.addressEntries()) {
            auto ip = address.ip();
            if (!ip.isLoopback())
                d->addresses << QString("%1 - %2").arg(interface.humanReadableName(), ip.toString());
        }
    }
}

QList<QString> NetworkInfo::addresses() const
{
    Q_D(const NetworkInfo);
    return d->addresses;
}

} // namespace Proof

