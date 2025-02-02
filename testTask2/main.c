/*
 * Программа: Task2.c
 * Назначение: Принимает размер буфера как аргумент командной строки (если не задан, используется значение по умолчанию),
 *            выделяет динамический буфер, заполняет его короткими словами с полосатым кодом, записывает буфер в бинарный файл,
 *            а затем считывает и выводит содержимое файла.
 * Параметры: argv[1] (необязательно) - размер буфера в байтах (положительное число).
 * Автор: Смирнова И.Д.
 * Дата создания: 01.02.2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "ru_RU.UTF-8");

    const size_t DefaultBufferSize = 32768;      
    const unsigned short int StripeCode = 025252; 

    size_t BufferSize = DefaultBufferSize;

    if (argc > 1)
    {
        char *endptr;
        long val = strtol(argv[1], &endptr, 10);
        if (endptr != argv[1] && val > 0)
        {
            BufferSize = (size_t) val;
        }
        else
        {
            fprintf(stderr, "Неверный аргумент, используется значение по умолчанию %zu байт.\n", DefaultBufferSize);
        }
    }

    //printf("Размер буфера: %zu байт\n", BufferSize);

    unsigned char *pBuffer = (unsigned char *) calloc(BufferSize, sizeof(unsigned char));
    if (pBuffer == NULL)
    {
        fprintf(stderr, "Ошибка: не удалось выделить память.\n");
        return 1;
    }

    size_t NumShorts = BufferSize / sizeof(unsigned short int);
    unsigned short int *pShort = (unsigned short int *) pBuffer;
    for (size_t i = 0; i < NumShorts; i++)
    {
        pShort[i] = StripeCode;
    }

    FILE *pFile = fopen("stripe.bin", "wb");
    if (pFile == NULL)
    {
        fprintf(stderr, "Ошибка: не удалось открыть файл для записи.\n");
        free(pBuffer);
        return 1;
    }

    if (fwrite(pBuffer, sizeof(unsigned char), BufferSize, pFile) != BufferSize)
    {
        fprintf(stderr, "Ошибка: сбой при записи в файл.\n");
        fclose(pFile);
        free(pBuffer);
        return 1;
    }
    fclose(pFile);
    free(pBuffer);

    if (argc > 1)
    {
        pFile = fopen("stripe.bin", "rb");
        if (pFile == NULL)
        {
            fprintf(stderr, "Ошибка: не удалось открыть файл для чтения.\n");
            return 1;
        }

        unsigned char *pReadBuffer = (unsigned char *) malloc(BufferSize * sizeof(unsigned char));
        if (pReadBuffer == NULL)
        {
            fprintf(stderr, "Ошибка: не удалось выделить память для чтения файла.\n");
            fclose(pFile);
            return 1;
        }

        size_t bytesRead = fread(pReadBuffer, sizeof(unsigned char), BufferSize, pFile);
        if (bytesRead != BufferSize)
        {
            fprintf(stderr, "Предупреждение: считано меньше байт, чем ожидалось (%zu из %zu).\n", bytesRead, BufferSize);
        }
        fclose(pFile);

        printf("\nСодержимое файла \"stripe.bin\":\n");
        size_t readNumShorts = bytesRead / sizeof(unsigned short int);
        unsigned short int *pReadShort = (unsigned short int *) pReadBuffer;
        for (size_t i = 0; i < readNumShorts; i++)
        {
            printf("%06o ", pReadShort[i]);
            if ((i + 1) % 10 == 0)
            {
                printf("\n");
            }
        }
        if (readNumShorts % 10 != 0)
        {
            printf("\n");
        }

        free(pReadBuffer);
    }
    return 0;
}
