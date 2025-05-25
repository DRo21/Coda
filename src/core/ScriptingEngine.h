/**
 * @file ScriptingEngine.h
 * @brief Wrapper for the embedded Lua scripting engine in Coda.
 *        Enables plugin development using Lua scripts via the Open/Closed Principle architecture.
 * @author Dario Romandini
 */

#pragma once

#include <sol/sol.hpp>
#include <string>

class EditorWidget;

/**
 * @class ScriptingEngine
 * @brief Manages the embedded Lua scripting engine and script execution.
 *        Exposes core editor functionality for plugins via Lua.
 */
class ScriptingEngine {
public:
    /**
     * @brief Constructor. Initializes the Lua state.
     * @param editor Pointer to the EditorWidget for API exposure.
     */
    explicit ScriptingEngine(EditorWidget *editor);

    /**
     * @brief Executes a Lua script file.
     * @param path Path to the Lua script.
     */
    void runScript(const std::string &path);

    /**
     * @brief Access the Lua state for custom extensions.
     * @return Reference to the Lua state.
     */
    sol::state &getLua();

private:
    void registerCodaAPI();

    sol::state lua;          ///< Lua interpreter state.
    EditorWidget *editor;    ///< Editor widget for text manipulation.
};
