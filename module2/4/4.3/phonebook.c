#include "phonebook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//Логика

phonebook *root = NULL;

int currentPosition = 0;

//Сразу выделить/перевыделить память под динамический charник и скопировать туда
char* copyString(const char* source) {
    char* dest = (char*)malloc(strlen(source) + 1);
    strcpy(dest, source);
    return dest;
}
//Вставка в дерево
void insTree(phonebook **node, int key, Person person)
{
    if (*node == NULL)
    {
        *node = malloc(sizeof(phonebook));
        (*node)->left = (*node)->right = NULL;
        (*node)->key = key;
        (*node)->person = person;
        return;
    }
    if ((*node)->key > key)
    {
        insTree(&(*node)->left, key, person);
    }
    else
    {
        insTree(&(*node)->right, key, person);
    }
    
}

void printTree(phonebook *node)
{
    if (node == NULL)
        return;
    printTree(node->left);
    printf("%d\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
           node->key,
           node->person.surname,
           node->person.name,
           node->person.patronym ? node->person.patronym : "-",
           node->person.phone[0] ? node->person.phone : "-",
           node->person.job[0] ? node->person.job : "-");
    printTree(node->right);
}
phonebook *findNode(phonebook *node, int key)
{
    phonebook *result = NULL;
    if (node == NULL)
    {
        return NULL;
    }
    if (node->key == key)
    {
        return node;
    }
    result = findNode(node->left, key);
    if (result != NULL)
    {
        return result;
    }
    result = findNode(node->right, key);
    if (result != NULL)
    {
        return result;
    }
    return result;
}
/*
int treeDel(phonebook **node)
{
    if ((*node)->left == NULL && (*node)->right == NULL)
    {
        free((*node)->person.name);
        free((*node)->person.surname);
        free((*node)->person.patronym);
        free()
    }

    return 0;
}
*/

// Создание персоны
int createPerson(char p_name[], char p_surname[], char p_patronym[], phonebook **node)
{
    if (currentPosition >= MAX_CONTACTS) {
        return -1; // Переполнение
    }

    if (!p_name || !p_surname || !p_patronym || strlen(p_name) == 0 || strlen(p_surname) == 0 || strlen(p_patronym) == 0) {
        return -2; // Обязательные поля не заполнены
    }

    Person newPerson;


    newPerson.name = copyString(p_name);
    newPerson.surname = copyString(p_surname);
    newPerson.patronym = copyString(p_patronym);
    strcpy(newPerson.job, "");
    strcpy(newPerson.phone, "");

    insTree(node, currentPosition, newPerson);
    //persons[currentPosition] = newPerson;
    currentPosition++;

    return 0;
}
int deletePerson(int personID, phonebook **node)
{
    if (personID < 0 || personID >= currentPosition) {
        return -3; // Неверный ID
    }

    phonebook *t, *up;
    if (*node == NULL)
        return 0; // нет такого значения в дереве

    if ((*node)->key == personID)
    {
        //если значение в листе, то удаляем лист
        if(((*node)->left == NULL) && ((*node)->right == NULL))
        {
            free((*node)->person.name);
            free((*node)->person.surname);
            free((*node)->person.patronym);
            *node=NULL;
            return 1;
        }
        //Если только правый потомок
        if((*node)->left == NULL)
        {
            t = *node;
            *node = (*node)->right;
            free(t->person.name);
            free(t->person.surname);
            free(t->person.patronym);
            free(t);
            return 1;
        }
        //Если только левый потомок
        if ((*node)->right == NULL)
        {
            t = *node;
            *node = (*node)->left;
            free(t->person.name);
            free(t->person.surname);
            free(t->person.patronym);
            free(t);
            return 1;
        }
        //Если и левый и правый потомки есть
        up=*node;
        t=(*node)->left;
        while (t->right !=NULL)
        {
            up=t;
            t=t->right;
        }
        (*node)->key = t->key;
        (*node)->person = t->person;
        
        if (up!=(*node))
        {
            if(t->left != NULL) {up->right = t->left;}
            else 
            {
                //up->right->person
                up->right = NULL;
            }
        }
        else (*node)->left = t->left;
        free(t->person.name);
        free(t->person.surname);
        free(t->person.patronym);
        free(t);
        return 1;
    }

    if ((*node)->key < personID)
    {
        return deletePerson(personID, &(*node)->right);
    }
    return deletePerson(personID, &(*node)->left);
}
int editPerson(int personID, phonebook *node, char format[], ...)
{
   
    if (personID < 0 || personID >= currentPosition) {
        return -3; // Неверный ID
    }

    va_list args;
    
    va_start(args, format);

    phonebook *prs= findNode(node, personID);

    for (int i = 0; format[i] != '\0'; i++)
    {
        
        if (format[i] == '%') {
            i++;
            switch (format[i])
            {
            case 'n': //Изменение имени
                {
                    const char* new_name = va_arg(args, const char*);
                    if (new_name && strlen(new_name) > 0) {
                        free(prs->person.name);
                        prs->person.name = copyString(new_name);
                    }
                }
                break;
            case 's'://Изменение фамилии
                {
                    const char* new_surname = va_arg(args, const char*);
                    if (new_surname && strlen(new_surname) > 0) {
                        free(prs->person.surname);
                        prs->person.surname = copyString(new_surname);
                    }
                }
                break;
            case 'p': //Изменение отчества
                {
                    const char* new_patronym = va_arg(args, const char*);
                    if (new_patronym && strlen(new_patronym) > 0) {
                        free(prs->person.patronym);
                        prs->person.patronym = copyString(new_patronym);
                    }
                }
                break;
            case 'j': //Изменение работы
                {
                    const char* new_job = va_arg(args, const char*);
                    if (new_job && strlen(new_job) > 0) {
                        strncpy(prs->person.job, new_job, JOB_LEN);
                    }
                }
                break;
            case 'P': //Изменение номера телефона
                {
                    const char* new_phone = va_arg(args, const char*);
                    if (new_phone && strlen(new_phone) > 0) {
                        strncpy(prs->person.phone, new_phone, PHONE_LEN - 1);
                        prs->person.phone[PHONE_LEN - 1] = '\0';
                    }
                }
                break;
            default:
                return -4; //Неверный формат
            }
        }
    }
    
    va_end(args);
    
    return 0;

}


//Интерфейс

int createPerson_ui(phonebook **book)
{
    //const int buffSize=100;
    char tmpName[NAME_LEN], tmpSurname[NAME_LEN], tmpPatronym[NAME_LEN];
   

    printf("Enter contact name: ");
    scanf("%s",tmpName);

    printf("Enter contact surname: ");
    scanf("%s",tmpSurname);
    
    printf("Enter contact patronym: ");
    scanf("%s",tmpPatronym);

    int result = createPerson(tmpName, tmpSurname, tmpPatronym, book);

    switch (result)
    {
        case 0:
            printf("\nContact added succesfully! ID: %d\n", currentPosition-1);
            break;
        case -1:
            printf("\nError: Phonebook is full\n");
            break;
        case -2:
            printf("\nError: All fields are required!\n");
            break;
    }

    return result;
}
int deletePerson_ui(phonebook **book)
{
    if (currentPosition == 0) {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to delete: ");
    scanf("%d", &personID);

    int result = deletePerson(personID, book);

    switch (result)
    {
    case 0:
        printf("\nContact deleted succsesfully!");
        break;
    case -3:
        printf("\nWrong ID!\n");
        break;
    default:
        break;
    }


    return result;
}
int editPerson_ui(phonebook *book)
{
    
    if (currentPosition == 0) {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to edit: ");
    scanf("%d", &personID);

    if (personID < 0 || personID >= currentPosition) {
        printf("\nWrong ID!\n");
        return -3; // Неверный ID
    }

    char format[FORMAT_LEN];
    printf("\nEnter format of editng: ");
    scanf("%s", format);

    char new_name[NAME_LEN] = "";
    char new_surname[NAME_LEN] = "";
    char new_patronym[NAME_LEN] = "";
    char new_job[JOB_LEN] = "";
    char new_phone[PHONE_LEN] = "";

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            if (format[i] == '\0') break;

            switch (format[i]) {
            case 'n':
                printf("\nEnter new name: ");
                scanf("%s", new_name);
                break;
            case 's':
                printf("\nEnter new surname: ");
                scanf("%s", new_surname);
                break;
            case 'p':
                printf("\nEnter new patronym: ");
                scanf("%s", new_patronym);
                break;
            case 'j':
                printf("\nEnter new job: ");
                scanf("%s", new_job);
                break;
            case 'P':
                printf("\nEnter new phone: ");
                scanf("%12s", new_phone);
                break;
            default:
                printf("\nWrong Fromat\n");
                return -5;
            }
        }
    }


    const char* arguments[5];

    int arg_count = 0;

    int result = -1;

    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%') {
            i++;
            switch (format[i]) {
            case 'n': 
                arguments[arg_count++] = new_name; 
                break;
            case 's': 
                arguments[arg_count++] = new_surname; 
                break;
            case 'p': 
                arguments[arg_count++] = new_patronym; 
                break;
            case 'j': 
                arguments[arg_count++] = new_job; 
                break;
            case 'P': 
                arguments[arg_count++] = new_phone; 
                break;
            default:
                return result;
            }
    
        }
    }

    switch (arg_count)
    {
    case 1:
        result = editPerson(personID, book, format, arguments[0]);
        break;
    case 2:
        result = editPerson(personID, book, format, arguments[0], arguments[1]);
        break;
    case 3:
        result = editPerson(personID, book, format, arguments[0], arguments[1], arguments[2]);
        break;
    case 4:
        result = editPerson(personID, book, format, arguments[0], arguments[1], arguments[2], arguments[3]);
        break;
    case 5:
        result = editPerson(personID, book, format, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
        break;
    default:
        break;
    }
    
    switch (result)
    {
    case 0:
        printf("\nContact edited successfully!\n");
        break;
    case -3:
        printf("\nWrong ID!\n");
        break;
    default:
        break;
    }

    return result;
}

void showAllPersons_ui(phonebook *book)
{
    if (currentPosition == 0) {
        printf("\nPhonebook is empty!\n");
        return;
    }

    printf("\n=== ALL CONTACTS (%d) ===\n", currentPosition);
    printf("ID\tSurname\t\tName\t\tPatronym\tPhone\t\tJob\n");
    printf("------------------------------------------------------------------------\n");

    printTree(book);
}