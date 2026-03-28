#!/usr/bin/env bash

SCRIPT_NAME=$(basename "$0")
N=0
if [[ "$SCRIPT_NAME" == "template_task.sh" ]]; then
    echo "я бригадир, сам не работаю"
    exit 0
fi

LOG_FILE="report_${SCRIPT_NAME}.log"
FIFO="/tmp/run/cuckoo.pid"
LOCK_FILE="/tmp/run/cuckoo.lock"
PID=$$

log_message() {
    echo "$(date '+%Y-%m-%d %H:%M:%S') [$PID] $1" >> "$LOG_FILE"
}

log_message "Скрипт запущен"

if [[ ! -p "$FIFO" ]]; then
    log_message "Ошибка: именованный канал $FIFO не существует"
    exit 1
fi

#(
 #   flock -x 200 || { log_message "Ошибка: не удалось получить блокировку"; exit 1; }

   exec 3<> "$FIFO"  || { log_message "Ошибка открытия FIFO"; exit 1; }
   REQUEST="${SCRIPT_NAME}[${PID}]: how much time do I have?"
   echo "$REQUEST" >&3  || { log_message "Ошибка записи в FIFO"; exit 1; }
    

    # Таймаут 5 секунд на получение ответа
    if read -t 5 -r N <&3; then
        exec 3>&-
    else
        log_message "Ошибка: таймаут при чтении ответа от cuckoo"
        exec 3>&-
        exit 1
    fi
#) 200>"$LOCK_FILE"

if [[ ! "$N" =~ ^[2-9]$|^10$ ]]; then
    log_message "Ошибка: получено неверное значение от cuckoo: $N"
    exit 1
fi

sleep "$N"
log_message "Скрипт завершился, работал $N секунд."
