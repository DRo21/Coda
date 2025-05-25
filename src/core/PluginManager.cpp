/**
 * @file PluginManager.cpp
 * @brief Implementation of the PluginManager class for Coda. Loads and executes Lua plugins based on JSON configuration.
 * @author Dario Romandini
 */

#include "PluginManager.h"
#include <QFile>
#include <QDebug>

PluginManager::PluginManager(ScriptingEngine *engine)
    : scriptingEngine(engine) {}

void PluginManager::loadPlugins(const QString &jsonPath) {
    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open plugin config:" << jsonPath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "Invalid plugin config format.";
        return;
    }

    QJsonArray plugins = doc.object().value("plugins").toArray();
    for (const QJsonValue &pluginVal : plugins) {
        QJsonObject pluginObj = pluginVal.toObject();
        QString file = pluginObj.value("file").toString();
        bool enabled = pluginObj.value("enabled").toBool();

        qInfo() << "Loading plugin:" << file << "Enabled:" << enabled;
        if (enabled) {
            scriptingEngine->runScript(file.toStdString());
        }
    }
}
