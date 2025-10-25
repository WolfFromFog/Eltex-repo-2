#!/bin/bash

#Почему-то не работает без этого. Вроде из-за того, что cron запускает в другой директории или че-то типо того
cd "/root/Eltex-repo-2/module1/task3" || exit 1

#Переменные с файлами
CONFIG_FILE="observer.conf"
LOG_FILE="observer.log"
TIMESTAMP=$(date '+%Y-%m-%d %H:%M:%S')

#Функция проверки процесса
check_process() {
    local script_name="$1"
    local found=0
    
    
    for pid_dir in /proc/[0-9]*; do
        if [ -d "$pid_dir" ]; then
            if [ -r "$pid_dir/cmdline" ]; then
                cmdline=$(tr -d '\0' < "$pid_dir/cmdline")
                if [[ "$cmdline" == *"$script_name"* ]]; then
                    found=1
                    break
                fi
            fi
        fi
    done
    
    echo $found
}

#Запуск скриптов

start_script() {
    local script_name="$1"
    nohup "./$script_name" > /dev/null 2>&1 &
    echo "[$TIMESTAMP] Запущен скрипт: $script_name (PID: $!)" >> "$LOG_FILE"
}

#Основная логика
while IFS= read -r script_name; do

    [[ -z "$script_name" || "$script_name" =~ ^# ]] && continue
    
    # Проверка процесса
    if [ $(check_process "$script_name") -eq 0 ]; then
        echo "[$TIMESTAMP] Скрипт $script_name не найден в процессах, запускаем..." >> "$LOG_FILE"
        start_script "$script_name"
    else
        echo "[$TIMESTAMP] Скрипт $script_name работает нормально" >> "$LOG_FILE"
    fi
done < "$CONFIG_FILE"
