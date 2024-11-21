#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void print_help() {
    printf("Info: remove_a <filename>\n");
    printf("Remove all occurrences of 'a' from the file and append the count of removed 'a' in end\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        print_help();
        return 0;
    }

    HANDLE hFile = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("Error while opening file: %lu\n", GetLastError());
        return -1;
    }

    DWORD fileSize = GetFileSize(hFile, NULL);
    if (fileSize == INVALID_FILE_SIZE) {
        printf("Error while getting file size: %lu\n", GetLastError());
        CloseHandle(hFile);
        return -1;
    }

    HANDLE hMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, fileSize, NULL);
    if (hMapping == NULL) {
        printf("Error while creating file mapping: %lu\n", GetLastError());
        CloseHandle(hFile);
        return -1;
    }

    char* fileContent = (char*)MapViewOfFile(hMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, fileSize);
    if (fileContent == NULL) {
        printf("Error while mapping file view: %lu\n", GetLastError());
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return -1;
    }

    int counter = 0;
    for (DWORD i = 0; i < fileSize; i++) {
        if (fileContent[i] == 'a') {
            fileContent[i] = ' ';
            counter++;
        }
    }

    char countBuffer[32];
    sprintf(countBuffer, "\nNumber of 'a' removed: %d", counter);
    DWORD countSize = strlen(countBuffer);
    SetFilePointer(hFile, 0, NULL, FILE_END);
    DWORD written;
    WriteFile(hFile, countBuffer, countSize, &written, NULL);

    UnmapViewOfFile(fileContent);
    CloseHandle(hMapping);
    CloseHandle(hFile);

    printf("Successfully removed %d occurrences of 'a'\n", counter);
    return 0;
}
