/**
 * @file KSyntaxHighlightingAdapter.h
 * @brief Adapter for integrating KSyntaxHighlighting into Coda's OCP architecture via the ISyntaxHighlighter interface.
 *        Provides syntax highlighting for multiple languages based on KDE's syntax definition files.
 *        Dynamically detects language by file path and supports theme changes at runtime.
 * @author Dario Romandini
 */

#pragma once

#include <KSyntaxHighlighting/SyntaxHighlighter>
#include <KSyntaxHighlighting/Repository>
#include <KSyntaxHighlighting/Definition>
#include <KSyntaxHighlighting/Theme>
#include "ISyntaxHighlighter.h"

/**
 * @class KSyntaxHighlightingAdapter
 * @brief Adapts the KSyntaxHighlighting library to the ISyntaxHighlighter interface for use in Coda.
 *        Supports dynamic detection of syntax highlighting definitions based on file path and dynamic theme changes.
 */
class KSyntaxHighlightingAdapter : public KSyntaxHighlighting::SyntaxHighlighter, public ISyntaxHighlighter {
    Q_OBJECT

public:
    /**
     * @brief Constructor for KSyntaxHighlightingAdapter.
     * @param document The QTextDocument to highlight.
     */
    explicit KSyntaxHighlightingAdapter(QTextDocument *document);

    /**
     * @brief Sets the file path for syntax detection.
     * @param filePath The file path to analyze.
     */
    void setFilePath(const QString &filePath);

    /**
     * @brief Returns the name of the language currently highlighted.
     * @return Language name as a QString.
     */
    QString language() const override;

    /**
     * @brief Attaches the highlighter to a QTextDocument.
     * @param document The QTextDocument to highlight.
     */
    void attachToDocument(QTextDocument *document) override;

    /**
     * @brief Sets the syntax highlighting theme.
     * @param theme The KSyntaxHighlighting::Theme to apply.
     */
    void setTheme(const KSyntaxHighlighting::Theme &theme);

private:
    KSyntaxHighlighting::Repository repository;    ///< Repository of syntax definitions.
    KSyntaxHighlighting::Definition definition;    ///< The syntax definition for the detected language.
    QString languageId;                            ///< The name of the detected language.
};
