#!/bin/bash
path=/sys/class/backlight/amdgpu_bl1/brightness

if [[ ! -f $path ]]; then
  echo 0 > $path
fi

current=$(cat $path)

if ! [[ "$current" =~ ^[0-9]+$ ]]; then
  exit 1
fi

new=$((current - 1000))
echo "$new" > $path


