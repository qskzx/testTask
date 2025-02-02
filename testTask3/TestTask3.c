/*
    Имя программы:  TestTask3.c
    Назначение:     1) Создаёт динамический буфер на 32К и заполняет его 
                       прямым и обратным "адресным кодом", пишет в 2 файла.
                    2) Считывает из этих файлов в два буфера, обрабатывает 
                       (перестановка пар, циклический сдвиг) и записывает 
                       в ещё 2 файла.
    Параметры:      Необязательно
                    Если запустить программу как:
                      ./TestTask3 24 1
                    то:
                      - первый аргумент задаёт размер буфера (в байтах),
                      - второй аргумент (значение "1") включает отладочный вывод,
                        который записывается в текстовый файл debug.txt.
    Автор:          Смирнова И.Д.
    Дата создания:  01.02.2025
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

static const size_t DEFAULT_BUFFER_SIZE = 32768;
static const unsigned short SHIFT_N = 5;

static const char *FILE1_NAME = "file1.bin";
static const char *FILE2_NAME = "file2.bin";
static const char *FILE3_NAME = "file3.bin";
static const char *FILE4_NAME = "file4.bin";
static const char *DEBUG_FILE_NAME = "debug.txt";

static FILE *debug_file = NULL;  

static int WriteShortsToFile(const char *filename,
                             const unsigned short *pData,
                             size_t nShorts)
{
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s для записи!\n", filename);
        return 1;
    }
    size_t written = fwrite(pData, sizeof(unsigned short), nShorts, fp);
    if (written != nShorts)
    {
        fprintf(stderr, "Ошибка: при записи в '%s' ожидалось %zu слов, записано %zu.\n",
                filename, nShorts, written);
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

static int ReadShortsFromFile(const char *filename,
                              unsigned short *pData,
                              size_t nShorts)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        fprintf(stderr, "Ошибка: не удалось открыть файл %s для чтения!\n", filename);
        return 1;
    }
    size_t readCount = fread(pData, sizeof(unsigned short), nShorts, fp);
    if (readCount != nShorts)
    {
        fprintf(stderr, "Ошибка: при чтении из '%s' ожидалось %zu слов, прочитано %zu.\n",
                filename, nShorts, readCount);
        fclose(fp);
        return 1;
    }
    fclose(fp);
    return 0;
}

static void SwapNeighborWords(unsigned short *pData, size_t nShorts)
{
    for (size_t i = 0; i + 1 < nShorts; i += 2)
    {
        unsigned short temp = pData[i];
        pData[i] = pData[i + 1];
        pData[i + 1] = temp;
    }
}

static void PrintArray(unsigned short *pData, size_t nShorts)
{
    if (debug_file)
    {
        for (size_t i = 0; i < nShorts; ++i)
            fprintf(debug_file, "%o ", pData[i]);
        fprintf(debug_file, "\n");
    }
}

static void RotateArrayLeft(unsigned short *pData, size_t nShorts, size_t shift)
{
    unsigned short *temp = (unsigned short *)malloc(nShorts * sizeof(unsigned short));
    if (!temp)
    {
        fprintf(stderr, "Ошибка: не удалось выделить память под временный буфер для RotateArray.\n");
        return;
    }
    for (size_t i = 0; i < nShorts; i++)
    {
        temp[i] = pData[i];
    }
    for (size_t i = 0; i < nShorts; i++)
    {
        size_t newIndex = (i + shift) % nShorts;
        pData[i] = temp[newIndex];
    }
    free(temp);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    size_t BUFFER_SIZE = DEFAULT_BUFFER_SIZE;
    if (argc > 1)
    {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && val > 0)
        {
            BUFFER_SIZE = (size_t)val;
        }
        else
        {
            fprintf(stderr, "Неверный аргумент, используется значение по умолчанию %zu байт.\n", DEFAULT_BUFFER_SIZE);
        }
    }
    
    if (argc > 2 && strcmp(argv[2], "1") == 0)
    {
        debug_file = fopen(DEBUG_FILE_NAME, "w");
        if (!debug_file)
        {
            fprintf(stderr, "Ошибка: не удалось открыть файл отладки %s\n", DEBUG_FILE_NAME);
            return 1;
        }
        fprintf(debug_file, "Отладочный вывод:\n");
    }

    if (debug_file) fprintf(debug_file, "Размер буфера: %zu байт\n", BUFFER_SIZE);
    
    unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);
    if (!buffer)
    {
        fprintf(stderr, "Ошибка: не удалось выделить память.\n");
        return 1;
    }
    size_t nShorts = BUFFER_SIZE / sizeof(unsigned short);

    {
        unsigned short *pData = (unsigned short *)buffer;
        for (size_t i = 0; i < nShorts; i++)
        {
            pData[i] = (unsigned short)i;
        }
    }
    if (WriteShortsToFile(FILE1_NAME, (unsigned short *)buffer, nShorts) != 0)
    {
        free(buffer);
        return 1;
    }

    {
        unsigned short *pData = (unsigned short *)buffer;
        unsigned short Imax = (unsigned short)(nShorts - 1);
        for (size_t i = 0; i < nShorts; i++)
        {
            pData[i] = (unsigned short)(Imax - i);
        }
    }
    if (WriteShortsToFile(FILE2_NAME, (unsigned short *)buffer, nShorts) != 0)
    {
        free(buffer);
        return 1;
    }
    free(buffer);

    unsigned char *buffer1 = (unsigned char *)malloc(BUFFER_SIZE);
    unsigned char *buffer2 = (unsigned char *)malloc(BUFFER_SIZE);
    if (!buffer1 || !buffer2)
    {
        fprintf(stderr, "Ошибка: не удалось выделить память для buffer1/buffer2.\n");
        free(buffer1);
        free(buffer2);
        return 1;
    }
    if (ReadShortsFromFile(FILE1_NAME, (unsigned short *)buffer1, nShorts) != 0)
    {
        free(buffer1);
        free(buffer2);
        return 1;
    }
    if (ReadShortsFromFile(FILE2_NAME, (unsigned short *)buffer2, nShorts) != 0)
    {
        free(buffer1);
        free(buffer2);
        return 1;
    }

    if (debug_file) fprintf(debug_file, "Исходные данные из file1.bin:\n");
    PrintArray((unsigned short *)buffer1, nShorts);
    if (debug_file) fprintf(debug_file, "Исходные данные из file2.bin:\n");
    PrintArray((unsigned short *)buffer2, nShorts);

    SwapNeighborWords((unsigned short *)buffer1, nShorts);
    SwapNeighborWords((unsigned short *)buffer2, nShorts);
    if (debug_file) fprintf(debug_file, "После обмена соседних слов в file1.bin:\n");
    PrintArray((unsigned short *)buffer1, nShorts);
    if (debug_file) fprintf(debug_file, "После обмена соседних слов в file2.bin:\n");
    PrintArray((unsigned short *)buffer2, nShorts);

    RotateArrayLeft((unsigned short *)buffer1, nShorts, SHIFT_N);
    RotateArrayLeft((unsigned short *)buffer2, nShorts, SHIFT_N);
    if (debug_file) fprintf(debug_file, "После циклического сдвига в file1.bin:\n");
    PrintArray((unsigned short *)buffer1, nShorts);
    if (debug_file) fprintf(debug_file, "После циклического сдвига в file2.bin:\n");
    PrintArray((unsigned short *)buffer2, nShorts);

    if (WriteShortsToFile(FILE3_NAME, (unsigned short *)buffer1, nShorts) != 0)
    {
        free(buffer1);
        free(buffer2);
        return 1;
    }
    if (WriteShortsToFile(FILE4_NAME, (unsigned short *)buffer2, nShorts) != 0)
    {
        free(buffer1);
        free(buffer2);
        return 1;
    }
    free(buffer1);
    free(buffer2);

    if (debug_file)
        fclose(debug_file);

    return 0;
}
