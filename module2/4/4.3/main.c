#include "phonebook.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{

    int choice;
    int result = 0;
    

    printf("\n=== СПИСОК КОНТАКТОВ ===\n");
    
    do {
        printf("\n--- МЕНЮ ---\n");
        printf("1. Показать все контакты\n");
        printf("2. Добавить контакт\n");
        printf("3. Изменить контакт\n");
        printf("4. Удалить контакт\n");
        printf("5. Отобразить древо\n");
        printf("6. Отбалансировать древо\n");
        printf("0. Выйти\n");
        printf("Выбирите действие: ");


        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("\nОтображаем контакты...\n");
                showAllPersons_ui(root);
                verifyAllIDs(root);
                break;
            case 2:
                printf("\nДобавляем контакты...\n");
                result = createPerson_ui(&root);
                break;
            case 3:
                printf("\nИзменяем контакты...\n");
                result = editPerson_ui(root);
                break;
            case 4:
                printf("\nУдаляем контакты...\n");
                result = deletePerson_ui(&root);
                break;
            case 5:
                printf("\nПоказываем древо...\n");
                printTreeAsTree(root, 0);
                break;
            case 6:
                printf("\nПеребалансировка дерева.\n");
                balanceTree_ui(&root);
                printf("Перебалнсировка завершена!\n");
                break;
            case 0:
                printf("\nПрограмма закрывается...\n");
                break;
            default:
                printf("\nНевреный выбор, попробуйте снова.\n");
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