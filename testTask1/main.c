/*
===============================================================================
Имя файла: main.c
Назначение: Главная программа. 
           Секция I – ввод, вывод, сортировка и запись в файл.
           Секция II – чтение ранее записанного файла, дополнение данных, повторная
           сортировка и запись в файл.
Параметры: нет
Автор: Смирнова И.Д.
Дата создания: 01.02.2025
===============================================================================
*/

#include "header.h"

int main(void)
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    Student students[MAX_STUDENTS];
    int count = 0;
    
    printf("=== Секция I: Ввод данных студентов ===\n");
    readStudents(students, &count);
    
    printf("\nВведённые данные:\n");
    printStudents(students, count);
    
    sortStudents(students, count);
    
    printf("\nОтсортированные данные:\n");
    printStudents(students, count);
    
    writeStudentsToFile("students.bin", students, count);
    printf("\nДанные записаны в бинарный файл \"students.bin\".\n");
    
    printf("\n=== Секция II: Чтение и дополнение данных из файла ===\n");
    {
        Student fileStudents[MAX_STUDENTS];
        int fileCount = 0;
        
        fileCount = readStudentsFromFile("students.bin", fileStudents, MAX_STUDENTS);
        
        if (fileCount > 0) {
            printf("\nДанные, прочитанные из файла:\n");
            printStudents(fileStudents, fileCount);
        } else {
            printf("\nФайл пуст или данные не прочитаны.\n");
        }
        
        printf("\nВведите дополнительные записи студентов (пустой Enter для завершения ввода):\n");
        readStudents(fileStudents, &fileCount);

        printf("\nОбновлённый массив студентов:\n");
        printStudents(fileStudents, fileCount);
        
        sortStudents(fileStudents, fileCount);
        
        printf("\nОтсортированный массив студентов:\n");
        printStudents(fileStudents, fileCount);
        
        writeStudentsToFile("students.bin", fileStudents, fileCount);
        printf("\nОбновлённые данные записаны в бинарный файл \"students.bin\".\n");
    }
    
    return 0;
}
