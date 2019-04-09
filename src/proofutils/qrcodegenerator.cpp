/* Copyright 2018, OpenSoft Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright notice, this list of
 * conditions and the following disclaimer in the documentation and/or other materials provided
 * with the distribution.
 *     * Neither the name of OpenSoft Inc. nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
 * IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "proofutils/qrcodegenerator.h"

#include <QPainter>
#include <qrencode.h>

using namespace Proof;

using ModeDict = QHash<QrCodeGenerator::Mode, QRencodeMode>;
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(ModeDict, MODE_CONVERTOR,
                          ({{QrCodeGenerator::Mode::Numeric, QR_MODE_NUM},
                            {QrCodeGenerator::Mode::AlphaNumeric, QR_MODE_AN},
                            {QrCodeGenerator::Mode::Character, QR_MODE_8}}))

using CorrectionDict = QHash<QrCodeGenerator::ErrorCorrection, QRecLevel>;
// NOLINTNEXTLINE(cppcoreguidelines-special-member-functions)
Q_GLOBAL_STATIC_WITH_ARGS(CorrectionDict, ERROR_CORRECTION_CONVERTOR,
                          ({{QrCodeGenerator::ErrorCorrection::LowLevel, QR_ECLEVEL_L},
                            {QrCodeGenerator::ErrorCorrection::MediumLevel, QR_ECLEVEL_M},
                            {QrCodeGenerator::ErrorCorrection::QuartileLevel, QR_ECLEVEL_Q},
                            {QrCodeGenerator::ErrorCorrection::HighLevel, QR_ECLEVEL_H}}))

namespace {
struct QrCodeData
{
    QByteArray data;
    int width = 0;
};

QrCodeData generateRawQrCode(const QString &string, QrCodeGenerator::Mode mode,
                             QrCodeGenerator::ErrorCorrection errorCorrection)
{
    auto result = QRcode_encodeString(string.toLatin1().constData(), 0, (*ERROR_CORRECTION_CONVERTOR)[errorCorrection],
                                      (*MODE_CONVERTOR)[mode], 1);
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
    transform.scale((double)width / rawData.width, (double)width / rawData.width);
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
            quint8 byte = 0;
            for (unsigned bit = 0; bit < 8; ++bit) {
                if (bitmap.pixel(j * 8 + static_cast<int>(bit), i) == QColor(Qt::white).rgb())
                    byte |= 1u << (7u - bit);
            }
            result.append(static_cast<qint8>(byte));
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
