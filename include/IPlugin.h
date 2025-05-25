/**
 * @file IPlugin.h
 * @brief Abstract interface for Coda plugins. Defines the basic plugin structure for extensions.
 * @author Dario Romandini
 */

#pragma once

#include <QString>

/**
 * @class IPlugin
 * @brief Abstract interface for Coda plugins. All plugins must implement this interface.
 */
class IPlugin {
public:
    virtual ~IPlugin() = default;

    /**
     * @brief Returns the name of the plugin.
     * @return Plugin name as a QString.
     */
    virtual QString name() const = 0;

    /**
     * @brief Initializes the plugin.
     */
    virtual void initialize() = 0;
};
