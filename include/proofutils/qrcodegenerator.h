#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include "proofutils_global.h"

#include <QImage>
#include "qrencode.h"

namespace Proof {

namespace QrCodeGenerator
{
    enum class Mode {
        Numeric,
        AlphaNumeric,
        Character
    };

    enum class ErrorCorrection {
        LowLevel,
        MediumLevel,
        QuartileLevel,
        HighLevel
    };

    PROOF_UTILS_EXPORT QImage generateBitmap(const QString &string, int width = 200,
                                             Mode mode = Mode::Character, ErrorCorrection errorCorrection = ErrorCorrection::QuartileLevel);
    PROOF_UTILS_EXPORT QByteArray generateEplBinaryData(const QString &string, int width = 200,
                                                        Mode mode = Mode::Character, ErrorCorrection errorCorrection = ErrorCorrection::QuartileLevel);

    PROOF_UTILS_EXPORT uint qHash(Proof::QrCodeGenerator::Mode arg, uint seed = 0);
    PROOF_UTILS_EXPORT uint qHash(Proof::QrCodeGenerator::ErrorCorrection arg, uint seed = 0);
}
} // namespace Proof


#endif // QRCODEGENERATOR_H
