#include "networkconfigurationmanager.h"
#include "proofutils_global.h"

#include "proofcore/coreapplication.h"
#include "proofcore/helpers/versionhelper.h"
#include "proofcore/proofglobal.h"
#include "proofcore/settings.h"
#include "proofcore/settingsgroup.h"

Q_LOGGING_CATEGORY(proofUtilsMiscLog, "proof.utils.misc")
Q_LOGGING_CATEGORY(proofUtilsEplGeneratorLog, "proof.utils.eplgenerator")
Q_LOGGING_CATEGORY(proofUtilsQrCodeGeneratorLog, "proof.utils.qrcodegenerator")
Q_LOGGING_CATEGORY(proofUtilsNetworkConfigurationLog, "proof.utils.networkconfiguration")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    // clang-format off
    qRegisterMetaType<Proof::NetworkConfigurationManager::ProxyType>("Proof::NetworkConfigurationManager::ProxyType");
    qRegisterMetaType<QVector<Proof::NetworkConfigurationManager::ProxyType>>("QVector<Proof::NetworkConfigurationManager::ProxyType>");
    // clang-format on

    //label printer migration
    Proof::CoreApplication::addMigration(Proof::packVersion(0, 17, 12, 25), [](quint64, quint64 oldProofVersion,
                                                                               Proof::Settings *settings) {
        if (oldProofVersion >= Proof::packVersion(0, 17, 12, 25))
            return;

        auto printerGroup = settings->group("label_printer", Proof::Settings::NotFoundPolicy::DoNothing);

        if (!printerGroup)
            return;

        auto commonPrinters = settings->mainGroup()
                                  ->value("label_printers", "", Proof::Settings::NotFoundPolicy::DoNothing)
                                  .toString()
                                  .split("|", QString::SkipEmptyParts);

        auto name = printerGroup->value("name", "", Proof::Settings::NotFoundPolicy::DoNothing).toString();
        if (name.isEmpty()) {
            settings->deleteGroup("label_printer", Proof::Settings::Storage::Local);
            return;
        }

        settings->mainGroup()->setValue("selected_printer", name, Proof::Settings::Storage::Local);

        if (commonPrinters.contains(name)) {
            settings->deleteGroup("label_printer", Proof::Settings::Storage::Local);
            return;
        }

        commonPrinters << name;
        settings->mainGroup()->setValue("label_printers", commonPrinters.join("|"), Proof::Settings::Storage::Global);
        auto newPrinterGroup = settings->group(name, Proof::Settings::NotFoundPolicy::AddGlobal);
        auto host = printerGroup->value("host", "", Proof::Settings::NotFoundPolicy::DoNothing).toString();
        auto port = printerGroup->value("port", "", Proof::Settings::NotFoundPolicy::DoNothing).toInt();
        auto binariesCheck = printerGroup->value("binaries_check", "", Proof::Settings::NotFoundPolicy::DoNothing).toBool();
        auto forceServiceUsage =
            printerGroup->value("force_service_usage", "", Proof::Settings::NotFoundPolicy::DoNothing).toBool();

        newPrinterGroup->setValue("name", name);
        newPrinterGroup->setValue("title", name);
        newPrinterGroup->setValue("host", host);
        newPrinterGroup->setValue("port", port);
        newPrinterGroup->setValue("binaries_check", binariesCheck);
        newPrinterGroup->setValue("force_service_usage", forceServiceUsage);

        settings->deleteGroup("label_printer", Proof::Settings::Storage::Local);
    });
}
