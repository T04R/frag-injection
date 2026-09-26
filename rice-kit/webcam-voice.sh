#!/bin/bash
if pgrep ffmpeg >/dev/null; then
    pkill ffmpeg & echo 0 > /tmp/ffmpeg-state
else
    mkdir -p $HOME/Videos
    ffmpeg -f v4l2 -i /dev/video0 -f alsa -i default -c:v libx264 -preset ultrafast -c:a aac $HOME/Videos/webcam_$(date +%Y-%m-%d-%H%M%S).mp4 & echo 1 > /tmp/ffmpeg-state
fi


