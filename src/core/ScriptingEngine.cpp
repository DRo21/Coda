/**
 * @file ScriptingEngine.cpp
 * @brief Implementation of the ScriptingEngine class for Coda. Integrates Lua scripting capabilities via sol2 library.
 * @author Dario Romandini
 */

#include "ScriptingEngine.h"
#include "EditorWidget.h"
#include <iostream>

ScriptingEngine::ScriptingEngine(EditorWidget *editor)
    : editor(editor) {
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string);
    registerCodaAPI();
}

void ScriptingEngine::runScript(const std::string &path) {
    try {
        lua.script_file(path);
    } catch (const sol::error &e) {
        std::cerr << "Lua error: " << e.what() << std::endl;
    }
}

void ScriptingEngine::registerCodaAPI() {
    lua["Coda"] = lua.create_table();

    // Simple message output
    lua["Coda"]["showMessage"] = [](const std::string &msg) {
        std::cout << "[Coda] " << msg << std::endl;
    };

    // Expose editor text manipulation
    lua["editor"] = lua.create_table();
    lua["editor"]["getText"] = [this]() {
        return editor->toPlainText().toStdString();
    };
    lua["editor"]["setText"] = [this](const std::string &text) {
        editor->setPlainText(QString::fromStdString(text));
    };
}

sol::state &ScriptingEngine::getLua() {
    return lua;
}
