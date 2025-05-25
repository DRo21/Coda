/**
 * @file ScriptingEngine.cpp
 * @brief Implementation of the ScriptingEngine class for Coda. Integrates Lua scripting capabilities via sol2 library.
 *        Supports event-driven plugin execution and exposes editor API functions.
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

void ScriptingEngine::triggerEvent(const std::string &eventName, const std::string &filePath) {
    sol::function handler = lua[eventName];
    if (handler.valid()) {
        try {
            handler(filePath);
        } catch (const sol::error &e) {
            std::cerr << "Lua error in " << eventName << ": " << e.what() << std::endl;
        }
    }
}

void ScriptingEngine::registerCodaAPI() {
    lua["Coda"] = lua.create_table();

    lua["Coda"]["showMessage"] = [](const std::string &msg) {
        std::cout << "[Coda] " << msg << std::endl;
    };

    lua["editor"] = lua.create_table();

    lua["editor"]["getText"] = [this]() {
        return editor->toPlainText().toStdString();
    };

    lua["editor"]["setText"] = [this](const std::string &text) {
        editor->setPlainText(QString::fromStdString(text));
    };

    lua["editor"]["getCursorPosition"] = [this]() {
        QTextCursor cursor = editor->textCursor();
        return std::make_tuple(cursor.blockNumber() + 1, cursor.columnNumber() + 1);
    };

    lua["editor"]["setCursorPosition"] = [this](int line, int column) {
        QTextCursor cursor = editor->textCursor();
        QTextBlock block = editor->document()->findBlockByNumber(line - 1);
        if (block.isValid()) {
            cursor.setPosition(block.position() + column - 1);
            editor->setTextCursor(cursor);
        }
    };

    lua["editor"]["getSelection"] = [this]() {
        return editor->textCursor().selectedText().toStdString();
    };

    lua["editor"]["replaceSelection"] = [this](const std::string &text) {
        QTextCursor cursor = editor->textCursor();
        cursor.insertText(QString::fromStdString(text));
    };

    lua["editor"]["insertTextAt"] = [this](int line, int column, const std::string &text) {
        QTextBlock block = editor->document()->findBlockByNumber(line - 1);
        if (block.isValid()) {
            QTextCursor cursor = editor->textCursor();
            cursor.setPosition(block.position() + column - 1);
            cursor.insertText(QString::fromStdString(text));
        }
    };
}

sol::state &ScriptingEngine::getLua() {
    return lua;
}
