#!/bin/bash
if pgrep ffmpeg >/dev/null; then
    pkill ffmpeg & echo 0 > /tmp/ffmpeg-state
else
    mkdir -p $HOME/Videos
    ffmpeg -f x11grab -s 1920x1080 -i :0.0 -f alsa -i default -c:v libx264 -c:a aac $HOME/Videos/video_$(date +%Y-%m-%d-%H%M%S).mp4 & echo 1 > /tmp/ffmpeg-state
fi

