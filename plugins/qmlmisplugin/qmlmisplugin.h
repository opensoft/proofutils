#ifndef QMLMISPLUGIN_H
#define QMLMISPLUGIN_H

#include <QQmlExtensionPlugin>

class QmlMisPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    explicit QmlMisPlugin(QObject *parent = nullptr);
    void registerTypes(const char *uri) override;
};

#endif // QMLMISPLUGIN_H
