/**
 * @file EditorWidget.h
 * @brief Editor widget for the Coda text editor, based on QPlainTextEdit.
 *        Provides the text editing area functionality with line numbers and syntax highlighting support.
 *        Supports multiple languages via the ISyntaxHighlighter interface and KSyntaxHighlightingAdapter implementation.
 * @author Dario Romandini
 */

#pragma once

#include "ISyntaxHighlighter.h"
#include <QPlainTextEdit>
#include <QWidget>

/**
 * @class LineNumberArea
 * @brief Widget for displaying line numbers alongside the EditorWidget.
 */
class LineNumberArea : public QWidget {
public:
    /**
     * @brief Constructor for LineNumberArea.
     * @param parent Parent widget (typically the EditorWidget).
     */
    explicit LineNumberArea(QWidget *parent = nullptr) : QWidget(parent) {}

    /**
     * @brief Returns the preferred size for the line number area.
     * @return Size hint for the widget.
     */
    QSize sizeHint() const override {
        return QSize(40, 0); ///< Fixed width for line numbers.
    }

protected:
    /**
     * @brief Paint event for the line number area.
     * @param event The paint event.
     */
    void paintEvent(QPaintEvent *event) override;
};

/**
 * @class EditorWidget
 * @brief The text editing area of the Coda text editor.
 * Inherits from QPlainTextEdit and adds a line number margin and syntax highlighting.
 */
class EditorWidget : public QPlainTextEdit {
    Q_OBJECT

public:
    /**
     * @brief Constructor for EditorWidget.
     * @param parent Optional parent widget.
     */
    explicit EditorWidget(QWidget *parent = nullptr);

    /**
     * @brief Paints the line numbers in the line number area.
     * @param event The paint event.
     */
    void lineNumberAreaPaintEvent(QPaintEvent *event);

    /**
     * @brief Sets the syntax highlighter.
     * @param highlighter Pointer to an ISyntaxHighlighter implementation.
     */
    void setSyntaxHighlighter(ISyntaxHighlighter *highlighter);

    /**
     * @brief Gets the current file path (for language detection, etc.).
     * @return File path as a QString.
     */
    QString currentFilePath() const;

    /**
     * @brief Sets the current file path (used for determining the file type and highlighter).
     * @param path The new file path.
     */
    void setCurrentFilePath(const QString &path);

    /**
    * @brief Returns the currently attached syntax highlighter.
    * @return Pointer to the ISyntaxHighlighter.
    */
    ISyntaxHighlighter *getSyntaxHighlighter() const;

protected:
    /**
     * @brief Handles resizing of the editor widget and adjusts the line number area.
     * @param event The resize event.
     */
    void resizeEvent(QResizeEvent *event) override;

private slots:
    /**
     * @brief Updates the width of the line number area when the number of blocks changes.
     * @param newBlockCount The new number of blocks (lines) in the document.
     */
    void updateLineNumberAreaWidth(int newBlockCount);

    /**
     * @brief Highlights the current line in the editor.
     */
    void highlightCurrentLine();

    /**
     * @brief Updates the line number area when the editor content changes.
     * @param rect The area to update.
     * @param dy The vertical scroll offset.
     */
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea; ///< Widget for displaying line numbers.
    ISyntaxHighlighter *syntaxHighlighter; ///< The syntax highlighter used by the editor.
    QString filePath; ///< Path of the currently opened file.

    /**
     * @brief Computes the width of the line number area.
     * @return The width in pixels.
     */
    int lineNumberAreaWidth();
};
