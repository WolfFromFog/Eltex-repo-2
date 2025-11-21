#include "phonebook.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

int positionCounter = 0;
Person mass[MAX_CONTACTS];

int main()
{
    int filedesc;
    int choice;
    int result;
    char filename[] ="phonebook.txt";

    filedesc = open(filename, O_RDWR | O_CREAT, 0777);
    if (filedesc ==-1)
    {
        perror("Ошибка открытия");
        return -1;
    }
    
    if(readFileToArr(filedesc)!=0)
    {
        printf("Файл пуст\n");
    }

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
                showAllPersons_ui(filedesc);
                break;
            case 2:
                printf("\nAdding contact...\n");
                result = createPerson_ui(filedesc);
                break;
            case 3:
                printf("\nEditing contact...\n");
                result = editPerson_ui(filedesc);
                break;
            case 4:
                printf("\nDeleting contact...\n");
                result = deletePerson_ui(filedesc);
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

    writeArrToFile(filedesc);

    close(filedesc);

    return 0;
}