#!/bin/sh

DIR=~/applications/dwm/misc
while true; do
    date=$(date +'%d.%m.%Y %T')
    current_song=$($DIR/mediaplayer)

    xsetroot -name "$current_song $date"
	sleep 1
done
