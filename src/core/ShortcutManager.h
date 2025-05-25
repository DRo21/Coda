/**
 * @file ShortcutManager.h
 * @brief Manages keyboard shortcuts for the Coda text editor.
 *        Loads shortcuts from a JSON config and provides OCP-compliant action binding.
 * @author Dario Romandini
 */

#pragma once

#include <QObject>
#include <QKeySequence>
#include <QMap>
#include <QAction>
#include <QString>
#include <QWidget>
#include <QMainWindow>
#include <QDebug>
#include <type_traits>

class ConfigManager;

/**
 * @class ShortcutManager
 * @brief Provides centralized management of keyboard shortcuts.
 *        Loads shortcut mappings from a JSON file for OCP-compliant, modular shortcut handling.
 */
class ShortcutManager : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param configManager Pointer to the ConfigManager to retrieve shortcut keys.
     * @param parent Optional parent QObject.
     */
    explicit ShortcutManager(ConfigManager *configManager, QObject *parent = nullptr);

    /**
     * @brief Get the shortcut for a specific action.
     * @param actionName Name of the action.
     * @return QKeySequence for the action, or an empty QKeySequence if not found.
     */
    QKeySequence getShortcut(const QString &actionName) const;

    /**
     * @brief Binds a shortcut to a slot in the target QObject.
     *        Supports OCP by allowing external binding without modifying this class.
     * @tparam T The type of the receiver (e.g., MainWindow or QWidget).
     * @tparam Callable The callable type: either a member function pointer or a lambda.
     * @param actionName Name of the action as defined in config.json.
     * @param receiver The target QObject (usually MainWindow or a QWidget).
     * @param callable The member function pointer or lambda to connect.
     */
    template <typename T, typename Callable>
    void bindShortcut(const QString &actionName, T *receiver, Callable callable) {
        QKeySequence key = getShortcut(actionName);
        if (key.isEmpty()) {
            qWarning() << QStringLiteral("Shortcut not found for action:") << actionName;
            return;
        }

        auto *action = new QAction(receiver);
        action->setShortcut(key);

        if constexpr (std::is_member_function_pointer_v<Callable>) {
            QObject::connect(action, &QAction::triggered, receiver, callable);
        } else {
            QObject::connect(action, &QAction::triggered, callable);
        }

        if (auto widget = qobject_cast<QWidget *>(receiver)) {
            widget->addAction(action);
        } else if (auto window = qobject_cast<QMainWindow *>(receiver)) {
            window->addAction(action);
        } else {
            qWarning() << QStringLiteral("Warning: Shortcut target is not a QWidget or QMainWindow. Shortcut may not be active.");
        }

        qInfo() << QStringLiteral("Bound shortcut for action:") << actionName << "=>" << key.toString();
    }

private:
    QMap<QString, QKeySequence> shortcuts; ///< Map of action names to shortcuts.
    ConfigManager *configManager;          ///< Pointer to the ConfigManager for loading shortcuts.
};
