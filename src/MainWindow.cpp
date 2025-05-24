/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class for the Coda text editor.
 *        Provides file handling and menu integration.
 * @author Dario
 */

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QMenuBar>
#include "MainWindow.h"
#include "EditorWidget.h"

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
            setWindowTitle("Coda - " + currentFilePath);
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
