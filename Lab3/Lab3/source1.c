#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define RENAMING_SUCCESS "File renamed successfully\n"
#define RENAMING_ERROR_MSG "Error while renaming file with code: %lu\n"

void print_help() {
    printf("Info: rename_file <old_file_name> <new_file_name>\n");
    printf("Rename file from <old_file_name> to <new_file_name>\n");
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        char old_file_name[MAX_PATH], new_file_name[MAX_PATH];
        printf("Enter filename: ");
        scanf("%s", old_file_name);
        printf("Enter new filename: ");
        scanf("%s", new_file_name);

        if (MoveFile(old_file_name, new_file_name)) {
            printf(RENAMING_SUCCESS);
        }
        else {
            printf(RENAMING_ERROR_MSG, GetLastError());
            return -1;
        }
    }
    else if (argc == 3) {
        if (MoveFile(argv[1], argv[2])) {
            printf(RENAMING_SUCCESS);
        }
        else {
            printf(RENAMING_ERROR_MSG, GetLastError());
            return -1;
        }
    }
    else {
        print_help();
    }

    return 0;
}