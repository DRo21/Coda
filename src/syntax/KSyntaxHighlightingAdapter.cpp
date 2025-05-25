/**
 * @file KSyntaxHighlightingAdapter.cpp
 * @brief Implementation of the KSyntaxHighlightingAdapter class for Coda. Bridges KSyntaxHighlighting and ISyntaxHighlighter interface for OCP compliance.
 *        Dynamically detects language based on file path using KDE's syntax definitions.
 *        Sets a default dark theme and allows theme changes at runtime.
 * @author Dario Romandini
 */

#include "KSyntaxHighlightingAdapter.h"
#include <QMimeDatabase>
#include <QFileInfo>
#include <QDebug>

KSyntaxHighlightingAdapter::KSyntaxHighlightingAdapter(QTextDocument *document)
    : KSyntaxHighlighting::SyntaxHighlighter(document) {
    // Set default theme to Breeze Dark
    setTheme(repository.theme("Breeze Dark"));
}

void KSyntaxHighlightingAdapter::setFilePath(const QString &filePath) {
    QMimeDatabase mimeDb;
    QMimeType mime = mimeDb.mimeTypeForFile(filePath);

    definition = repository.definitionForMimeType(mime.name());
    if (!definition.isValid()) {
        definition = repository.definitionForFileName(QFileInfo(filePath).fileName());
    }

    if (definition.isValid()) {
        setDefinition(definition);
        languageId = definition.name();
    } else {
        qWarning() << "No syntax definition found for file:" << filePath;
        languageId = "";
    }
}

QString KSyntaxHighlightingAdapter::language() const {
    return languageId;
}

void KSyntaxHighlightingAdapter::attachToDocument(QTextDocument *document) {
    setDocument(document);
}

void KSyntaxHighlightingAdapter::setTheme(const KSyntaxHighlighting::Theme &theme) {
    SyntaxHighlighter::setTheme(theme);
}
