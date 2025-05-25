/**
 * @file EditorWidget.cpp
 * @brief Implementation of the EditorWidget class for the Coda text editor.
 *        Provides the text editing area functionality with line numbers and syntax highlighting support.
 *        Integrates a LineNumberArea widget for displaying line numbers alongside the text editor.
 *        Supports multiple languages via the ISyntaxHighlighter interface and KSyntaxHighlightingAdapter implementation.
 * @author Dario Romandini
 */

#include "EditorWidget.h"
#include <QPainter>
#include <QTextBlock>

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
    filePath = ""; // Initialize file path
}

int EditorWidget::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void EditorWidget::updateLineNumberAreaWidth(int) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
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
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void EditorWidget::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
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
        selection.format.setBackground(QColor(Qt::yellow).lighter(160));
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    static_cast<EditorWidget *>(parent())->lineNumberAreaPaintEvent(event);
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
