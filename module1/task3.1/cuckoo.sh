#!/bin/bash

LOG_FILE=cuckoo.log
PID=$$
FIFO="/tmp/run/cuckoo.pid"


cleanup() {
    rm -f "$FIFO"
    echo "$(date '+%Y-%m-%d %H:%M:%S') Удалён FIFO: $FIFO"
    exit 0
}
 
trap cleanup SIGINT SIGTERM EXIT

mkdir -p /tmp/run
if [[ ! -p "$FIFO" ]]; then
    mkfifo "$FIFO"
    chmod 666 "$FIFO"
fi

log_message() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') $1" >> "$LOG_FILE"
}

exec 3<> "$FIFO"

log_message "Startup!"

while true; do

	if read -u 3 line; then
		echo "DEBUG: получил строку: $line"
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
