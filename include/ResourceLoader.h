/**
 * @file ResourceLoader.h
 * @brief Utility class for managing and ensuring availability of resource files in Coda.
 *        Automatically creates default resources like themes and example plugins if they are missing.
 * @author Dario Romandini
 */

#pragma once

#include <QString>

class ResourceLoader {
public:
    /**
     * @brief Ensures that essential resource files (themes, example plugins) exist.
     *        If not, generates them with default content.
     */
    static void ensureResources();

private:
    static void ensureThemeFile(const QString &path, const QString &content);
    static void ensureExampleLua(const QString &path);
};
