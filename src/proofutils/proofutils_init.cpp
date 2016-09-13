#include "proofutils_global.h"

Q_LOGGING_CATEGORY(proofUtilsMiscLog, "proof.utils.misc")
Q_LOGGING_CATEGORY(proofUtilsEplGeneratorLog, "proof.utils.eplgenerator")
Q_LOGGING_CATEGORY(proofUtilsQrCodeGeneratorLog, "proof.utils.qrcodegenerator")
Q_LOGGING_CATEGORY(proofUtilsNetworkConfigurationLog, "proof.utils.networkconfiguration")

__attribute__((constructor))
static void libraryInit()
{
}
