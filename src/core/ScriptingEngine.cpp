/**
 * @file ScriptingEngine.cpp
 * @brief Implementation of the ScriptingEngine class for Coda. Integrates Lua scripting capabilities via sol2 library.
 * @author Dario Romandini
 */

#include "ScriptingEngine.h"
#include <iostream>

ScriptingEngine::ScriptingEngine() {
    lua.open_libraries(sol::lib::base, sol::lib::package);
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
    lua["Coda"]["showMessage"] = [](const std::string &msg) {
        std::cout << "[Coda] " << msg << std::endl;
    };
}
