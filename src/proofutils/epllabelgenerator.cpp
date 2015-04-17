#include "epllabelgenerator.h"

#include "proofcore/proofobject_p.h"

#include <QtMath>

namespace Proof {
class EplLabelGeneratorPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(EplLabelGenerator)

    QSize charSize(int fontSize, int horizontalScale, int verticalScale) const;

    QByteArray lastLabel;
    int dpi = 203;
    int labelWidth = 795;
    int labelHeight = 1250;

};

} // namespace Proof

using namespace Proof;

EplLabelGenerator::EplLabelGenerator(int printerDpi, QObject *parent)
    : ProofObject(*new EplLabelGeneratorPrivate, parent)
{
    Q_D(EplLabelGenerator);
    //We don't support any other dpi's for now
    d->dpi = (printerDpi < 300) ? 203 : 300;
}

void EplLabelGenerator::startLabel(int width, int height, int speed, int density, int gapLength)
{
    Q_D(EplLabelGenerator);
    d->labelWidth = width;
    d->labelHeight = height;
    d->lastLabel.clear();
    d->lastLabel.append("I8,A,001\n");
    d->lastLabel.append("OD\n");
    d->lastLabel.append(QString("q%1\n").arg(width));
    d->lastLabel.append(QString("Q%1,%2\n").arg(height).arg(gapLength));
    d->lastLabel.append(QString("S%1\n").arg(speed));
    d->lastLabel.append(QString("D%1\n").arg(density));
    d->lastLabel.append("JF\n\n");
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
        horizontalScale = 7;

    if (verticalScale < 1)
        verticalScale = 1;
    if (verticalScale > 9)
        verticalScale = 9;

    QString preparedText = text;
    preparedText.replace("\\", "\\\\").replace("\"", "\\\"");

    rotation = (rotation % 360) / 90;

    d->lastLabel.append(QString("A%1,%2,%3,%4,%5,%6,%7,\"%8\"\n")
                        .arg(x)
                        .arg(y)
                        .arg(rotation)
                        .arg(fontSize)
                        .arg(horizontalScale)
                        .arg(verticalScale)
                        .arg(inverseColors ? "R" : "N")
                        .arg(preparedText));

    QSize singleCharSize = d->charSize(fontSize, horizontalScale, verticalScale);
    QRect rect(QPoint(x, y), QSize(singleCharSize.width() * text.length(), singleCharSize.height()));

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

QRect EplLabelGenerator::addBarcode(const QString &data, EplLabelGenerator::BarcodeType type, int x, int y, int height,
                                    bool printReadableCode, int narrowBarWidth, int narrowBarHeight)
{
    Q_D(EplLabelGenerator);
    //We can't calc width here, so let's assume it goes straight to the end
    QRect rect(x, y, 0, height);
    return rect;
}

QRect EplLabelGenerator::addLine(int x, int y, int width, int height, EplLabelGenerator::LineType type, int endY)
{
    Q_D(EplLabelGenerator);
    QRect rect(x, y, width, height);

    QString templateString;

    switch (type) {
    case LineType::Black:
        templateString = QStringLiteral("LO%1,%2,%3,%4\n");
        break;
    case LineType::White:
        templateString = QStringLiteral("LW%1,%2,%3,%4\n");
        break;
    case LineType::Xor:
        templateString = QStringLiteral("LE%1,%2,%3,%4\n");
        break;
    case LineType::Diagonal: {
        templateString = QString("LE%1,%2,%3,%4,") + QString::number(endY) + QStringLiteral("\n");
        int rectHeight = endY - y;
        rect = QRect(x, y, qSqrt(width * width - rectHeight * rectHeight), rectHeight);
        break;
    }
    }

    d->lastLabel.append(templateString
                        .arg(x)
                        .arg(y)
                        .arg(width)
                        .arg(height));

    return rect;
}

void EplLabelGenerator::addPrintCommand(int copies)
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append(QString("P%1\n").arg(copies));
}

void EplLabelGenerator::addClearBufferCommand()
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append("N\n");
}

QByteArray EplLabelGenerator::labelData() const
{
    Q_D(const EplLabelGenerator);
    return d->lastLabel;
}

QSize EplLabelGeneratorPrivate::charSize(int fontSize, int horizontalScale, int verticalScale) const
{
    QSize result;
    switch(fontSize) {
    case 1:
        result = (dpi == 300) ? QSize(12, 20) : QSize(8, 12);
        break;
    case 2:
        result = (dpi == 300) ? QSize(16, 28) : QSize(10, 16);
        break;
    case 3:
        result = (dpi == 300) ? QSize(20, 36) : QSize(12, 20);
        break;
    case 4:
        result = (dpi == 300) ? QSize(24, 44) : QSize(14, 24);
        break;
    case 5:
        result = (dpi == 300) ? QSize(48, 80) : QSize(32, 48);
        break;
    case 6:
    case 7:
        result = QSize(14, 19);
        break;
    default:
        result = QSize(0, 0);
        break;
    }
    //Adding 2 to width for inter-character gaps
    return QSize((result.width() + 2) * horizontalScale, result.height() * verticalScale);
}

