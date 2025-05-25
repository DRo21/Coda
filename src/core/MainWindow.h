/**
 * @file MainWindow.h
 * @brief Main window class for the Coda text editor.
 *        Provides file handling, menu integration, syntax highlighting,
 *        Lua scripting support, theme loading, embedded terminal, and shortcut management via ShortcutManager.
 * @author Dario Romandini
 */

#pragma once

#include <QMainWindow>
#include <QString>
#include <QSplitter>

#include "ScriptingEngine.h"
#include "PluginManager.h"
#include "ConfigManager.h"
#include "ShortcutManager.h"

class EditorWidget;
class QTermWidget;

/**
 * @class MainWindow
 * @brief The main application window of the Coda text editor.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void closeFile();

    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();

    void runLuaScript();
    void applyDarkTheme();
    void applyLightTheme();

    void toggleTerminal();  ///< Show/hide the embedded terminal

private:
    void setupMenus();
    void setupShortcuts();
    void applyTheme(const QString &themeName);

    EditorWidget *editor;
    QTermWidget *terminal;            ///< Embedded terminal widget (QTermWidget)
    QSplitter *splitter;              ///< Splitter for resizing editor/terminal
    QString currentFilePath;
    ScriptingEngine *scriptingEngine;
    PluginManager *pluginManager;
    ConfigManager *configManager;
    ShortcutManager *shortcutManager;
};
