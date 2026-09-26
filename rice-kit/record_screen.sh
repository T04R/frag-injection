#!/bin/bash
if pgrep ffmpeg >/dev/null; then
    pkill ffmpeg & echo 0 > /tmp/ffmpeg-state
else
    mkdir -p $HOME/Videos
    ffmpeg -f x11grab -video_size 1920x1080 -framerate 30 -i :0.0 $HOME/Videos/video_$(date +%Y-%m-%d-%H%M%S).mp4 & echo 1 > /tmp/ffmpeg-state
fi

