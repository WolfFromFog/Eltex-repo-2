#!/bin/bash

script_name=$(basename "$0")
if [ "$script_name" = "template_task.sh" ]; then
	    echo "я бригадир, сам не работаю"
	        exit 1
fi

log_file="reposrt_${script_name%.*}.log"
echo $log_file
pid=$$

echo "[$pid] $(date '+%d.%m.%Y %H:%M:%S') Скрипт запущен " >> "$log_file"
sleep_time=$(( (RANDOM % 1800) + 30))
echo $sleep_time
minutes=$(( $sleep_time / 60 ))
echo $minutes

sleep $sleep_time

echo "[$pid] $(date '+%d.%m.%Y %H:%M:%S') Скрипт завершился, работал $minutes минут " >> "$log_file"
