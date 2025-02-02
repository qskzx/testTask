/*
===============================================================================
Имя файла: functions.c
Назначение: Реализация функций для ввода, вывода, сортировки данных студентов и
           работы с бинарным файлом.
Параметры: см. прототипы в header.h
Автор: Смирнова И.Д.
Дата создания: 01.02.2025
===============================================================================
*/

#include "header.h"

static int caseInsensitiveCompare(const char *s1, const char *s2)
{
    while (*s1 && *s2) {
        char c1 = tolower((unsigned char)*s1);
        char c2 = tolower((unsigned char)*s2);
        if (c1 != c2)
            return c1 - c2;
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}

void readStudents(Student students[], int *count)
{
    char inputBuffer[256];
    
    while (*count < MAX_STUDENTS) {
        printf("Введите фамилию студента (пустой Enter - завершить ввод): ");
        if (gets(inputBuffer) == NULL) {
            break;
        }

        if (inputBuffer[0] == '\0')
            break;
        
        strncpy(students[*count].fio, inputBuffer, MAX_NAME_LENGTH - 1);
        students[*count].fio[MAX_NAME_LENGTH - 1] = '\0';
        
        printf("Введите стипендию: ");
        if (gets(inputBuffer) == NULL) {
            break;
        }
        students[*count].stipa = atoi(inputBuffer);
        
        (*count)++;
    }
}

void printStudents(const Student students[], int count)
{
    int i;
    for (i = 0; i < count; i++) {
        printf("Студент: %s, Стипендия: %d\n", students[i].fio, students[i].stipa);
    }
}

void sortStudents(Student students[], int count)
{
    int i, j;
    Student temp;
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (caseInsensitiveCompare(students[i].fio, students[j].fio) > 0) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
}

void writeStudentsToFile(const char *filename, const Student students[], int count)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("Ошибка открытия файла для записи.\n");
        return;
    }
    if (fwrite(students, sizeof(Student), count, fp) != (size_t)count) {
        printf("Ошибка записи в файл.\n");
    }
    fclose(fp);
}

int readStudentsFromFile(const char *filename, Student students[], int maxCount)
{
    FILE *fp = fopen(filename, "rb");
    int count = 0;
    if (fp == NULL) {
        printf("Ошибка открытия файла для чтения.\n");
        return 0;
    }
    count = (int)fread(students, sizeof(Student), maxCount, fp);
    if (count == 0) {
        printf("Нет данных для чтения из файла.\n");
    }
    fclose(fp);
    return count;
}
