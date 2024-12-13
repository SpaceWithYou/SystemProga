#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_INCREMENT 256
#define MAX_INPUT_SIZE 256
#define ERROR_MSG "ERROR!\n"

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

char* read_input() {
    size_t bufferSize = BUFFER_INCREMENT;
    char* buffer = (char*)malloc(bufferSize);
    if (buffer == NULL) {
        printf(ERROR_MSG);
        return NULL;
    }

    size_t total = 0;
    char tempBuffer[MAX_INPUT_SIZE];

    while (1) {
        if (fgets(tempBuffer, sizeof(tempBuffer), stdin) == NULL) {
            break;
        }

        if (tempBuffer[0] == '\n') {
            break;
        }

        size_t tempLength = strlen(tempBuffer);
        if (total + tempLength >= bufferSize) {
            
            bufferSize += BUFFER_INCREMENT;
            char* newBuffer = (char*)realloc(buffer, bufferSize);
            if (newBuffer == NULL) {
                printf(ERROR_MSG);
                free(buffer);
                return NULL;
            }
            buffer = newBuffer;
        }

        strcpy(buffer + total, tempBuffer);
        total += tempLength;
    }

    buffer[total] = '\0';

    return buffer;
}

void print_help() {
    printf("INFO:\n");
    printf("create <file>\n");
    printf("read <file>\n");
    printf("rename <oldfile> <newfile>\n");
    printf("delete <file>\n");
    printf("copy <src> <dst>\n");
    printf("size <filename>\n");
    printf("attrib <filename>\n");
    printf("readonly <filename>\n");
    printf("hidden <filename>\n");
    printf("'dir' <path>\n");
    printf("'help'\n");
    printf("'exit'\n");
}

void create_file(const char* filename) {
    HANDLE hFile;
    DWORD bytesWritten;

    char* buffer = read_input();
    if (buffer == NULL) {
        return;
    }

    hFile = CreateFile(
        filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE) {
        printf("ERROR: %d\n", GetLastError());
        free(buffer);
        return;
    }

    if (!WriteFile(hFile, buffer, (DWORD)strlen(buffer), &bytesWritten, NULL)) {
        printf("Ошибка записи в файл: %d\n", GetLastError());
    }
    else {
        printf("Записано %lu байт в файл %s\n", bytesWritten, filename);
    }

    CloseHandle(hFile);
    free(buffer);
}



void read_file(const char* filename) {
    HANDLE hFile = CreateFileA(
        filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        printf(ERROR_MSG);
        return;
    }

    char buffer[10];
    DWORD bytesRead;
    printf("File content:\n");
    while (TRUE) {
        if (ReadFile(hFile, buffer, 9, &bytesRead, NULL) == FALSE) {
            printf("ERROR: %d\n", GetLastError());
            CloseHandle(hFile);
            return;
        }

        if (bytesRead == 0) {
            break;
        }

        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    CloseHandle(hFile);
}

void delete_file(const char* filename) {
    if (DeleteFile(filename)) {
        printf("Файл удален\n");
    }
    else {
        printf("Ошибка удаления файла\n");
    }
}

void rename_file(const char* oldname, const char* newname) {
    if (MoveFile(oldname, newname)) {
        printf("Файл переименован\n");
    }
    else {
        printf("Ошибка переименования файла\n");
    }
}

void copy_file(const char* src, const char* dst) {
    if (CopyFile(src, dst, FALSE)) {
        printf("Файл скопирован\n");
    }
    else {
        printf("Ошибка копирования файла\n");
    }
}

void file_size(const char* filename) {
    HANDLE hFile = CreateFile(
        filename,
        GENERIC_READ,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    
    if (hFile == INVALID_HANDLE_VALUE) {
        printf(ERROR_MSG);
        return;
    }

    DWORD size = GetFileSize(hFile, NULL);
    printf("File size: %d байт\n", size);
    CloseHandle(hFile);
}

void file_attrib(const char* filename) {
    DWORD attributes = GetFileAttributes(filename);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Ошибка получения атрибутов файла\n");
        return;
    }

    printf("Атрибуты файла: ");
    if (attributes & FILE_ATTRIBUTE_READONLY) printf("Только для чтения ");
    if (attributes & FILE_ATTRIBUTE_HIDDEN) printf("Скрытый ");
    if (attributes & FILE_ATTRIBUTE_SYSTEM) printf("Системный ");
    if (attributes & FILE_ATTRIBUTE_ARCHIVE) printf("Архив ");
    printf("\n");
}

void set_readonly(const char* filename) {
    DWORD attributes = GetFileAttributes(filename);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Ошибка получения атрибутов файла\n");
        return;
    }

    attributes |= FILE_ATTRIBUTE_READONLY;
    if (SetFileAttributes(filename, attributes)) {
        printf("Атрибут 'только для чтения' установлен\n");
    }
    else {
        printf("Ошибка установки атрибута\n");
    }
}

void set_hidden(const char* filename) {
    DWORD attributes = GetFileAttributes(filename);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        printf("Ошибка получения атрибутов файла\n");
        return;
    }

    attributes |= FILE_ATTRIBUTE_HIDDEN;
    if (SetFileAttributes(filename, attributes)) {
        printf("Атрибут 'скрытый' установлен\n");
    }
    else {
        printf("Ошибка установки атрибута\n");
    }
}

void list_directory(const char* path) {
    WIN32_FIND_DATA findData;

    char pathWithWildcard[MAX_PATH];
    strcpy(pathWithWildcard, path);
    strcat(pathWithWildcard, "\\*");

    HANDLE hFind = FindFirstFile(pathWithWildcard, &findData);

    if (hFind == INVALID_HANDLE_VALUE) {
        printf("Ошибка открытия директории\n");
        return;
    }

    do {
        printf("%s\n", findData.cFileName);
    } while (FindNextFile(hFind, &findData) != 0);

    FindClose(hFind);
}


void process_command(char* command) {
    char cmd[256], arg1[256], arg2[256];
    int args = sscanf(command, "%s %s", cmd, arg1);
    if (strcmp(cmd, "create") == 0) {
        create_file(arg1);
    }
    else if (strcmp(cmd, "read") == 0) {
        read_file(arg1);
    }
    else if (strcmp(cmd, "delete") == 0) {
        delete_file(arg1);
    }
    else if (strcmp(cmd, "rename") == 0) {
        args = sscanf(command, "%s %s %s", cmd, arg1, arg2);
        rename_file(arg1, arg2);
    }
    else if (strcmp(cmd, "copy") == 0) {
        args = sscanf(command, "%s %s %s", cmd, arg1, arg2);
        copy_file(arg1, arg2);
    }
    else if (strcmp(cmd, "size") == 0) {
        file_size(arg1);
    }
    else if (strcmp(cmd, "attrib") == 0) {
        file_attrib(arg1);
    }
    else if (strcmp(cmd, "readonly") == 0) {
        set_readonly(arg1);
    }
    else if (strcmp(cmd, "hidden") == 0) {
        set_hidden(arg1);
    }
    else if (strcmp(cmd, "dir") == 0) {
        list_directory(arg1);
    }
    else if (strcmp(cmd, "help") == 0) {
        print_help();
    }
    else if (strcmp(cmd, "exit") == 0) {
        exit(0);
    }
    else {
        printf("Неизвестная команда. Введите 'help' для получения списка команд.\n");
    }
}

int main(int argc, char* argv[]) {
    SetConsoleOutputCP(CP_UTF8);
    if (argc == 1) {
        char command[256];
        print_help();
        while (1) {
            printf("\nВведите команду: ");
            fgets(command, sizeof(command), stdin);
            command[strcspn(command, "\n")] = 0;
            process_command(command);
        }
    }
    else {
        char command[256] = "";
        for (int i = 1; i < argc; ++i) {
            strcat(command, argv[i]);
            strcat(command, " ");
        }
        process_command(command);
    }

    return 0;
}