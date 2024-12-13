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
    printf("������������� ���������:\n");
    printf("1. sort_letters <filename> - ����������� ����� �� �������� � �����.\n");
    printf("2. count_letters <filename> - ��������� ���������� �������� � ��������� ���� � �����.\n");
    printf("3. remove_letter_a <filename> - ������� ��� ����� '�' �� ����� � �������� ���������� ��������� ����.\n");
    printf("4. sort_numbers <filename> - ������������� ���������� ��������� ����� �� ��������.\n");
}

void sort_letters(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("������ �������� �����.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("������ ��������� ������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("������ �������� ����������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("������ ����������� �����.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    qsort(fileContent, fileSize, sizeof(char), compare_letters);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    printf("���������� ���������.\n");
}

void count_letters(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("������ �������� �����.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("������ ��������� ������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("������ �������� ����������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("������ ����������� �����.\n");
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

    printf("�������� �����: %d\n", lowercase);
    printf("��������� �����: %d\n", uppercase);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

void remove_letter_a(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("������ �������� �����.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("������ ��������� ������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("������ �������� ����������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("������ ����������� �����.\n");
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

    printf("������� ���� '�': %d\n", removedCount);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

void sort_numbers(const char* filename) {
    HANDLE hFile = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("������ �������� �����.\n");
        return;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("������ ��������� ������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("������ �������� ����������� �����.\n");
        CloseHandle(hFile);
        return;
    }

    int* fileContent = (int*)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("������ ����������� �����.\n");
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return;
    }

    DWORD count = fileSize / sizeof(int);
    qsort(fileContent, count, sizeof(int), compare_desc);

    printf("����� ������������� �� ��������.\n");

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);
}

void interactive_mode() {
    int choice;
    char filename[BUFFER_SIZE];

    while (1) {
        printf("\n�������� �������:\n");
        printf("1. ����������� ����� �� ��������\n");
        printf("2. ��������� ���������� �������� � ��������� ����\n");
        printf("3. ������� ��� ����� '�'\n");
        printf("4. ������������� ����� �� ��������\n");
        printf("5. �����\n");
        printf("��� �����: ");
        scanf("%d", &choice);

        if (choice == 5) {
            break;
        }

        printf("������� ��� �����: ");
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
            printf("�������� �����!\n");
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