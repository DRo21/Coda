/**
 * @file ConfigManager.h
 * @brief Loads and manages application configuration from a JSON file.
 *        Provides access to settings like shortcuts, theme, and plugins.
 *        Generates a default config if none exists.
 * @author Dario Romandini
 */

#pragma once

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <vector>

/**
 * @class ConfigManager
 * @brief Singleton-like manager for application configuration.
 *        Loads config.json and provides access to key settings.
 */
class ConfigManager {
public:
    /**
     * @brief Constructor. Loads config.json from the provided path.
     *        Generates a default config if the file is missing.
     * @param configPath Path to the config.json file.
     */
    explicit ConfigManager(const QString &configPath);

    /**
     * @brief Get the shortcut for a given action.
     * @param actionName The name of the action.
     * @return The key sequence as a QString, or empty if not set.
     */
    QString getShortcut(const QString &actionName) const;

    /**
     * @brief Get the preferred theme.
     * @return Theme name as a QString.
     */
    QString getTheme() const;

    /**
     * @brief Get the list of plugins to load.
     * @return Vector of plugin file paths.
     */
    std::vector<QString> getPluginList() const;

private:
    QJsonObject config; ///< The parsed JSON configuration.

    /**
     * @brief Generates a default config.json file with sensible defaults.
     * @param configPath Path where the file should be created.
     */
    void generateDefaultConfig(const QString &configPath);
};
