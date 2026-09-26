#!/bin/bash
if pgrep ffmpeg >/dev/null; then
    pkill ffmpeg & echo 0 > /tmp/ffmpeg-state
else
    mkdir -p $HOME/Voices
    ffmpeg -f alsa -i default -t 30 $HOME/Voices/voice_$(date +%Y-%m-%d-%H%M%S).mp3 & echo 1 > /tmp/ffmpeg-state
fi


