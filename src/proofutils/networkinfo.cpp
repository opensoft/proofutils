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

QVariantMap NetworkInfo::addresses() const
{
    QVariantMap addresses;
    for (const auto &interface : QNetworkInterface::allInterfaces()) {
        for (const auto &address : interface.addressEntries()) {
            auto ip = address.ip();
            if (ip.isLoopback())
                continue;
            QVariantList list = addresses.value(interface.humanReadableName(), QVariantList{}).toList();
            list << ip.toString();
            addresses[interface.humanReadableName()] = list;
        }
    }
    return addresses;
}

} // namespace Proof

