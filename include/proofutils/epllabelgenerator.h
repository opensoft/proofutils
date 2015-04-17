#ifndef PROOF_EPLLABELGENERATOR_H
#define PROOF_EPLLABELGENERATOR_H

#include "proofcore/proofobject.h"
#include "proofutils_global.h"

#include <QRect>

namespace Proof {

class EplLabelGeneratorPrivate;
class PROOF_UTILS_EXPORT EplLabelGenerator : public ProofObject
{
    Q_OBJECT
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

    EplLabelGenerator(int printerDpi = 203, QObject *parent = 0);

    void startLabel(int width = 795, int height = 1250, int speed = 4, int density = 10, int gapLength = 24);

    QRect addText(const QString &text, int x, int y, int fontSize = 3,
                  int horizontalScale = 1, int verticalScale = 1,
                  int rotation = 0, bool inverseColors = false);

    QRect addBarcode(const QString &data, BarcodeType type, int x, int y, int height,
                     bool printReadableCode = true, int narrowBarWidth = 2, int narrowBarHeight = 4);

    void addPrintCommand(int copies = 1);
    void addClearBufferCommand();

    QByteArray labelData() const;
};

} // namespace Proof

#endif // PROOF_EPLLABELGENERATOR_H
