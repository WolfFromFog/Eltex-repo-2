#include "phonebook.h"
#include <stdlib.h>
#include <stdio.h>


int positionCounter = 0;
//phonebook *root = NULL;

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
                showAllPersons_ui(root);
                break;
            case 2:
                printf("\nAdding contact...\n");
                result = createPerson_ui(&root);
                break;
            case 3:
                printf("\nEditing contact...\n");
                result = editPerson_ui(root);
                break;
            case 4:
                printf("\nDeleting contact...\n");
                result = deletePerson_ui(&root);
                break;
            case 0:
                printf("\nExiting programm...\n");
                break;
            default:
                printf("\nWrong choise! Try again.\n");
        }
        switch (result)
        {
        case -1:
            printf("\nError: Phonebook is full\n");
            break;
        case -2:
            printf("\nError: All fields are required!\n");
            break;
        case -3:
            printf("\nError: Wrong ID\n");
            break;
        case -4:
            printf("\nWarning: Phonebook is empty\n");
            break;
        default:
            
            break;
        }
    } while (choice != 0);

    for (int i = 0; i < currentPosition; i++)
    {
        deletePerson(i, &root);
    }
    
    return 0;
}