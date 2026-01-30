#include "phonebook.h"
#include <stdlib.h>
#include <stdio.h>


int positionCounter = 0;
Person mass[MAX_CONTACTS];

int main()
{

    int choice;
    int result = 0;
    
    printf("\n=== PHONE BOOK ===\n");
    
    do {
        printf("\n--- MAIN MENU ---\n");
        printf("1. Show all contacts\n");
        printf("2. Add contact\n");
        printf("3. Edit contact\n");
        printf("4. Delete contact\n");
        printf("0. Exit\n");
        printf("Choose action: ");


        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("\nShowing contacts...\n");
                showAllPersons_ui();
                break;
            case 2:
                printf("\nAdding contact...\n");
                result = createPerson_ui();
                break;
            case 3:
                printf("\nEditing contact...\n");
                result = editPerson_ui();
                break;
            case 4:
                printf("\nDeleting contact...\n");
                result = deletePerson_ui();
                break;
            case 0:
                printf("\nExiting programm...\n");
                break;
            default:
                printf("\nWrong choise! Try again.\n");
        }
    } while (choice != 0);


    freeAll();

    return 0;
}