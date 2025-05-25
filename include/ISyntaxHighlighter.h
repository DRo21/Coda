/**
 * @file ISyntaxHighlighter.h
 * @brief Abstract interface for Coda syntax highlighters. Enables extensible syntax highlighting strategies via OCP design pattern.
 * @author Dario Romandini
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSyntaxHighlighter>

/**
 * @class ISyntaxHighlighter
 * @brief Interface for syntax highlighters in Coda. Supports multiple languages and themes.
 */
class ISyntaxHighlighter {
public:
    virtual ~ISyntaxHighlighter() = default;

    /**
     * @brief Returns the language name supported by this highlighter (e.g., "C++", "Python").
     * @return Language name as a QString.
     */
    virtual QString language() const = 0;

    /**
     * @brief Attaches the highlighter to a QTextDocument.
     * @param document The QTextDocument to highlight.
     */
    virtual void attachToDocument(QTextDocument *document) = 0;
};
