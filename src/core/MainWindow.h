/**
 * @file MainWindow.h
 * @brief Main window class for the Coda text editor.
 *        Provides file handling, menu integration, dynamic syntax highlighting,
 *        Lua scripting support via JSON-configured plugins, and theme loading.
 * @author Dario Romandini
 */

#pragma once

#include <QMainWindow>
#include <QString>

#include "ScriptingEngine.h"
#include "PluginManager.h"
#include "ConfigManager.h"

class EditorWidget;

/**
 * @class MainWindow
 * @brief The main application window of the Coda text editor.
 *        Inherits from QMainWindow and manages file operations, syntax highlighting,
 *        theme switching via .qss files, and Lua scripting integration.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor for MainWindow.
     * @param parent Optional parent widget.
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor for MainWindow.
     */
    ~MainWindow();

private slots:
    /**
     * @brief Opens a text file and applies appropriate syntax highlighting.
     */
    void openFile();

    /**
     * @brief Saves the current file.
     */
    void saveFile();

    /**
     * @brief Saves the current file as a new file.
     */
    void saveFileAs();

    /**
     * @brief Executes a Lua script selected by the user.
     */
    void runLuaScript();

private:
    /**
     * @brief Applies a stylesheet theme from the 'styles' directory.
     * @param themeName The name of the theme (e.g., "dark", "light").
     */
    void applyTheme(const QString &themeName);

    EditorWidget *editor;             ///< The text editor widget.
    QString currentFilePath;          ///< The current file's path.
    ScriptingEngine *scriptingEngine; ///< The Lua scripting engine.
    PluginManager *pluginManager;     ///< The plugin manager for Lua plugins.
    ConfigManager *configManager;     ///< Configuration manager for app settings (JSON-based).
};
