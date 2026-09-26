#!/bin/bash

WIN_TITLE="note"
NOTE_PATH="~/.config/i3/rice-kit/note.md"

if xdotool search --name "$WIN_TITLE" >/dev/null 2>&1; then
    xdotool search --name "$WIN_TITLE" windowkill
else
    st -t "$WIN_TITLE" -e tmux new-session -A -s note nano "$NOTE_PATH" &
fi

