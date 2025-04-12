#include <stdio.h>
#include "mapping.h"

void displayMenu() {
    printf("1. Open mapping\n");
    printf("2. Add row\n");
    printf("3. Remove row\n");
    printf("4. Print row\n");
    printf("5. Print rows\n");
    printf("6. Close mapping\n");
}

int main() {
    int choice;
    int listSize = 0;

    while (1) {
        displayMenu();
        printf("Choose option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char filePath[256];
                printf("Enter path to file: ");
                scanf("%s", filePath);

                if (OpenMapping(filePath, &listSize) != 0) {
                    fprintf(stderr, "Error opening file\n");
                } else {
                    printf("File opened.\n");
                }
                break;
            }
            case 2: {
                struct Student newStudent;
                printf("Enter name: ");
                scanf("%s", newStudent.Name);
                printf("Enter surname: ");
                scanf("%s", newStudent.Surname);
                printf("Enter year: ");
                scanf("%hhu", &newStudent.Course);
                printf("Enter group: ");
                scanf("%hhu", &newStudent.Group);
                printf("Enter ID: ");
                scanf("%s", newStudent.ID);

                int position;
                printf("Enter index of the row to add (negative values for add from end): ");
                scanf("%d", &position);

                if (AddRow(newStudent, position) != 0) {
                    fprintf(stderr, "Error while adding.\n");
                }
                break;
            }
            case 3: {
                int pos;
                printf("Enter index of the row to delete: ");
                scanf("%d", &pos);
                if (RemRow(pos) != 0) {
                    fprintf(stderr, "Error while removing.\n");
                }
                break;
            }
            case 4: {
                int pos;
                printf("Enter index of the row to print: ");
                scanf("%d", &pos);
                if (PrintRow(pos) != 0) {
                    fprintf(stderr, "Error while printing one row.\n");
                }
                break;
            }
            case 5: {
                if (PrintRows() != 0) {
                    fprintf(stderr, "Error while printing all rows.\n");
                }
                break;
            }
            case 6: {
                CloseMapping(hFileMapping);
                break;
            }
            default: {
                printf("Incorrect input. Try again.\n");
                break;
            }
        }
    }

    return 0;
}
