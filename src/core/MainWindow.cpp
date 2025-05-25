/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class for the Coda text editor.
 *        Provides file handling, menu integration, and dynamic syntax highlighting with theme support.
 *        Uses KSyntaxHighlighting for multi-language support with OCP design principles.
 * @author Dario Romandini
 */

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMenuBar>

#include <KSyntaxHighlighting/Repository>

#include "MainWindow.h"
#include "EditorWidget.h"
#include "KSyntaxHighlightingAdapter.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), editor(new EditorWidget(this)) {
    setCentralWidget(editor);
    setWindowTitle("Coda");

    auto *fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("Open", this, &MainWindow::openFile);
    fileMenu->addAction("Save", this, &MainWindow::saveFile);
    fileMenu->addAction("Save As", this, &MainWindow::saveFileAs);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit", this, &QWidget::close);

    auto *viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("Light Theme", this, &MainWindow::setLightTheme);
    viewMenu->addAction("Dark Theme", this, &MainWindow::setDarkTheme);
}

MainWindow::~MainWindow() {}

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

void MainWindow::setLightTheme() {
    auto *highlighter = dynamic_cast<KSyntaxHighlightingAdapter *>(editor->getSyntaxHighlighter());
    if (highlighter) {
        KSyntaxHighlighting::Repository repo;
        highlighter->setTheme(repo.theme("Breeze Light"));
    }
}

void MainWindow::setDarkTheme() {
    auto *highlighter = dynamic_cast<KSyntaxHighlightingAdapter *>(editor->getSyntaxHighlighter());
    if (highlighter) {
        KSyntaxHighlighting::Repository repo;
        highlighter->setTheme(repo.theme("Breeze Dark"));
    }
}
