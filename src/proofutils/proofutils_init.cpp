#include "proofutils_global.h"
#include "proofcore/proofglobal.h"
#include "networkconfigurationmanager.h"

Q_LOGGING_CATEGORY(proofUtilsMiscLog, "proof.utils.misc")
Q_LOGGING_CATEGORY(proofUtilsEplGeneratorLog, "proof.utils.eplgenerator")
Q_LOGGING_CATEGORY(proofUtilsQrCodeGeneratorLog, "proof.utils.qrcodegenerator")
Q_LOGGING_CATEGORY(proofUtilsNetworkConfigurationLog, "proof.utils.networkconfiguration")

PROOF_LIBRARY_INITIALIZER(libraryInit)
{
    //clang-format off
    qRegisterMetaType<Proof::NetworkConfigurationManager::ProxyType>("Proof::NetworkConfigurationManager::ProxyType");
    qRegisterMetaType<QList<Proof::NetworkConfigurationManager::ProxyType>>("QList<Proof::NetworkConfigurationManager::ProxyType>");
    //clang-format on
}
