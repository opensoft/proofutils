#include "ipaddressvalidator.h"

namespace Proof {
class IpAddressValidatorPrivate
{
    Q_DECLARE_PUBLIC(IpAddressValidator)

    IpAddressValidator *q_ptr = nullptr;

    bool maskValidationMode = false;

    QValidator::State validateIp(const QList<int> &parts) const;
    QValidator::State validateMask(const QList<int> &parts) const;
    static quint32 ipToUInt(const QString &ip);
    static QList<int> ipToIntParts(const QString &ip);

    bool checkBinary(quint8 byte) const;
};
}

using namespace Proof;

IpAddressValidator::IpAddressValidator(QObject *parent) :
    QValidator (parent),
    d_ptr(new IpAddressValidatorPrivate)
{
    d_ptr->q_ptr = this;
}

IpAddressValidator::~IpAddressValidator()
{

}

QValidator::State IpAddressValidator::validate(QString &input, int &pos) const
{
    Q_D(const IpAddressValidator);
    Q_UNUSED(pos);

    QList<int> parts = IpAddressValidatorPrivate::ipToIntParts(input);

    if (parts.isEmpty())
        return State::Invalid;

    if (d->maskValidationMode)
        return d->validateMask(parts);

    return d->validateIp(parts);
}

bool IpAddressValidator::maskValidationMode() const
{
    Q_D(const IpAddressValidator);
    return d->maskValidationMode;
}

bool IpAddressValidator::ipBelongsToSubnet(const QString &ip, const QString &subnet) const
{
    QList<int> parts = IpAddressValidatorPrivate::ipToIntParts(ip);
    quint32 ipAddress = IpAddressValidatorPrivate::ipToUInt(ip);
    quint32 networkAddress = 0;
    quint8 byte = 255;

    if (parts[0] < 127)
        networkAddress |= byte << 16;

    if (parts[0] < 193)
        networkAddress |= byte << 8;
    networkAddress |= byte;

    networkAddress = ~networkAddress;
    networkAddress &= ipAddress;

    quint32 maskAddress = IpAddressValidatorPrivate::ipToUInt(subnet);

    quint32 lowerAddress = (networkAddress & maskAddress);
    quint32 upperAddress = (lowerAddress | (~maskAddress));

    if (ipAddress >= lowerAddress &&
            ipAddress <= upperAddress)
        return true;
    return false;
}

void IpAddressValidator::setMaskValidationMode(bool maskValidationMode)
{
    Q_D(IpAddressValidator);
    if (d->maskValidationMode == maskValidationMode)
        return;

    d->maskValidationMode = maskValidationMode;
    emit maskValidationModeChanged(d->maskValidationMode);
}

QValidator::State IpAddressValidatorPrivate::validateIp(const QList<int> &parts) const
{
    if (parts[0] > 223 || parts[0] < 1)
        return QValidator::State::Invalid;

    if (parts[1] > 255)
        return QValidator::State::Invalid;
    if (parts[1] == -1)
        return QValidator::State::Intermediate;

    if (parts[2] > 255)
        return QValidator::State::Invalid;
    if (parts[2] == -1)
        return QValidator::State::Intermediate;

    if (parts[3] > 255 || parts[3] == 0)
        return QValidator::State::Invalid;
    if (parts[3] == -1)
        return QValidator::State::Intermediate;

    return QValidator::State::Acceptable;
}

QValidator::State IpAddressValidatorPrivate::validateMask(const QList<int> &parts) const
{
    if (parts[0] <= 0 || parts[0] > 255)
        return QValidator::State::Invalid;
    if (parts[0] < 100)
        return QValidator::State::Intermediate;
    if (!checkBinary(static_cast<quint8>(parts[0])))
        return QValidator::State::Invalid;

    for (int i = 1; i < parts.count(); ++i) {
        if (parts[i] == 0)
            continue;
        if (parts[i] < 0)
            return QValidator::State::Intermediate;
        if (parts[i] > 255)
            return QValidator::State::Invalid;
        if (parts[i] < 100)
            return QValidator::State::Intermediate;
        if (!checkBinary(static_cast<quint8>(parts[i])))
            return QValidator::State::Invalid;
    }
    return QValidator::State::Acceptable;
}

bool Proof::IpAddressValidatorPrivate::checkBinary(quint8 byte) const
{
    if (byte == 0)
        return true;
    quint8 mask = 0;
    for (quint8 i = 0; i < 8; ++i) {
        mask |= 1 << (7 - i);
        if (!(byte ^ mask))
            return true;
    }
    return false;
}

QList<int> IpAddressValidatorPrivate::ipToIntParts(const QString &ip)
{
    QStringList splittedInput = ip.split(".");
    if (splittedInput.count() != 4)
        return QList<int>();

    QList<int> parts;

    for (const auto &part : splittedInput) {
        bool ok = true;
        int convertedPart = part.trimmed().isEmpty() ? -1 : part.trimmed().toInt(&ok);
        if (!ok)
            return QList<int>();
        parts.append(convertedPart);
    }

    return parts;
}

quint32 IpAddressValidatorPrivate::ipToUInt(const QString &ip)
{
    quint32 address = 0;

    QList<int> parts = ipToIntParts(ip);

    for (quint8 i = 0; i < parts.count(); ++i)
        address |= static_cast<quint8>(parts[i]) << ((3 - i) * 8);

    return address;
}

