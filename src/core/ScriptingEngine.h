/**
 * @file ScriptingEngine.h
 * @brief Wrapper for the embedded Lua scripting engine in Coda.
 *        Enables plugin development using Lua scripts via the Open/Closed Principle architecture.
 * @author Dario Romandini
 */

#pragma once

#include <sol/sol.hpp>

/**
 * @class ScriptingEngine
 * @brief Manages the embedded Lua scripting engine and script execution.
 */
class ScriptingEngine {
public:
    /**
     * @brief Constructor. Initializes the Lua state.
     */
    ScriptingEngine();

    /**
     * @brief Executes a Lua script file.
     * @param path Path to the Lua script.
     */
    void runScript(const std::string &path);

    /**
     * @brief Exposes core Coda API functions to Lua.
     */
    void registerCodaAPI();

private:
    sol::state lua; ///< Lua interpreter state.
};
