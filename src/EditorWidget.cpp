/**
 * @file EditorWidget.cpp
 * @brief Implementation of the EditorWidget class for the Coda text editor.
 *        Provides the basic text editing area functionality.
 * @author Dario
 */

#include "EditorWidget.h"

EditorWidget::EditorWidget(QWidget *parent) : QPlainTextEdit(parent) {
    setLineWrapMode(QPlainTextEdit::NoWrap);
    setFont(QFont("Courier", 12));
}
