#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 256

int compare_desc(const void* a, const void* b) {
    int num1 = *(int*)a;
    int num2 = *(int*)b;
    return (num2 - num1);
}

int compare_letters(const void* a, const void* b) {
    return (*(char*)a - *(char*)b);
}

void display_help() {
    printf("Использование программы:\n");
    printf("1. sort_letters <filename> - Упорядочить буквы по алфавиту в файле.\n");
    printf("2. count_letters <filename> - Посчитать количество строчных и прописных букв в файле.\n");
    printf("3. remove_letter_a <filename> - Удалить все буквы 'а' из файла и записать количество удаленных букв.\n");
    printf("4. sort_numbers <filename> - Отсортировать содержимое числового файла по убыванию.\n");
}

void sort_letters(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Ошибка получения размера файла.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("Ошибка создания отображения файла.\n");
        CloseHandle(hFile);
        return;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("Ошибка отображения файла.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    qsort(fileContent, fileSize, sizeof(char), compare_letters);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    printf("Сортировка завершена.\n");
}

void count_letters(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Ошибка получения размера файла.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("Ошибка создания отображения файла.\n");
        CloseHandle(hFile);
        return;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("Ошибка отображения файла.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    int lowercase = 0, uppercase = 0;
    for (DWORD i = 0; i < fileSize; i++) {
        if (fileContent[i] >= 'a' && fileContent[i] <= 'z') {
            lowercase++;
        }
        else if (fileContent[i] >= 'A' && fileContent[i] <= 'Z') {
            uppercase++;
        }
    }

    printf("Строчные буквы: %d\n", lowercase);
    printf("Прописные буквы: %d\n", uppercase);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

void remove_letter_a(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Ошибка получения размера файла.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("Ошибка создания отображения файла.\n");
        CloseHandle(hFile);
        return;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("Ошибка отображения файла.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    int removedCount = 0;
    DWORD j = 0;
    for (DWORD i = 0; i < fileSize; i++) {
        if (fileContent[i] != 'a') {
            fileContent[j++] = fileContent[i];
        }
        else {
            removedCount++;
        }
    }

    SetFilePointer(hFile, j, NULL, FILE_BEGIN);
    SetEndOfFile(hFile);

    printf("Удалено букв 'а': %d\n", removedCount);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

void sort_numbers(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Ошибка открытия файла.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Ошибка получения размера файла.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("Ошибка создания отображения файла.\n");
        CloseHandle(hFile);
        return;
    }

    int* fileContent = (int*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("Ошибка отображения файла.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    DWORD count = fileSize / sizeof(int);
    qsort(fileContent, count, sizeof(int), compare_desc);

    printf("Числа отсортированы по убыванию.\n");

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

void interactive_mode() {
    int choice;
    char filename[BUFFER_SIZE];

    while (1) {
        printf("\nВыберите команду:\n");
        printf("1. Упорядочить буквы по алфавиту\n");
        printf("2. Посчитать количество строчных и прописных букв\n");
        printf("3. Удалить все буквы 'а'\n");
        printf("4. Отсортировать числа по убыванию\n");
        printf("5. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        if (choice == 5) {
            break;
        }

        printf("Введите имя файла: ");
        scanf("%s", filename);

        switch (choice) {
        case 1:
            sort_letters(filename);
            break;
        case 2:
            count_letters(filename);
            break;
        case 3:
            remove_letter_a(filename);
            break;
        case 4:
            sort_numbers(filename);
            break;
        default:
            printf("Неверный выбор!\n");
            break;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        interactive_mode();
    }
    else if (argc == 3) {
        if (strcmp(argv[1], "sort") == 0) {
            sort_letters(argv[2]);
        }
        else if (strcmp(argv[1], "count") == 0) {
            count_letters(argv[2]);
        }
        else if (strcmp(argv[1], "remove_a") == 0) {
            remove_letter_a(argv[2]);
        }
        else if (strcmp(argv[1], "sort_numbers") == 0) {
            sort_numbers(argv[2]);
        }
        else {
            display_help();
        }
    }
    else {
        display_help();
    }

    return 0;
}