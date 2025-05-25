/**
 * @file PluginManager.h
 * @brief Manages Lua plugin loading and execution based on a JSON config file.
 * @author Dario Romandini
 */

#pragma once

#include <QString>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include "ScriptingEngine.h"

/**
 * @class PluginManager
 * @brief Loads Lua plugin definitions from a JSON config and executes enabled plugins.
 */
class PluginManager {
public:
    /**
     * @brief Constructor for PluginManager.
     * @param engine The scripting engine to use for executing plugins.
     */
    explicit PluginManager(ScriptingEngine *engine);

    /**
     * @brief Loads plugins from a JSON configuration file.
     * @param jsonPath Path to the JSON config file.
     */
    void loadPlugins(const QString &jsonPath);

private:
    ScriptingEngine *scriptingEngine; ///< The Lua scripting engine used for running plugins.
};
