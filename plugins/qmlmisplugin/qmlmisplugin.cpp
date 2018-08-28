#include "qmlmisplugin.h"

#include "proofnetwork/mis/apihelper.h"
#include "proofnetwork/mis/data/qmlwrappers/jobqmlwrapper.h"

#include <QtQml>

QmlMisPlugin::QmlMisPlugin(QObject *parent) : QQmlExtensionPlugin(parent)
{}

void QmlMisPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Proof.Network.Mis"));
    // clang-format off
    qmlRegisterUncreatableType<Proof::Mis::JobQmlWrapper>(uri, 1, 0, "MisJob", QStringLiteral("Creatable only from C++"));
    qmlRegisterUncreatableMetaObject(Proof::Mis::staticMetaObject, uri, 1, 0, "MisHelper", QStringLiteral("For enums only"));
    // clang-format on
}
