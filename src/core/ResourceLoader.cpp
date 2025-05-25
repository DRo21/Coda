/**
 * @file ResourceLoader.cpp
 * @brief Implementation of the ResourceLoader for Coda.
 *        Creates default themes and example Lua plugins if missing.
 * @author Dario Romandini
 */

#include "ResourceLoader.h"
#include <QFile>
#include <QDir>
#include <QDebug>

void ResourceLoader::ensureResources() {
    QDir().mkpath("styles");
    QDir().mkpath("plugins");

    ensureThemeFile("styles/dark.qss",
R"(
/* Dark Theme for Coda */

QMainWindow {
    background-color: #1E1E2F;
    color: #E0E0E0;
    font-family: "Fira Code", "Courier New", monospace;
    font-size: 12pt;
}

EditorWidget {
    background-color: #1E1E2F;
    color: #CCCCCC;
    selection-background-color: #555577;
    selection-color: #FFFFFF;
    qproperty-currentLineColor: #3A2E4E; /* Purple highlight for current line */
}

LineNumberArea {
    qproperty-backgroundColor: #2A2A3A;   /* Dark gray background for line numbers */
    qproperty-lineNumberTextColor: #AAAAAA; /* Light gray text for line numbers */
    qproperty-lineNumberWidth: 50;
}

/* Menus */
QMenuBar {
    background-color: #252536;
    color: #E0E0E0;
}

QMenuBar::item:selected {
    background-color: #444460;
}

QMenu {
    background-color: #252536;
    color: #E0E0E0;
}

QMenu::item:selected {
    background-color: #444460;
}

/* Buttons */
QPushButton {
    background-color: #333355;
    color: #E0E0E0;
    border: 1px solid #444460;
    padding: 4px 8px;
    border-radius: 4px;
}

QPushButton:hover {
    background-color: #444460;
}

/* Scrollbars */
QScrollBar:vertical, QScrollBar:horizontal {
    background: #252536;
}

QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
    background: #444460;
    border-radius: 4px;
}

QScrollBar::add-line, QScrollBar::sub-line {
    background: none;
    border: none;
}

/* Status bar */
QStatusBar, QToolBar {
    background-color: #252536;
    color: #CCCCCC;
}

QToolButton {
    background-color: transparent;
    color: #CCCCCC;
}

QToolButton:hover {
    background-color: #333355;
}
)");

    ensureThemeFile("styles/light.qss",
R"(
/* Light Theme for Coda */

/* Base Application Styling */
QMainWindow {
    background-color: #fefefe;
    color: #2c3e50;
    font-family: "Segoe UI", "Helvetica Neue", sans-serif;
    font-size: 13px;
}

/* Editor Widget */
EditorWidget {
    background-color: #ffffff;
    color: #2c3e50;
    selection-background-color: #a8dadc;
    selection-color: #1d3557;
    qproperty-currentLineColor: #e1f5fe; /* Light blue highlight */
}

/* Line Number Area */
LineNumberArea {
    qproperty-backgroundColor: #f0f0f0; /* Light grey background */
    qproperty-lineNumberTextColor: #7f8c8d; /* Grey text */
    qproperty-lineNumberWidth: 50; /* Slightly wider */
}

/* Menus */
QMenuBar {
    background-color: #f7f7f7;
    color: #2c3e50;
}

QMenuBar::item:selected {
    background-color: #dfe6e9;
    color: #1d3557;
}

QMenu {
    background-color: #ffffff;
    color: #2c3e50;
}

QMenu::item:selected {
    background-color: #dfe6e9;
    color: #1d3557;
}

/* Buttons */
QPushButton {
    background-color: #e1f5fe;
    border: 1px solid #81d4fa;
    border-radius: 4px;
    padding: 4px 8px;
}

QPushButton:hover {
    background-color: #b3e5fc;
}

QPushButton:pressed {
    background-color: #81d4fa;
}

/* Scrollbars */
QScrollBar:vertical, QScrollBar:horizontal {
    background: #ecf0f1;
}

QScrollBar::handle {
    background: #bdc3c7;
    border-radius: 4px;
}

QScrollBar::handle:hover {
    background: #95a5a6;
}

/* Status Bar */
QStatusBar {
    background: #f0f0f0;
    color: #2c3e50;
}
)");

    ensureExampleLua("plugins/example.lua");
}

void ResourceLoader::ensureThemeFile(const QString &path, const QString &content) {
    if (!QFile::exists(path)) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(content.toUtf8());
            file.close();
            qInfo() << "Created default theme:" << path;
        } else {
            qWarning() << "Failed to create theme file:" << path;
        }
    }
}

void ResourceLoader::ensureExampleLua(const QString &path) {
    if (!QFile::exists(path)) {
        QFile file(path);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(R"(-- example.lua: Lua plugin for Coda

-- Called when a file is opened
function onFileOpen(path)
    Coda.showMessage("Opened file: " .. path)
end

-- Called when a file is saved
function onFileSave(path)
    Coda.showMessage("Saved file: " .. path)
end
)");
            file.close();
            qInfo() << "Created example Lua plugin:" << path;
        } else {
            qWarning() << "Failed to create example Lua plugin:" << path;
        }
    }
}
