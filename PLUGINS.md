
# Coda Lua Plugin Guide

Welcome to **Coda's Lua Plugin System**!  
This guide shows how to write plugins using Lua to extend Coda's functionality.

---

## Events

Plugins can define **event handler functions**.  
These functions are called automatically by Coda when certain actions occur:

| Event Function    | Description                                    |
|-------------------|------------------------------------------------|
| `onFileOpen(path)` | Called when a file is opened in the editor     |
| `onFileSave(path)` | Called when a file is saved in the editor      |

- `path` is the absolute path of the file as a string.

Example:

```lua
function onFileOpen(path)
    Coda.showMessage("File opened: " .. path)
end
```

---

## Coda API

### `Coda.showMessage(message)`

- Prints a message to the console (useful for debug/logging).
- `message` (string): The message to display.

---

## Editor API

The `editor` object exposes functions to interact with the editor content:

| Function                                   | Description                                        |
|-------------------------------------------|----------------------------------------------------|
| `editor.getText()`                         | Returns the full editor text as a string           |
| `editor.setText(newText)`                  | Replaces the entire editor text                    |
| `editor.getCursorPosition()`               | Returns `(line, column)` (1-based) of the cursor   |
| `editor.setCursorPosition(line, column)`   | Moves the cursor to the given position             |
| `editor.getSelection()`                    | Returns the currently selected text                |
| `editor.replaceSelection(newText)`         | Replaces the current selection with new text       |
| `editor.insertTextAt(line, column, newText)` | Inserts text at specified position               |

---

## Plugin Example

```lua
function onFileSave(path)
    local text = editor.getText()
    if not text:find("/%*%*") then
        local header = "/**\n * @file " .. path .. "\n * @brief TODO\n */\n\n"
        editor.setText(header .. text)
        Coda.showMessage("Doxygen header added to: " .. path)
    end
end
```

---

## How to Use Plugins

1. Place your `.lua` scripts in the `plugins/` folder.
2. Define them in `plugins.json` like this:

```json
{
  "plugins": [
    { "file": "plugins/auto_doxygen.lua", "enabled": true },
    { "file": "plugins/example.lua", "enabled": true }
  ]
}
```

3. Run Coda—your plugins will be loaded automatically!

---

That’s it! Have fun building amazing plugins for Coda!
