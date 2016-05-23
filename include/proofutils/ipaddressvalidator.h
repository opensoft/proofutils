#ifndef IPADDRESSVALIDATOR_H
#define IPADDRESSVALIDATOR_H

#include "proofutils_global.h"

#include <QValidator>

namespace Proof {

class IpAddressValidatorPrivate;
class PROOF_UTILS_EXPORT IpAddressValidator : public QValidator
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(IpAddressValidator)
    Q_PROPERTY(bool maskValidationMode READ maskValidationMode WRITE setMaskValidationMode NOTIFY maskValidationModeChanged)

public:
    IpAddressValidator(QObject *parent = Q_NULLPTR);
    ~IpAddressValidator();

    QValidator::State validate(QString &input, int &pos) const override;
    bool maskValidationMode() const;

public slots:
    void setMaskValidationMode(bool maskValidationMode);

signals:
    void maskValidationModeChanged(bool maskValidationMode);

private:
    QScopedPointer<IpAddressValidatorPrivate> d_ptr;
};

}

#endif // IPADDRESSVALIDATOR_H
