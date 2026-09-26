
#!/bin/bash

WINDOW_CLASS="st.Ranger"
WINDOW_NAME="quickranger"

if xdotool search --class "$WINDOW_CLASS" >/dev/null 2>&1; then
    window_id=$(xdotool search --class "$WINDOW_CLASS" | head -1)
    current_state=$(xwininfo -id $window_id | grep "Map State:" | awk '{print $3}')

    if [ "$current_state" = "IsViewable" ]; then
        i3-msg "[class=\"$WINDOW_CLASS\"] move scratchpad"
    else
        i3-msg "[class=\"$WINDOW_CLASS\"] scratchpad show"
        i3-msg "[class=\"$WINDOW_CLASS\"] floating enable"
        i3-msg "[class=\"$WINDOW_CLASS\"] move absolute position 1435 24"
        i3-msg "[class=\"$WINDOW_CLASS\"] resize set 25 ppt 60 ppt"
    fi
else
    st -c "$WINDOW_CLASS" -t "$WINDOW_NAME" -e ranger /home/$USER &
    i3-msg "[class=\"$WINDOW_CLASS\"] floating enable"
    i3-msg "[class=\"$WINDOW_CLASS\"] move absolute position 0 24"
    i3-msg "[class=\"$WINDOW_CLASS\"] resize set 30 ppt 40 ppt"
fi

