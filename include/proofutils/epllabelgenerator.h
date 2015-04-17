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

    //TODO: add other types
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
        UccEan128
    };

    enum class LineType
    {
        Black,
        White,
        Xor,
        Diagonal
    };

    EplLabelGenerator(int printerDpi = 203);
    virtual ~EplLabelGenerator();

    void startLabel(int width = 795, int height = 1250, int speed = 4, int density = 10, int gapLength = 24);

    QRect addText(const QString &text, int x, int y, int fontSize = 3,
                  int horizontalScale = 1, int verticalScale = 1,
                  int rotation = 0, bool inverseColors = false);

    QSize textSize(const QString &text, int fontSize = 3, int horizontalScale = 1, int verticalScale = 1) const;

    QRect addBarcode(const QString &data, BarcodeType type, int x, int y, int height,
                     bool printReadableCode = true, int narrowBarWidth = 2, int narrowBarHeight = 4);

    //endY affects only LineType::Diagonal
    QRect addLine(int x, int y, int width, int height, LineType type = LineType::Black, int endY = 0);

    void addPrintCommand(int copies = 1);
    void addClearBufferCommand();

    QByteArray labelData() const;

private:
    QScopedPointer<EplLabelGeneratorPrivate> d_ptr;
};

} // namespace Proof

#endif // PROOF_EPLLABELGENERATOR_H
