/**
 * @file ConfigManager.cpp
 * @brief Implementation of ConfigManager class. Loads and provides access to JSON configuration for Coda.
 * @author Dario Romandini
 */

#include "ConfigManager.h"
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

ConfigManager::ConfigManager(const QString &configPath)
    : configFilePath(configPath) {
    QFile file(configPath);
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QByteArray data = file.readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                config = doc.object();
                qInfo() << "[ConfigManager] Loaded config from:" << configPath;
            } else {
                qWarning() << "[ConfigManager] Invalid JSON format in config file.";
            }
        } else {
            qWarning() << "[ConfigManager] Could not open config file for reading:" << configPath;
        }
    } else {
        qWarning() << "[ConfigManager] Config file not found, generating default config.";
        generateDefaultConfig(configPath);
    }
}

QString ConfigManager::getShortcut(const QString &actionName) const {
    if (config.contains("shortcuts") && config["shortcuts"].isObject()) {
        return config["shortcuts"].toObject().value(actionName).toString();
    }
    return QString();
}

QString ConfigManager::getTheme() const {
    return config.value("theme").toString("Dark");
}

std::vector<QString> ConfigManager::getPluginList() const {
    std::vector<QString> plugins;
    if (config.contains("plugins") && config["plugins"].isArray()) {
        QJsonArray arr = config["plugins"].toArray();
        for (const auto &entry : arr) {
            if (entry.isObject()) {
                QString file = entry.toObject().value("file").toString();
                bool enabled = entry.toObject().value("enabled").toBool();
                if (enabled && !file.isEmpty()) {
                    plugins.push_back(file);
                }
            }
        }
    }
    return plugins;
}

QString ConfigManager::getConfigPath() const {
    return configFilePath;
}

void ConfigManager::generateDefaultConfig(const QString &configPath) {
    QJsonObject shortcuts;
    shortcuts["open"] = "Ctrl+O";
    shortcuts["save"] = "Ctrl+S";
    shortcuts["saveAs"] = "Ctrl+Shift+S";
    shortcuts["exit"] = "Ctrl+Q";
    shortcuts["runLuaScript"] = "Ctrl+R";
    shortcuts["newFile"] = "Ctrl+N";
    shortcuts["closeFile"] = "Ctrl+W";
    shortcuts["undo"] = "Ctrl+Z";
    shortcuts["redo"] = "Ctrl+Y";
    shortcuts["cut"] = "Ctrl+X";
    shortcuts["copy"] = "Ctrl+C";
    shortcuts["paste"] = "Ctrl+V";
    shortcuts["selectAll"] = "Ctrl+A";
    shortcuts["find"] = "Ctrl+F";
    shortcuts["findNext"] = "F3";
    shortcuts["findPrevious"] = "Shift+F3";
    shortcuts["replace"] = "Ctrl+H";
    shortcuts["goToLine"] = "Ctrl+G";
    shortcuts["toggleComment"] = "Ctrl+/";
    shortcuts["duplicateLine"] = "Ctrl+D";
    shortcuts["moveLineUp"] = "Alt+Up";
    shortcuts["moveLineDown"] = "Alt+Down";
    shortcuts["toggleTerminal"] = "Ctrl+`";

    QJsonArray plugins;
    QJsonObject plugin1;
    plugin1["file"] = "plugins/example.lua";
    plugin1["enabled"] = true;
    plugins.append(plugin1);

    QJsonObject defaultConfig;
    defaultConfig["shortcuts"] = shortcuts;
    defaultConfig["theme"] = "Dark";
    defaultConfig["plugins"] = plugins;

    QJsonDocument doc(defaultConfig);

    QFile file(configPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        qInfo() << "[ConfigManager] Default config created at:" << configPath;
    } else {
        qWarning() << "[ConfigManager] Failed to write default config at:" << configPath;
    }

    config = defaultConfig;
}
