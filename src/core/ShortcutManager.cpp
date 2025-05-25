/**
 * @file ShortcutManager.cpp
 * @brief Implementation of the ShortcutManager class for the Coda text editor.
 *        Loads shortcuts from the ConfigManager and provides access to them.
 * @author Dario Romandini
 */

#include "ShortcutManager.h"
#include "ConfigManager.h"
#include <QDebug>

ShortcutManager::ShortcutManager(ConfigManager *configManager, QObject *parent)
    : QObject(parent), configManager(configManager) {
    if (!configManager) {
        qWarning() << "[ShortcutManager] ConfigManager is null!";
        return;
    }

    const QStringList keys = {
        "newFile", "open", "save", "saveAs", "closeFile",
        "undo", "redo", "cut", "copy", "paste", "selectAll",
        "runLuaScript", "toggleComment", "duplicateLine",
        "moveLineUp", "moveLineDown", "find", "findNext",
        "findPrevious", "replace", "goToLine", "toggleTerminal"
    };

    for (const QString &key : keys) {
        QString shortcutString = configManager->getShortcut(key);
        if (!shortcutString.isEmpty()) {
            shortcuts[key] = QKeySequence(shortcutString);
            qInfo() << "[ShortcutManager] Loaded shortcut:" << key << "=>" << shortcutString;
        } else {
            qWarning() << "[ShortcutManager] No shortcut defined for action:" << key;
        }
    }
}

QKeySequence ShortcutManager::getShortcut(const QString &actionName) const {
    return shortcuts.value(actionName, QKeySequence());
}
