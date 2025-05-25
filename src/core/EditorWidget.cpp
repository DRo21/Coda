/**
 * @file EditorWidget.cpp
 * @brief Implementation of EditorWidget for Coda. QSS-powered line numbers, current line highlighting, and syntax support.
 * @author Dario Romandini
 */

#include "EditorWidget.h"
#include <QPainter>
#include <QTextBlock>

EditorWidget::LineNumberArea::LineNumberArea(EditorWidget *editorWidget) : QWidget(editorWidget), editor(editorWidget) {}

QSize EditorWidget::LineNumberArea::sizeHint() const {
    return QSize(editor->lineNumberWidth(), 0);
}

void EditorWidget::LineNumberArea::paintEvent(QPaintEvent *event) {
    editor->lineNumberAreaPaintEvent(event);
}

EditorWidget::EditorWidget(QWidget *parent) : QPlainTextEdit(parent) {
    lineNumberArea = new LineNumberArea(this);
    syntaxHighlighter = nullptr;

    connect(this, &QPlainTextEdit::blockCountChanged, this, &EditorWidget::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &EditorWidget::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, &EditorWidget::highlightCurrentLine);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    setLineWrapMode(QPlainTextEdit::NoWrap);
    setFont(QFont("Courier", 12));
}

int EditorWidget::computeLineNumberAreaWidth() const {
    return lineNumberAreaWidthValue > 0 ? lineNumberAreaWidthValue : 40;
}

void EditorWidget::updateLineNumberAreaWidth(int) {
    setViewportMargins(computeLineNumberAreaWidth(), 0, 0, 0);
}

void EditorWidget::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void EditorWidget::resizeEvent(QResizeEvent *event) {
    QPlainTextEdit::resizeEvent(event);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), computeLineNumberAreaWidth(), cr.height()));
}

void EditorWidget::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), lineNumberBgColor);

    painter.setPen(lineNumberFgColor);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.drawText(0, top, lineNumberArea->width() - 5, fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void EditorWidget::highlightCurrentLine() {
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(highlightColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void EditorWidget::setSyntaxHighlighter(ISyntaxHighlighter *highlighter) {
    syntaxHighlighter = highlighter;
    if (syntaxHighlighter) {
        syntaxHighlighter->attachToDocument(document());
    }
}

QString EditorWidget::currentFilePath() const {
    return filePath;
}

void EditorWidget::setCurrentFilePath(const QString &path) {
    filePath = path;
}

ISyntaxHighlighter *EditorWidget::getSyntaxHighlighter() const {
    return syntaxHighlighter;
}

void EditorWidget::setCurrentLineColor(const QColor &color) {
    highlightColor = color;
    highlightCurrentLine();
}

QColor EditorWidget::currentLineColor() const {
    return highlightColor;
}

void EditorWidget::setLineNumberColor(const QColor &color) {
    lineNumberBgColor = color;
    lineNumberArea->update();
}

QColor EditorWidget::lineNumberColor() const {
    return lineNumberBgColor;
}

void EditorWidget::setLineNumberTextColor(const QColor &color) {
    lineNumberFgColor = color;
    lineNumberArea->update();
}

QColor EditorWidget::lineNumberTextColor() const {
    return lineNumberFgColor;
}

void EditorWidget::setLineNumberWidth(int width) {
    if (width > 0) {
        lineNumberAreaWidthValue = width;
        updateLineNumberAreaWidth(0);
    }
}

int EditorWidget::lineNumberWidth() const {
    return lineNumberAreaWidthValue;
}
