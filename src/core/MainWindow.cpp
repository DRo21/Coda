/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class for the Coda text editor.
 *        Provides file handling, menu integration, syntax highlighting,
 *        Lua scripting, theming, and shortcut management via ShortcutManager.
 * @author Dario Romandini
 */

#include "MainWindow.h"
#include "EditorWidget.h"
#include "KSyntaxHighlightingAdapter.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMenuBar>
#include <QStandardPaths>
#include <QApplication>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      editor(new EditorWidget(this)),
      scriptingEngine(new ScriptingEngine(editor)),
      configManager(new ConfigManager(
          QStandardPaths::locate(QStandardPaths::AppConfigLocation, "config.json").isEmpty()
          ? "config.json"
          : QStandardPaths::locate(QStandardPaths::AppConfigLocation, "config.json")
      )),
      shortcutManager(new ShortcutManager(configManager, this)),
      pluginManager(new PluginManager(scriptingEngine)) {

    setCentralWidget(editor);
    setWindowTitle("Coda");
    resize(1200, 800);

    setupMenus();
    setupShortcuts();

    applyTheme(configManager->getTheme());

    for (const auto &pluginPath : configManager->getPluginList()) {
        pluginManager->loadPlugins(pluginPath);
    }
}

MainWindow::~MainWindow() {
    delete pluginManager;
    delete scriptingEngine;
    delete configManager;
    delete shortcutManager;
}

void MainWindow::setupMenus() {
    auto *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("New File", this, &MainWindow::newFile);
    fileMenu->addAction("Open", this, &MainWindow::openFile);
    fileMenu->addAction("Save", this, &MainWindow::saveFile);
    fileMenu->addAction("Save As", this, &MainWindow::saveFileAs);
    fileMenu->addAction("Close File", this, &MainWindow::closeFile);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    auto *editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction("Undo", this, &MainWindow::undo);
    editMenu->addAction("Redo", this, &MainWindow::redo);
    editMenu->addAction("Cut", this, &MainWindow::cut);
    editMenu->addAction("Copy", this, &MainWindow::copy);
    editMenu->addAction("Paste", this, &MainWindow::paste);
    editMenu->addAction("Select All", this, &MainWindow::selectAll);

    auto *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Dark Theme", this, &MainWindow::applyDarkTheme);
    viewMenu->addAction("Light Theme", this, &MainWindow::applyLightTheme);

    auto *toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction("Run Lua Script", this, &MainWindow::runLuaScript);
}

void MainWindow::setupShortcuts() {
    shortcutManager->bindShortcut("newFile", this, &MainWindow::newFile);
    shortcutManager->bindShortcut("open", this, &MainWindow::openFile);
    shortcutManager->bindShortcut("save", this, &MainWindow::saveFile);
    shortcutManager->bindShortcut("saveAs", this, &MainWindow::saveFileAs);
    shortcutManager->bindShortcut("closeFile", this, &MainWindow::closeFile);

    shortcutManager->bindShortcut("undo", this, &MainWindow::undo);
    shortcutManager->bindShortcut("redo", this, &MainWindow::redo);
    shortcutManager->bindShortcut("cut", this, &MainWindow::cut);
    shortcutManager->bindShortcut("copy", this, &MainWindow::copy);
    shortcutManager->bindShortcut("paste", this, &MainWindow::paste);
    shortcutManager->bindShortcut("selectAll", this, &MainWindow::selectAll);

    shortcutManager->bindShortcut("runLuaScript", this, &MainWindow::runLuaScript);
    shortcutManager->bindShortcut("toggleComment", this, []() { qInfo() << "Toggle Comment triggered"; });
    shortcutManager->bindShortcut("duplicateLine", this, []() { qInfo() << "Duplicate Line triggered"; });
    shortcutManager->bindShortcut("moveLineUp", this, []() { qInfo() << "Move Line Up triggered"; });
    shortcutManager->bindShortcut("moveLineDown", this, []() { qInfo() << "Move Line Down triggered"; });
    shortcutManager->bindShortcut("find", this, []() { qInfo() << "Find triggered"; });
    shortcutManager->bindShortcut("findNext", this, []() { qInfo() << "Find Next triggered"; });
    shortcutManager->bindShortcut("findPrevious", this, []() { qInfo() << "Find Previous triggered"; });
    shortcutManager->bindShortcut("replace", this, []() { qInfo() << "Replace triggered"; });
    shortcutManager->bindShortcut("goToLine", this, []() { qInfo() << "Go To Line triggered"; });
}

void MainWindow::newFile() {
    editor->clear();
    currentFilePath.clear();
}

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File");
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            editor->setPlainText(in.readAll());
            currentFilePath = fileName;
            editor->setCurrentFilePath(currentFilePath);
            setWindowTitle("Coda - " + currentFilePath);

            auto *highlighter = new KSyntaxHighlightingAdapter(editor->document());
            highlighter->setFilePath(currentFilePath);
            editor->setSyntaxHighlighter(highlighter);

            scriptingEngine->triggerEvent("onFileOpen", currentFilePath.toStdString());
        } else {
            QMessageBox::warning(this, "Error", "Failed to open file");
        }
    }
}

void MainWindow::saveFile() {
    if (currentFilePath.isEmpty()) {
        saveFileAs();
        return;
    }

    QFile file(currentFilePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << editor->toPlainText();
        file.close();
        setWindowTitle("Coda - " + currentFilePath);

        scriptingEngine->triggerEvent("onFileSave", currentFilePath.toStdString());
    } else {
        QMessageBox::warning(this, "Error", "Failed to save file");
    }
}

void MainWindow::saveFileAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save File As");
    if (!fileName.isEmpty()) {
        currentFilePath = fileName;
        saveFile();
    }
}

void MainWindow::closeFile() {
    editor->clear();
    currentFilePath.clear();
    setWindowTitle("Coda");
}

void MainWindow::undo() { editor->undo(); }
void MainWindow::redo() { editor->redo(); }
void MainWindow::cut() { editor->cut(); }
void MainWindow::copy() { editor->copy(); }
void MainWindow::paste() { editor->paste(); }
void MainWindow::selectAll() { editor->selectAll(); }

void MainWindow::runLuaScript() {
    QString scriptPath = QFileDialog::getOpenFileName(this, "Select Lua Script");
    if (!scriptPath.isEmpty()) {
        scriptingEngine->runScript(scriptPath.toStdString());
    }
}

void MainWindow::applyDarkTheme() { applyTheme("dark"); }
void MainWindow::applyLightTheme() { applyTheme("light"); }

void MainWindow::applyTheme(const QString &themeName) {
    QString themeFile = "styles/" + themeName.toLower() + ".qss";
    QFile file(themeFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString styleSheet = file.readAll();
        qApp->setStyleSheet(styleSheet);
        qInfo() << "Applied theme:" << themeName;
    } else {
        qWarning() << "Theme file not found:" << themeFile;
    }
}
