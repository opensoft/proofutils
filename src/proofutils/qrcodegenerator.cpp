#include "qrcodegenerator.h"

#include <QPainter>
#include <qrencode.h>

using namespace Proof;

namespace {
static const QHash<QrCodeGenerator::Mode, QRencodeMode> MODE_CONVERTOR = {{QrCodeGenerator::Mode::Numeric, QR_MODE_NUM},
                                                                          {QrCodeGenerator::Mode::AlphaNumeric,
                                                                           QR_MODE_AN},
                                                                          {QrCodeGenerator::Mode::Character, QR_MODE_8}};

static const QHash<QrCodeGenerator::ErrorCorrection, QRecLevel> ERROR_CORRECTION_CONVERTOR =
    {{QrCodeGenerator::ErrorCorrection::LowLevel, QR_ECLEVEL_L},
     {QrCodeGenerator::ErrorCorrection::MediumLevel, QR_ECLEVEL_M},
     {QrCodeGenerator::ErrorCorrection::QuartileLevel, QR_ECLEVEL_Q},
     {QrCodeGenerator::ErrorCorrection::HighLevel, QR_ECLEVEL_H}};

struct QrCodeData
{
    QByteArray data;
    int width = 0;
};

QrCodeData generateRawQrCode(const QString &string, QrCodeGenerator::Mode mode,
                             QrCodeGenerator::ErrorCorrection errorCorrection)
{
    auto result = QRcode_encodeString(string.toLatin1().constData(), 0, ERROR_CORRECTION_CONVERTOR[errorCorrection],
                                      MODE_CONVERTOR[mode], 1);
    QrCodeData data;
    data.width = result->width;
    data.data = QByteArray(reinterpret_cast<char *>(result->data), data.width * data.width);
    QRcode_free(result);
    return data;
}

QImage generateBitmap(const QrCodeData &rawData, int width)
{
    QImage image(QSize(rawData.width, rawData.width), QImage::Format_MonoLSB);
    image.fill(1);

    for (int i = 0; i < rawData.width; ++i) {
        for (int j = 0; j < rawData.width; ++j) {
            if (rawData.data[i * rawData.width + j] % 2 != 0)
                image.setPixel(j, i, 0);
        }
    }

    QTransform transform;
    transform.scale(width / rawData.width, width / rawData.width);
    image = image.transformed(transform);
    int resultWidth = width + ((width + 8) % 8);

    QImage resultImage(QSize(resultWidth, resultWidth), QImage::Format_MonoLSB);
    resultImage.fill(1);

    QPainter painter(&resultImage);
    painter.drawImage(QPoint(0, 0), image);
    painter.end();

    return resultImage;
}
} // namespace

QImage QrCodeGenerator::generateBitmap(const QString &string, int width, QrCodeGenerator::Mode mode,
                                       QrCodeGenerator::ErrorCorrection errorCorrection)
{
    return ::generateBitmap(::generateRawQrCode(string, mode, errorCorrection), width);
}

QByteArray QrCodeGenerator::generateEplBinaryData(const QString &string, int width, QrCodeGenerator::Mode mode,
                                                  QrCodeGenerator::ErrorCorrection errorCorrection)
{
    QByteArray result;
    auto bitmap = generateBitmap(string, width, mode, errorCorrection);

    for (int i = 0; i < bitmap.height(); ++i) {
        for (int j = 0; j < bitmap.width() / 8; ++j) {
            qint8 byte = 0;
            for (int bit = 0; bit < 8; ++bit) {
                if (bitmap.pixel(j * 8 + bit, i) == QColor(Qt::white).rgb())
                    byte |= 1 << (7 - bit);
            }
            result.append(byte);
        }
    }

    return result;
}

uint QrCodeGenerator::qHash(QrCodeGenerator::Mode arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}

uint QrCodeGenerator::qHash(QrCodeGenerator::ErrorCorrection arg, uint seed)
{
    return ::qHash(static_cast<int>(arg), seed);
}
