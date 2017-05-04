#include "epllabelgenerator.h"

#include "qrcodegenerator.h"

#include <QtMath>

//All constants here are taken from manual https://www.zebra.com/content/dam/zebra/manuals/en-us/printer/epl2-pm-en.pdf

namespace Proof {
class EplLabelGeneratorPrivate
{
    Q_DECLARE_PUBLIC(EplLabelGenerator)

    QSize charSize(int fontSize, int horizontalScale, int verticalScale) const;

    EplLabelGenerator *q_ptr = nullptr;

    QByteArray lastLabel;
    int dpi = 203;
    int labelWidth = 795;
    int labelHeight = 1250;
    int speed = 4;
    int density = 10;
    int gapLength = 24;
};

uint qHash(EplLabelGenerator::BarcodeType barcodeType, uint seed = 0)
{
    return ::qHash(static_cast<int>(barcodeType), seed);
}

//TODO: consider moving all big statics to Q_GLOBAL_STATIC
static const QHash<EplLabelGenerator::BarcodeType, QString> STRINGIFIED_BARCODE_TYPES = {
    {EplLabelGenerator::BarcodeType::Code39, "3"},
    {EplLabelGenerator::BarcodeType::Code39WithCheckDigit, "3C"},
    {EplLabelGenerator::BarcodeType::Code93, "9"},
    {EplLabelGenerator::BarcodeType::Code128UCC, "0"},
    {EplLabelGenerator::BarcodeType::Code128Auto, "1"},
    {EplLabelGenerator::BarcodeType::Code128A, "1A"},
    {EplLabelGenerator::BarcodeType::Code128B, "1B"},
    {EplLabelGenerator::BarcodeType::Code128C, "1C"},
    {EplLabelGenerator::BarcodeType::Code128DeutschePost, "1D"},
    {EplLabelGenerator::BarcodeType::Codabar, "K"},
    {EplLabelGenerator::BarcodeType::Ean8, "E80"},
    {EplLabelGenerator::BarcodeType::Ean8Addon2, "E82"},
    {EplLabelGenerator::BarcodeType::Ean8Addon5, "E85"},
    {EplLabelGenerator::BarcodeType::Ean13, "E30"},
    {EplLabelGenerator::BarcodeType::Ean13Addon2, "E32"},
    {EplLabelGenerator::BarcodeType::Ean13Addon5, "E35"},
    {EplLabelGenerator::BarcodeType::GermanPostCode, "2G"},
    {EplLabelGenerator::BarcodeType::Interleaved2Of5, "2"},
    {EplLabelGenerator::BarcodeType::Interleaved2Of5WithMod10CheckDigit, "2C"},
    {EplLabelGenerator::BarcodeType::Interleaved2Of5WithHumanReadableCheckDigit, "2D"},
    {EplLabelGenerator::BarcodeType::Postnet, "P"},
    {EplLabelGenerator::BarcodeType::Planet, "PL"},
    {EplLabelGenerator::BarcodeType::PostnetJapanese, "J"},
    {EplLabelGenerator::BarcodeType::UccEan128, "1E"},
    {EplLabelGenerator::BarcodeType::UpcA, "UA0"},
    {EplLabelGenerator::BarcodeType::UpcAAddon2, "UA2"},
    {EplLabelGenerator::BarcodeType::UpcAAddon5, "UA5"},
    {EplLabelGenerator::BarcodeType::UpcE, "UE0"},
    {EplLabelGenerator::BarcodeType::UpcEAddon2, "UE2"},
    {EplLabelGenerator::BarcodeType::UpcEAddon5, "UE5"},
    {EplLabelGenerator::BarcodeType::UpcInterleaved2Of5, "2U"},
    {EplLabelGenerator::BarcodeType::Msi1WithMod10CheckDigit, "L"},
    {EplLabelGenerator::BarcodeType::Msi3WithMod10CheckDigit, "M"}
};

} // namespace Proof

using namespace Proof;

EplLabelGenerator::EplLabelGenerator(int printerDpi)
    : d_ptr(new EplLabelGeneratorPrivate)
{
    d_ptr->q_ptr = this;
    //We don't support any other dpi's for now
    d_ptr->dpi = (printerDpi < 300) ? 203 : 300;
}

EplLabelGenerator::~EplLabelGenerator()
{
}

void EplLabelGenerator::startLabel(int width, int height, int speed, int density, int gapLength)
{
    Q_D(EplLabelGenerator);
    d->labelWidth = width;
    d->labelHeight = height;
    d->speed = speed;
    d->density = density;
    d->gapLength = gapLength;
    d->lastLabel.clear();
    startPage();
}

QRect EplLabelGenerator::addText(const QString &text, int x, int y, int fontSize,
                                 int horizontalScale, int verticalScale, int rotation, bool inverseColors)
{
    Q_D(EplLabelGenerator);

    if (fontSize > 7)
        fontSize = 7;
    if (fontSize < 1)
        fontSize = 1;
    if (!text.toInt() && fontSize > 5)
        fontSize = 5;

    if (horizontalScale < 1)
        horizontalScale = 1;
    if (horizontalScale > 8)
        horizontalScale = 8;
    if (horizontalScale == 7)
        horizontalScale = 6;

    if (verticalScale < 1)
        verticalScale = 1;
    if (verticalScale > 9)
        verticalScale = 9;

    QString preparedText = text;
    preparedText.replace(QLatin1String("\\"), QLatin1String("\\\\")).replace(QLatin1String("\""), QLatin1String("\\\""));

    rotation = (rotation % 360) / 90;

    d->lastLabel.append(QStringLiteral("A%1,%2,%3,%4,%5,%6,%7,\"%8\"\n")
                        .arg(x)
                        .arg(y)
                        .arg(rotation)
                        .arg(fontSize)
                        .arg(horizontalScale)
                        .arg(verticalScale)
                        .arg(inverseColors ? QStringLiteral("R") : QStringLiteral("N")) // clazy:exclude=qstring-arg
                        .arg(preparedText));

    QRect rect(QPoint(x, y), textSize(text, fontSize, horizontalScale, verticalScale));

    switch (rotation) {
    case 1:
        rect = QRect(rect.x() - rect.height(), rect.y(), rect.height(), rect.width());
        break;
    case 2:
        rect = QRect(rect.x() - rect.width(), rect.y() - rect.height(), rect.width(), rect.height());
        break;
    case 3:
        rect = QRect(rect.x(), rect.y() - rect.width(), rect.height(), rect.width());
        break;
    default:
        break;
    }

    return rect;
}

QSize EplLabelGenerator::textSize(const QString &text, int fontSize, int horizontalScale, int verticalScale) const
{
    Q_D(const EplLabelGenerator);
    QSize singleCharSize = d->charSize(fontSize, horizontalScale, verticalScale);
    return QSize(singleCharSize.width() * text.length(), singleCharSize.height());
}

QSize EplLabelGenerator::labelSize() const
{
    Q_D(const EplLabelGenerator);
    return QSize(d->labelWidth, d->labelHeight);
}

QRect EplLabelGenerator::addBarcode(const QString &data, EplLabelGenerator::BarcodeType type, int x, int y, int height,
                                    bool printReadableCode, int narrowBarWidth, int wideBarWidth, int rotation)
{
    Q_D(EplLabelGenerator);
    QString preparedData = data;
    preparedData.replace(QLatin1String("\\"), QLatin1String("\\\\")).replace(QLatin1String("\""), QLatin1String("\\\""));

    rotation = (rotation % 360) / 90;

    d->lastLabel.append(QStringLiteral("B%1,%2,%3,%4,%5,%6,%7,%8,\"%9\"\n")
                        .arg(x)
                        .arg(y)
                        .arg(rotation)
                        .arg(STRINGIFIED_BARCODE_TYPES.value(type, QStringLiteral("1")))
                        .arg(narrowBarWidth)
                        .arg(wideBarWidth)
                        .arg(height)
                        .arg(printReadableCode ? QStringLiteral("B") : QStringLiteral("N")) // clazy:exclude=qstring-arg
                        .arg(preparedData));

    if (printReadableCode)
        height += d->charSize(4, 1, 1).height();
    QRect rect(x, y, d->labelWidth - x, height);

    //We can't calc width here, so let's assume it goes straight to the end
    switch (rotation) {
    case 1:
        rect = QRect(rect.x() - rect.height(), rect.y(), rect.height(), d->labelHeight - rect.y());
        break;
    case 2:
        rect = QRect(0, rect.y() - rect.height(), rect.x(), rect.height());
        break;
    case 3:
        rect = QRect(rect.x(), 0, rect.height(), rect.y());
        break;
    default:
        break;
    }

    return rect;
}

QRect EplLabelGenerator::addQrCode(const QString &data, int x, int y, int width)
{
    Q_D(EplLabelGenerator);
    auto rawBinary = QrCodeGenerator::generateEplBinaryData(data, width);
    width = width + ((width + 8) % 8);

    d->lastLabel.append(QStringLiteral("GW%1,%2,%3,%4,")
                        .arg(x)
                        .arg(y)
                        .arg(width / 8)
                        .arg(width));
    d->lastLabel.append(rawBinary);
    d->lastLabel.append("\n");

    return QRect(x, y, width, width);
}

QRect EplLabelGenerator::addLine(int x, int y, int width, int height, EplLabelGenerator::LineType type)
{
    Q_D(EplLabelGenerator);
    QString lineTypeString;
    switch (type) {
    case LineType::Black:
        lineTypeString = QStringLiteral("O");
        break;
    case LineType::White:
        lineTypeString = QStringLiteral("W");
        break;
    case LineType::Xor:
        lineTypeString = QStringLiteral("E");
        break;
    }

    d->lastLabel.append(QStringLiteral("L%1%2,%3,%4,%5\n")
                        .arg(lineTypeString)
                        .arg(x)
                        .arg(y)
                        .arg(width)
                        .arg(height));

    return QRect(x, y, width, height);
}

QRect EplLabelGenerator::addDiagonalLine(int x, int y, int endX, int endY, int width)
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append(QStringLiteral("LS%1,%2,%3,%4,%5\n")
                        .arg(x)
                        .arg(y)
                        .arg(width)
                        .arg(endX)
                        .arg(endY));

    return {QPoint(qMin(x, endX), qMin(y, endY)), QSize(qAbs(endX - x), qAbs(endY - y) + width)};
}

void EplLabelGenerator::addPrintCommand(int copies)
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append(QStringLiteral("P%1\n").arg(copies));
}

void EplLabelGenerator::addClearBufferCommand()
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append("N\n");
}

void EplLabelGenerator::startPage()
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append("I8,A,001\n");
    d->lastLabel.append("OD\n");
    d->lastLabel.append(QStringLiteral("q%1\n").arg(d->labelWidth));
    d->lastLabel.append(QStringLiteral("Q%1,%2\n").arg(d->labelHeight).arg(d->gapLength));
    d->lastLabel.append(QStringLiteral("S%1\n").arg(d->speed));
    d->lastLabel.append(QStringLiteral("D%1\n").arg(d->density));
    d->lastLabel.append("JF\n\n");
}

QByteArray EplLabelGenerator::labelData() const
{
    Q_D(const EplLabelGenerator);
    return d->lastLabel;
}

QSize EplLabelGeneratorPrivate::charSize(int fontSize, int horizontalScale, int verticalScale) const
{
    QSize result;
    //Adding 2 to each size for inter-character gaps
    switch(fontSize) {
    case 1:
        result = (dpi == 300) ? QSize(14, 22) : QSize(10, 14);
        break;
    case 2:
        result = (dpi == 300) ? QSize(18, 30) : QSize(12, 18);
        break;
    case 3:
        result = (dpi == 300) ? QSize(22, 38) : QSize(14, 22);
        break;
    case 4:
        result = (dpi == 300) ? QSize(26, 46) : QSize(16, 26);
        break;
    case 5:
        result = (dpi == 300) ? QSize(50, 82) : QSize(34, 50);
        break;
    case 6:
    case 7:
        result = QSize(16, 21);
        break;
    default:
        result = QSize(0, 0);
        break;
    }
    return QSize(result.width() * horizontalScale, result.height() * verticalScale);
}

