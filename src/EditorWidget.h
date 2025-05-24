/**
 * @file EditorWidget.h
 * @brief Editor widget for the Coda text editor, based on QPlainTextEdit.
 *        Provides the basic text editing area functionality.
 * @author Dario
 */

#pragma once

#include <QPlainTextEdit>

/**
 * @class EditorWidget
 * @brief The text editing area of the Coda text editor.
 * Inherits from QPlainTextEdit.
 */
class EditorWidget : public QPlainTextEdit {
    Q_OBJECT

public:
    /**
     * @brief Constructor for EditorWidget.
     * @param parent Optional parent widget.
     */
    explicit EditorWidget(QWidget *parent = nullptr);
};
