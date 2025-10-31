#include "phonebook.h"
#include <stdlib.h>
#include <stdio.h>


int positionCounter = 0;
Person mass[MAX_CONTACTS];

int main()
{

    int choice;
    
    printf("\n=== PHONE BOOK ===\n");
    
    do {
        printf("\n--- MAIN MENU ---\n");
        //printf("1. Show all contacts\n");
        printf("1. Add contact\n");
        printf("2. Edit contact\n");
        printf("3. Delete contact\n");
        printf("0. Exit\n");
        printf("Choose action: ");

        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Adding contact...\n");
                createPerson_ui(positionCounter);
                break;
            case 2:
                printf("Editing contact...\n");
                editPerson_ui(positionCounter);
                break;
            case 3:
                printf("Deleting contact...\n");
                deletePerson_ui(positionCounter);
                break;
            case 0:
                printf("Exiting programm...\n");
                break;
            default:
                printf("Wrong choise! Try again.\n");
        }

    } while (choice != 0);
    system("pause");
    return 0;
}