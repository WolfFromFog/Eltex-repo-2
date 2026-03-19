#!/bin/bash

FIFO_NAME=/tmp/run/cuckoo.pid

script_name=$(basename "$0")
if [ "$script_name" = "template_task.sh" ]; then
	    echo "я бригадир, сам не работаю"
	        exit 1
fi

log_file="report_${script_name%.*}.log"
echo $log_file
pid=$$

echo "$(date '+%d.%m.%Y %H:%M:%S') [$pid] Скрипт запущен " >> "$log_file"
#sleep $((2 + RANDOM % 10))
#(
#flock 9	
echo "${script_name}[$pid]: how much time do I have?" >> "$FIFO_NAME"
read wait_time < "$FIFO_NAME"
#)9>/tmp/run/lock
sleep $wait_time

echo "$(date '+%d.%m.%Y %H:%M:%S')[$pid] Скрипт завершился, работал $wait_time секунд " >> "$log_file"
