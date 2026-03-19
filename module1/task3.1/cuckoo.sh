#!/bin/bash

LOG_FILE=cuckoo.log
PID=$$
FIFO_NAME=cuckoo.pid

mkfifo /tmp/run/$FIFO_NAME 

trap ' echo "$(date "+%d.%m.%Y %H:%M:%S") Shutdown!">> "$LOG_FILE"; exit ' SIGINT

exec 3<> "/tmp/run/$FIFO_NAME"


echo "$(date '+%d.%m.%Y %H:%M:%S') Startup!">> "$LOG_FILE"

while true; do

	if read -u 3 line; then
		if [[ "$line" =~ ^([^[]+)\[([0-9]+)\]:\ how\ much\ time\ do\ I\ have\?$ ]]; then
			client_name="${BASH_REMATCH[1]}"
			client_pid="${BASH_REMATCH[2]}"
			NUMBER=$((2 + RANDOM % (10-2+1)))
			echo "$NUMBER" >&3
			echo "${client_name}[${client_pid}]: how much time do I have?" >> "$LOG_FILE"
			echo "$(date '+%d.%m.%Y %H:%M:%S') ${client_name}[${client_pid}] $NUMBER" >> "$LOG_FILE"
		fi
	fi

done

