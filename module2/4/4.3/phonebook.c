#include "phonebook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//Логика

Person persons[MAX_CONTACTS];
int currentPosition = 0;

//Сразу выделить/перевыделить память под динамический charник и скопировать туда
char* copyString(const char* source) {
    char* dest = (char*)malloc(strlen(source) + 1);
    strcpy(dest, source);
    return dest;
}

void insTree(phonebook **book, int key, Person person)
{
    if (*book == NULL)
    {
        *book = malloc(sizeof(phonebook));
        (*book)->left = (*book)->right = NULL;
        (*book)->key = key;
        (*book)->person = person;
        return;
    }
    if((*book)->key > key)
    {
        insTree(&(*book)->left, key, person);
    }
    else
    {
        insTree(&(*book)->right, key, person);
    }
    
}

int createPerson(char p_name[], char p_surname[], char p_patronym[])
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

    persons[currentPosition] = newPerson;
    currentPosition++;

    return 0;
}
int deletePerson(int personID)
{
    if (personID < 0 || personID >= currentPosition) {
        return -3; // Неверный ID
    }

    free(persons[personID].name);
    free(persons[personID].surname);
    free(persons[personID].patronym);

    for (int i = personID; i < currentPosition - 1; i++)
    {
        
        persons[i].name = copyString(persons[i + 1].name);
        persons[i].surname = copyString(persons[i + 1].surname);
        persons[i].patronym = copyString(persons[i + 1].patronym);
        strcpy(persons[i].phone, persons[i + 1].phone);
        strcpy(persons[i].job, persons[i + 1].job);
    }
    
    if (currentPosition > 0) {

        //free(persons[currentPosition - 1].name);
        //free(persons[currentPosition - 1].surname);
        //free(persons[currentPosition - 1].patronym);
        persons[currentPosition - 1].name = NULL;
        persons[currentPosition - 1].surname = NULL;
        persons[currentPosition - 1].patronym = NULL;
        persons[currentPosition - 1].phone[0] = '\0';
        persons[currentPosition - 1].job[0] = '\0';
    }
    currentPosition--;
    return 0;
}
int editPerson(int personID, char format [], ...)
{
   
    if (personID < 0 || personID >= currentPosition) {
        return -3; // Неверный ID
    }

    va_list args;
    
    va_start(args, format);
    
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
                        free(persons[personID].name);
                        persons[personID].name = copyString(new_name);
                    }
                }
                break;
            case 's'://Изменение фамилии
                {
                    const char* new_surname = va_arg(args, const char*);
                    if (new_surname && strlen(new_surname) > 0) {
                        free(persons[personID].surname);
                        persons[personID].surname = copyString(new_surname);
                    }
                }
                break;
            case 'p': //Изменение отчества
                {
                    const char* new_patronym = va_arg(args, const char*);
                    if (new_patronym && strlen(new_patronym) > 0) {
                        free(persons[personID].patronym);
                        persons[personID].patronym = copyString(new_patronym);
                    }
                }
                break;
            case 'j': //Изменение работы
                {
                    const char* new_job = va_arg(args, const char*);
                    if (new_job && strlen(new_job) > 0) {
                        strncpy(persons[personID].job, new_job, JOB_LEN);
                    }
                }
                break;
            case 'P': //Изменение номера телефона
                {
                    const char* new_phone = va_arg(args, const char*);
                    if (new_phone && strlen(new_phone) > 0) {
                        strncpy(persons[personID].phone, new_phone, PHONE_LEN - 1);
                        persons[personID].phone[PHONE_LEN - 1] = '\0';
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

int createPerson_ui()
{
    //const int buffSize=100;
    char tmpName[NAME_LEN], tmpSurname[NAME_LEN], tmpPatronym[NAME_LEN];
   

    printf("Enter contact name: ");
    scanf("%s",tmpName);

    printf("Enter contact surname: ");
    scanf("%s",tmpSurname);
    
    printf("Enter contact patronym: ");
    scanf("%s",tmpPatronym);
    

    int result = createPerson(tmpName, tmpSurname, tmpPatronym);

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
int deletePerson_ui()
{
    if (currentPosition == 0) {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to delete: ");
    scanf("%d", &personID);

    int result = deletePerson(personID);
    
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
int editPerson_ui()
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
        result = editPerson(personID, format, arguments[0]);
        break;
    case 2:
        result = editPerson(personID, format, arguments[0], arguments[1]);
        break;
    case 3:
        result = editPerson(personID, format, arguments[0], arguments[1], arguments[2]);
        break;
    case 4:
        result = editPerson(personID, format, arguments[0], arguments[1], arguments[2], arguments[3]);
        break;
    case 5:
        result = editPerson(personID, format, arguments[0], arguments[1], arguments[2], arguments[3], arguments[4]);
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

void showAllPersons_ui()
{
    if (currentPosition == 0) {
        printf("\nPhonebook is empty!\n");
        return;
    }

    printf("\n=== ALL CONTACTS (%d) ===\n", currentPosition);
    printf("ID\tSurname\t\tName\t\tPatronym\tPhone\t\tJob\n");
    printf("------------------------------------------------------------------------\n");

    for (int i = 0; i < currentPosition; i++) {
        printf("%d\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
            i,
            persons[i].surname,
            persons[i].name,
            persons[i].patronym ? persons[i].patronym : "-",
            persons[i].phone[0] ? persons[i].phone : "-",
            persons[i].job[0] ? persons[i].job : "-");
    }
}