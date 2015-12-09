#ifndef PROOF_EPLLABELGENERATOR_H
#define PROOF_EPLLABELGENERATOR_H

#include "proofutils_global.h"

#include <QRect>

namespace Proof {

class EplLabelGeneratorPrivate;
class PROOF_UTILS_EXPORT EplLabelGenerator
{
    Q_DECLARE_PRIVATE(EplLabelGenerator)
public:

    enum class BarcodeType
    {
        Code39,
        Code39WithCheckDigit,
        Code93,
        Code128UCC,
        Code128Auto,
        Code128A,
        Code128B,
        Code128C,
        Code128DeutschePost,
        Codabar,
        Ean8,
        Ean8Addon2,
        Ean8Addon5,
        Ean13,
        Ean13Addon2,
        Ean13Addon5,
        GermanPostCode,
        Interleaved2Of5,
        Interleaved2Of5WithMod10CheckDigit,
        Interleaved2Of5WithHumanReadableCheckDigit,
        Postnet,
        Planet,
        PostnetJapanese,
        UccEan128,
        UpcA,
        UpcAAddon2,
        UpcAAddon5,
        UpcE,
        UpcEAddon2,
        UpcEAddon5,
        UpcInterleaved2Of5,
        Msi1WithMod10CheckDigit,
        Msi3WithMod10CheckDigit
    };

    enum class LineType
    {
        Black,
        White,
        Xor
    };

    EplLabelGenerator(int printerDpi = 203);
    virtual ~EplLabelGenerator();

    void startLabel(int width = 795, int height = 1250, int speed = 4, int density = 10, int gapLength = 24);

    QRect addText(const QString &text, int x, int y, int fontSize = 4,
                  int horizontalScale = 1, int verticalScale = 1,
                  int rotation = 0, bool inverseColors = false);

    QSize textSize(const QString &text, int fontSize = 4, int horizontalScale = 1, int verticalScale = 1) const;
    QSize labelSize() const;

    QRect addBarcode(const QString &data, BarcodeType type, int x, int y, int height = 200,
                     bool printReadableCode = true, int narrowBarWidth = 2, int wideBarWidth = 4, int rotation = 0);

    QRect addLine(int x, int y, int width, int height, LineType type = LineType::Black);
    QRect addDiagonalLine(int x, int y, int endX, int endY, int width);

    void addPrintCommand(int copies = 1);
    void addClearBufferCommand();
    void startPage();

    QByteArray labelData() const;

private:
    QScopedPointer<EplLabelGeneratorPrivate> d_ptr;
};

} // namespace Proof

#endif // PROOF_EPLLABELGENERATOR_H
