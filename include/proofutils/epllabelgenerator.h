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

    explicit EplLabelGenerator(int printerDpi = 203);
    EplLabelGenerator(const EplLabelGenerator &other) = delete;
    EplLabelGenerator &operator=(const EplLabelGenerator &other) = delete;
    EplLabelGenerator(EplLabelGenerator &&other) = delete;
    EplLabelGenerator &operator=(EplLabelGenerator &&other) = delete;
    virtual ~EplLabelGenerator();

    void startLabel(int width = 795, int height = 1250, int speed = 4, int density = 10, int gapLength = 24);

    QRect addText(const QString &text, int x, int y, int fontSize = 4, int horizontalScale = 1, int verticalScale = 1,
                  int rotation = 0, bool inverseColors = false);

    QSize textSize(const QString &text, int fontSize = 4, int horizontalScale = 1, int verticalScale = 1) const;
    QSize labelSize() const;

    QRect addBarcode(const QString &data, BarcodeType type, int x, int y, int height = 200,
                     bool printReadableCode = true, int narrowBarWidth = 2, int wideBarWidth = 4, int rotation = 0);

    QRect addQrCode(const QString &data, int x, int y, int width = 200);

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
