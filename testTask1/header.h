/*
===============================================================================
Имя файла: header.h
Назначение: Объявление констант, структуры Student и прототипов функций для работы
           с данными студентов (фамилия и стипендия).
Параметры: нет
Автор: Смирнова И.Д.
Дата создания: 01.02.2025
===============================================================================
*/

#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

static const int MAX_STUDENTS = 64;
static const int MAX_NAME_LENGTH = 16;

typedef struct {
    char fio[MAX_NAME_LENGTH];  
    int stipa;                 
} Student;

void readStudents(Student students[], int *count);
void printStudents(const Student students[], int count);
void sortStudents(Student students[], int count);
void writeStudentsToFile(const char *filename, const Student students[], int count);
int readStudentsFromFile(const char *filename, Student students[], int maxCount);

#endif
