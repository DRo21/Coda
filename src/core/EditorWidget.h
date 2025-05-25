/**
 * @file EditorWidget.h
 * @brief Editor widget for the Coda text editor, based on QPlainTextEdit.
 *        Provides text editing, line numbers, syntax highlighting, and full QSS customization.
 *        QSS properties: currentLineColor, lineNumberColor, lineNumberTextColor, lineNumberWidth.
 * @author Dario Romandini
 */

#pragma once

#include "ISyntaxHighlighter.h"
#include <QPlainTextEdit>
#include <QWidget>
#include <QColor>

/**
 * @class EditorWidget
 * @brief The text editing area of the Coda text editor.
 *        Inherits from QPlainTextEdit, adds line numbers, syntax highlighting, and QSS theming support.
 */
class EditorWidget : public QPlainTextEdit {
    Q_OBJECT
    Q_PROPERTY(QColor currentLineColor READ currentLineColor WRITE setCurrentLineColor)
    Q_PROPERTY(QColor lineNumberColor READ lineNumberColor WRITE setLineNumberColor)
    Q_PROPERTY(QColor lineNumberTextColor READ lineNumberTextColor WRITE setLineNumberTextColor)
    Q_PROPERTY(int lineNumberWidth READ lineNumberWidth WRITE setLineNumberWidth)

public:
    explicit EditorWidget(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void setSyntaxHighlighter(ISyntaxHighlighter *highlighter);
    QString currentFilePath() const;
    void setCurrentFilePath(const QString &path);
    ISyntaxHighlighter *getSyntaxHighlighter() const;

    // Property accessors for QSS
    void setCurrentLineColor(const QColor &color);
    QColor currentLineColor() const;

    void setLineNumberColor(const QColor &color);
    QColor lineNumberColor() const;

    void setLineNumberTextColor(const QColor &color);
    QColor lineNumberTextColor() const;

    void setLineNumberWidth(int width);
    int lineNumberWidth() const;

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    class LineNumberArea : public QWidget {
    public:
        explicit LineNumberArea(EditorWidget *editor);
        QSize sizeHint() const override;
    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        EditorWidget *editor;
    };

    LineNumberArea *lineNumberArea;
    ISyntaxHighlighter *syntaxHighlighter;
    QString filePath;

    QColor highlightColor = QColor("#44475a");
    QColor lineNumberBgColor = QColor("#2E2E2E");
    QColor lineNumberFgColor = QColor("#CCCCCC");
    int lineNumberAreaWidthValue = 40;

    int computeLineNumberAreaWidth() const;
};
