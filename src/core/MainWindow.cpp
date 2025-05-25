/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class for the Coda text editor.
 *        Provides file handling, menu integration, dynamic syntax highlighting,
 *        Lua scripting support, and theme switching via .qss files.
 * @author Dario Romandini
 */

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMenuBar>
#include <QStandardPaths>
#include <QApplication>
#include <KSyntaxHighlighting/Repository>

#include "MainWindow.h"
#include "EditorWidget.h"
#include "KSyntaxHighlightingAdapter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      editor(new EditorWidget(this)),
      scriptingEngine(new ScriptingEngine(editor)),
      pluginManager(new PluginManager(scriptingEngine)) {

    setCentralWidget(editor);
    setWindowTitle("Coda");
    resize(1200, 800);

    // Load configuration
    QString configPath = QStandardPaths::locate(QStandardPaths::AppConfigLocation, "config.json");
    if (configPath.isEmpty()) configPath = "config.json";
    configManager = new ConfigManager(configPath);

    // Menus
    auto *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open", this, &MainWindow::openFile, QKeySequence(configManager->getShortcut("open")));
    fileMenu->addAction("Save", this, &MainWindow::saveFile, QKeySequence(configManager->getShortcut("save")));
    fileMenu->addAction("Save As", this, &MainWindow::saveFileAs, QKeySequence(configManager->getShortcut("saveAs")));
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close, QKeySequence(configManager->getShortcut("exit")));

    auto *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Dark Theme", [this]() { applyTheme("dark"); });
    viewMenu->addAction("Light Theme", [this]() { applyTheme("light"); });

    auto *toolsMenu = menuBar()->addMenu("&Tools");
    toolsMenu->addAction("Run Lua Script", this, &MainWindow::runLuaScript, QKeySequence(configManager->getShortcut("runLuaScript")));

    // Apply theme from config
    applyTheme(configManager->getTheme());

    // Load plugins from config.json
    for (const auto &pluginPath : configManager->getPluginList()) {
        pluginManager->loadPlugins(pluginPath);
    }
}

MainWindow::~MainWindow() {
    delete pluginManager;
    delete scriptingEngine;
    delete configManager;
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

void MainWindow::runLuaScript() {
    QString scriptPath = QFileDialog::getOpenFileName(this, "Select Lua Script");
    if (!scriptPath.isEmpty()) {
        scriptingEngine->runScript(scriptPath.toStdString());
    }
}

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
