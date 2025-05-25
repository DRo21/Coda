-- example.lua: Lua scripting template for Coda plugins
-- This file shows available functions, events, and usage comments only

-- Event Hook: Called when a file is opened
function onFileOpen(path)
--     path: string - The absolute path of the opened file
--     Use this to modify editor text, show messages, or react to file events
end

-- Event Hook: Called when a file is saved
function onFileSave(path)
--     path: string - The absolute path of the saved file
--     Use this to inject headers, validate content, or trigger actions
end

-- Global Table: Coda
Coda.showMessage(message)
--     Prints a message to the console (for debug or user feedback)
--     message: string - The message to display

-- Global Table: editor
editor.getText()
--     Returns the current editor text as a string

editor.setText(newText)
--     Replaces the current editor text with newText
--     newText: string - The text to set in the editor

editor.getCursorPosition()
--     Returns the current cursor position as (line, column)

editor.setCursorPosition(line, column)
--     Moves the cursor to the specified line and column (1-based)

editor.getSelection()
--     Returns the currently selected text as a string

editor.replaceSelection(newText)
--     Replaces the current selection with newText

editor.insertTextAt(line, column, newText)
--     Inserts newText at the specified line and column (1-based)
