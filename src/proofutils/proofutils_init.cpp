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
#include "proofcore/coreapplication.h"
#include "proofcore/helpers/versionhelper.h"
#include "proofcore/proofglobal.h"
#include "proofcore/settings.h"
#include "proofcore/settingsgroup.h"

#include "proofutils/proofutils_global.h"

Q_LOGGING_CATEGORY(proofUtilsMiscLog, "proof.utils.misc")
Q_LOGGING_CATEGORY(proofUtilsEplGeneratorLog, "proof.utils.eplgenerator")
Q_LOGGING_CATEGORY(proofUtilsQrCodeGeneratorLog, "proof.utils.qrcodegenerator")
Q_LOGGING_CATEGORY(proofUtilsLprPrinterInfoLog, "proof.utils.lprprinter.info")
Q_LOGGING_CATEGORY(proofUtilsLprPrinterDataLog, "proof.utils.lprprinter.data")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    // clang-format off
    // clang-format on

    //label printer migration
    Proof::CoreApplication::addMigration(Proof::packVersion(0, 17, 12, 25), [](quint64, quint64 oldProofVersion,
                                                                               Proof::Settings *settings) {
        if (oldProofVersion >= Proof::packVersion(0, 17, 12, 25))
            return;

        auto printerGroup = settings->group(QStringLiteral("label_printer"), Proof::Settings::NotFoundPolicy::DoNothing);

        if (!printerGroup)
            return;

        auto commonPrinters = settings->mainGroup()
                                  ->value(QStringLiteral("label_printers"), "", Proof::Settings::NotFoundPolicy::DoNothing)
                                  .toString()
                                  .split(QStringLiteral("|"), QString::SkipEmptyParts);

        auto name = printerGroup->value(QStringLiteral("name"), "", Proof::Settings::NotFoundPolicy::DoNothing).toString();
        if (name.isEmpty()) {
            settings->deleteGroup(QStringLiteral("label_printer"), Proof::Settings::Storage::Local);
            return;
        }

        settings->mainGroup()->setValue(QStringLiteral("selected_printer"), name, Proof::Settings::Storage::Local);

        if (commonPrinters.contains(name)) {
            settings->deleteGroup(QStringLiteral("label_printer"), Proof::Settings::Storage::Local);
            return;
        }

        commonPrinters << name;
        settings->mainGroup()->setValue(QStringLiteral("label_printers"), commonPrinters.join(QStringLiteral("|")),
                                        Proof::Settings::Storage::Global);
        auto newPrinterGroup = settings->group(name, Proof::Settings::NotFoundPolicy::AddGlobal);
        auto host = printerGroup->value(QStringLiteral("host"), "", Proof::Settings::NotFoundPolicy::DoNothing).toString();
        auto port = printerGroup->value(QStringLiteral("port"), "", Proof::Settings::NotFoundPolicy::DoNothing).toInt();
        auto binariesCheck =
            printerGroup->value(QStringLiteral("binaries_check"), "", Proof::Settings::NotFoundPolicy::DoNothing).toBool();
        auto forceServiceUsage = printerGroup
                                     ->value(QStringLiteral("force_service_usage"), "",
                                             Proof::Settings::NotFoundPolicy::DoNothing)
                                     .toBool();

        newPrinterGroup->setValue(QStringLiteral("name"), name);
        newPrinterGroup->setValue(QStringLiteral("title"), name);
        newPrinterGroup->setValue(QStringLiteral("host"), host);
        newPrinterGroup->setValue(QStringLiteral("port"), port);
        newPrinterGroup->setValue(QStringLiteral("binaries_check"), binariesCheck);
        newPrinterGroup->setValue(QStringLiteral("force_service_usage"), forceServiceUsage);

        settings->deleteGroup(QStringLiteral("label_printer"), Proof::Settings::Storage::Local);
    });
}
