/**
 * @file MainWindow.h
 * @brief Main window class for the Coda text editor.
 * @author Dario
 */

#pragma once

#include <QMainWindow>

class EditorWidget;

/**
 * @class MainWindow
 * @brief The main application window of the Coda text editor.
 * Inherits from QMainWindow and manages file operations.
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
     * @brief Opens a text file.
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

private:
    EditorWidget *editor; /**< The text editor widget. */
    QString currentFilePath; /**< The current file's path. */
};
