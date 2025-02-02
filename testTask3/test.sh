#!/bin/bash
# Скрипт для компиляции и запуска программы TestTask3.c с отладочным выводом (если требуется).

if [ -f debug.txt ]; then
    rm debug.txt
fi

echo "Компиляция программы TestTask3.c..."
gcc -o TestTask3 TestTask3.c
if [ $? -ne 0 ]; then
    echo "Ошибка компиляции."
    exit 1
fi

# запуск с аргументами: размер буфера 24 байта и включение отладочного вывода ("1").
echo "Запуск программы с аргументами: 24 1"
./TestTask3 24 1

if [ -f debug.txt ]; then
    echo "Содержимое файла debug.txt:"
    cat debug.txt
else
    echo "Отладочный вывод не создан (второй аргумент не равен \"1\")."
fi

if [ -f debug.txt ]; then
    rm debug.txt
fi

#запуск с аргументами: размер буфера 10 байта и включение отладочного вывода ("1").
echo "Запуск программы с аргументами: 10 1"
./TestTask3 10 1

if [ -f debug.txt ]; then
    echo "Содержимое файла debug.txt:"
    cat debug.txt
else
    echo "Отладочный вывод не создан (второй аргумент не равен \"1\")."
fi