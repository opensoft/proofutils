#include "ipaddressvalidator.h"

namespace Proof {
class IpAddressValidatorPrivate
{
    Q_DECLARE_PUBLIC(IpAddressValidator)

    IpAddressValidator *q_ptr = nullptr;

    bool maskValidationMode = false;

    QValidator::State validateIp(const QVector<quint8> &parts) const;
    QValidator::State validateMask(const QVector<quint8> &parts) const;
    static QVector<quint8> ipToUIntParts(const QString &ip);

    bool checkBinary(quint8 byte) const;
};
} // namespace Proof

using namespace Proof;

IpAddressValidator::IpAddressValidator(QObject *parent) : QValidator(parent), d_ptr(new IpAddressValidatorPrivate)
{
    d_ptr->q_ptr = this;
}

IpAddressValidator::~IpAddressValidator()
{}

QValidator::State IpAddressValidator::validate(QString &input, int &pos) const
{
    Q_D(const IpAddressValidator);
    Q_UNUSED(pos)

    QVector<quint8> parts = IpAddressValidatorPrivate::ipToUIntParts(input);

    if (parts.isEmpty())
        return State::Intermediate;

    if (d->maskValidationMode)
        return d->validateMask(parts);

    return d->validateIp(parts);
}

bool IpAddressValidator::maskValidationMode() const
{
    Q_D(const IpAddressValidator);
    return d->maskValidationMode;
}

void IpAddressValidator::setMaskValidationMode(bool maskValidationMode)
{
    Q_D(IpAddressValidator);
    if (d->maskValidationMode == maskValidationMode)
        return;

    d->maskValidationMode = maskValidationMode;
    emit maskValidationModeChanged(d->maskValidationMode);
}

QValidator::State IpAddressValidatorPrivate::validateIp(const QVector<quint8> &parts) const
{
    if (parts[0] > 223)
        return QValidator::State::Intermediate;
    if (parts[3] == 0)
        return QValidator::State::Intermediate;

    return QValidator::State::Acceptable;
}

QValidator::State IpAddressValidatorPrivate::validateMask(const QVector<quint8> &parts) const
{
    if (!checkBinary(parts[0]))
        return QValidator::State::Intermediate;

    bool zeroFound = (parts[0] == 0);
    for (int i = 1; i < parts.count(); ++i) {
        if (parts[i] == 0) {
            zeroFound = true;
            continue;
        }
        if (zeroFound && parts[i] != 0)
            return QValidator::State::Intermediate;

        if (!checkBinary(parts[i]))
            return QValidator::State::Intermediate;
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

QVector<quint8> IpAddressValidatorPrivate::ipToUIntParts(const QString &ip)
{
    QStringList splittedInput = ip.split(QStringLiteral("."));
    if (splittedInput.count() != 4)
        return QVector<quint8>();

    QVector<quint8> parts;

    for (const auto &part : splittedInput) {
        bool ok = true;
        uint convertedPart = part.trimmed().toUInt(&ok);
        if (!ok || convertedPart > 255)
            return QVector<quint8>();
        parts.append(static_cast<quint8>(convertedPart));
    }

    return parts;
}
