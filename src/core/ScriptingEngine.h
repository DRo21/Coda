/**
 * @file ScriptingEngine.h
 * @brief Wrapper for the embedded Lua scripting engine in Coda.
 *        Enables plugin development using Lua scripts via the Open/Closed Principle architecture.
 *        Supports event-driven plugin execution and exposes editor API features.
 * @author Dario Romandini
 */

#pragma once

#include <sol/sol.hpp>
#include <string>

class EditorWidget;

/**
 * @class ScriptingEngine
 * @brief Manages the embedded Lua scripting engine and script execution.
 *        Exposes core editor functionality for plugins via Lua and supports event-driven triggers.
 */
class ScriptingEngine {
public:
    /**
     * @brief Constructor. Initializes the Lua state and registers the Coda API.
     * @param editor Pointer to the EditorWidget for text manipulation.
     */
    explicit ScriptingEngine(EditorWidget *editor);

    /**
     * @brief Executes a Lua script file.
     * @param path Path to the Lua script.
     */
    void runScript(const std::string &path);

    /**
     * @brief Triggers a Lua event handler by name (e.g., "onFileOpen").
     * @param eventName The name of the event (Lua function name).
     * @param filePath The file path to pass as an argument (optional).
     */
    void triggerEvent(const std::string &eventName, const std::string &filePath = "");

    /**
     * @brief Access the Lua state for custom extensions.
     * @return Reference to the Lua state.
     */
    sol::state &getLua();

private:
    /**
     * @brief Registers the Coda API functions and editor methods into the Lua state.
     */
    void registerCodaAPI();

    sol::state lua;          ///< Lua interpreter state.
    EditorWidget *editor;    ///< Editor widget for text manipulation.
};
