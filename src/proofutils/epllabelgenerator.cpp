#include "epllabelgenerator.h"

#include "proofcore/proofobject_p.h"

namespace Proof {
class EplLabelGeneratorPrivate : public ProofObjectPrivate
{
    Q_DECLARE_PUBLIC(EplLabelGenerator)
    QByteArray lastLabel;
};


} // namespace Proof

using namespace Proof;

EplLabelGenerator::EplLabelGenerator(QObject *parent)
    : ProofObject(*new EplLabelGeneratorPrivate, parent)
{
}

void EplLabelGenerator::startLabel(int width, int height, int speed, int density, int gapLength)
{
    Q_D(EplLabelGenerator);
    d->lastLabel.clear();
    d->lastLabel.append("I8,A,001\n");
    d->lastLabel.append("OD\n");
    d->lastLabel.append(QString("q%1\n").arg(width));
    d->lastLabel.append(QString("Q%2,%3\n").arg(height).arg(gapLength));
    d->lastLabel.append(QString("D%5\n").arg(speed));
    d->lastLabel.append(QString("D%5\n").arg(density));
    d->lastLabel.append("N\n");
}

QRect EplLabelGenerator::addText(const QString &text, int x, int y, int fontSize, int horizontalScale, int verticalScale, int rotation, bool inverseColors)
{
    Q_D(EplLabelGenerator);
    return QRect();
}

QRect EplLabelGenerator::addBarcode(const QString &data, EplLabelGenerator::BarcodeType type, int x, int y, int height, bool printCode, int narrowBarWidth, int narrowBarHeight)
{
    Q_D(EplLabelGenerator);
    return QRect();
}

void EplLabelGenerator::addPrintCommand(int copies)
{
    Q_D(EplLabelGenerator);
    d->lastLabel.append(QString("P%1\n").arg(copies));
}

QByteArray EplLabelGenerator::labelData() const
{
    Q_D(const EplLabelGenerator);
    return d->lastLabel;
}
