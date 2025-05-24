/**
 * @file EditorWidget.h
 * @brief Editor widget for the Coda text editor, based on QPlainTextEdit.
 *        Provides the basic text editing area functionality with line numbers.
 * @author Dario
 */

#pragma once

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
 * Inherits from QPlainTextEdit and adds a line number margin.
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

    /**
     * @brief Computes the width of the line number area.
     * @return The width in pixels.
     */
    int lineNumberAreaWidth();
};
