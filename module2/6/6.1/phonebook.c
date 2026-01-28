#include "phonebook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

//Логика


Persons* head= NULL;
Persons* tail = NULL;

int persons_count = 0;

//Сразу выделить/перевыделить память под динамический charник и скопировать туда
char* copyString(const char* source) {
    char* dest = (char*)malloc(strlen(source) + 1);
    strcpy(dest, source);
    return dest;
}
//Сравнить два контакта: 0 - равны, <0 первый стоить раньше второго, >0 первый стоит позже второго
int comparePersons(Person* pers1, Person* pers2)
{
    int surname_compare = strcmp(pers1->surname, pers2->surname);
    if (surname_compare !=0 ) return surname_compare;

    int name_compare = strcmp(pers1->name, pers2->name);
    if (name_compare !=0 ) return name_compare;

    return strcmp(pers1->patronym, pers2->patronym);
}
//Поиск узла по контакту: NULL - равен head, возвращает указатель на узел. 
Persons* findInsertPosition(Person* newPers)
{
    Persons* current = head;
    Persons* prev = NULL;

    while (current != NULL)
    {
        if(comparePersons(newPers, current->person)<=0) break;
        prev = current;
        current = current->next;
    }

    return prev;
}

int createPerson(char p_name[], char p_surname[], char p_patronym[])
{
    if (!p_name || !p_surname || !p_patronym || strlen(p_name) == 0 || strlen(p_surname) == 0 || strlen(p_patronym) == 0) {
        return -2; // Обязательные поля не заполнены
    }

    Person* newPerson = (Person*)malloc(sizeof(Person));
    //new
    newPerson->name = copyString(p_name);
    newPerson->surname = copyString(p_surname);
    newPerson->patronym = copyString(p_patronym);
    strcpy(newPerson->job, "");
    strcpy(newPerson->phone, "");

    Persons* newNode = (Persons*)malloc(sizeof(Persons));
    newNode->person=newPerson;
    newNode->prev = NULL;
    newNode->next = NULL;

    if(head==NULL)
    {
        //Если первая вставка
        head = newNode;
        tail = newNode;
    }
    else 
    {
        Persons* insertAfter = findInsertPosition(newPerson);
        if (insertAfter == NULL)
        {
            //В начало
            newNode->next=head;
            head->prev=newNode;
            head = newNode;
        } 
        else 
        {
            if (insertAfter == tail)
                {
                    //В конец
                    tail->next=newNode;
                    newNode->next=NULL;
                    newNode->prev=tail;
                    tail=newNode;
                }
                else 
                {
                    //В середину
                    newNode->prev = insertAfter;
                    newNode->next = insertAfter->next;
                    insertAfter->next->prev=newNode;
                    insertAfter->next=newNode;
                    
                }
        }
            
        
    }

    persons_count++;

    return 0;
}

Persons* findPersonByID(int personID)
{
    if (personID < 0 || personID >= persons_count) {
        printf("Ошибка: Выход за границы!\n");
        return NULL; // Неверный ID
    }

    Persons* current = head;

    for (int i = 0; i < personID && current!=NULL; i++)
    {
        current = current->next;
    }
    return current;
}

int deletePerson(int personID)
{
    

    Persons* nodeToDelete = findPersonByID(personID);
    
    if (nodeToDelete == NULL) {
        return -3; // Неверный ID
    }

    if(nodeToDelete->prev!=NULL)
    {
        nodeToDelete->prev->next = nodeToDelete->next;
    }
    else
    {
        head = nodeToDelete->next;
    }

    if (nodeToDelete->next!=NULL)
    {
        nodeToDelete->next->prev = nodeToDelete->prev;
    }
    else
    {
        tail=nodeToDelete->prev;
    }
    
    

    free(nodeToDelete->person->surname);
    free(nodeToDelete->person->name);
    free(nodeToDelete->person->patronym);
    free(nodeToDelete->person);
    free(nodeToDelete);
    
    persons_count--;
    return 0;
}
int editPerson(int personID, char format [], ...)
{
    Persons* nodeToEdit = findPersonByID(personID);
    if (nodeToEdit == NULL) {
        return -3; // Неверный ID
    }
    Person* personToEdit = nodeToEdit->person;

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
                        free(personToEdit->name);
                        personToEdit->name = copyString(new_name);
                    }
                }
                break;
            case 's'://Изменение фамилии
                {
                    const char* new_surname = va_arg(args, const char*);
                    if (new_surname && strlen(new_surname) > 0) {
                        free(personToEdit->surname);
                        personToEdit->surname = copyString(new_surname);
                    }
                }
                break;
            case 'p': //Изменение отчества
                {
                    const char* new_patronym = va_arg(args, const char*);
                    if (new_patronym && strlen(new_patronym) > 0) {
                        free(personToEdit->patronym);
                        personToEdit->patronym = copyString(new_patronym);
                    }
                }
                break;
            case 'j': //Изменение работы
                {
                    const char* new_job = va_arg(args, const char*);
                    if (new_job && strlen(new_job) > 0) {
                        strncpy(personToEdit->job, new_job, JOB_LEN);
                    }
                }
                break;
            case 'P': //Изменение номера телефона
                {
                    const char* new_phone = va_arg(args, const char*);
                    if (new_phone && strlen(new_phone) > 0) {
                        strncpy(personToEdit->phone, new_phone, PHONE_LEN - 1);
                        personToEdit->phone[PHONE_LEN - 1] = '\0';
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

//Освбождение всей памяти
void freeAll()
{
    Persons* current = head;
    while (current != NULL)
    {
        Persons* next = current->next;
        free(current->person->name);
        free(current->person->surname);
        free(current->person->patronym);
        free(current->person);
        free(current);
        current = next;
    }
    head = NULL;
    tail = NULL;
    persons_count=0;
}


//Интерфейс

int createPerson_ui()
{
    //const int buffSize=100;
    char tmpName[NAME_LEN], tmpSurname[NAME_LEN], tmpPatronym[NAME_LEN];
   

    printf("Enter contact name: ");
    scanf("%49s",tmpName);

    printf("Enter contact surname: ");
    scanf("%49s",tmpSurname);
    
    printf("Enter contact patronym: ");
    scanf("%49s",tmpPatronym);
    

    int result = createPerson(tmpName, tmpSurname, tmpPatronym);

    switch (result)
    {
        case 0:
            printf("\nContact added succesfully! ID: %d\n", persons_count-1);
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
    if (head == NULL) {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to delete (0-%d): ", persons_count - 1);
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
    
    if (head == NULL) {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to edit (0-%d): ", persons_count - 1);
    scanf("%d", &personID);

    if (personID < 0 || personID >= persons_count) {
        printf("\nWrong ID!\n");
        return -3; // Неверный ID
    }

    char format[FORMAT_LEN];
    printf("\nEnter format of editng: ");
    scanf("%10s", format);

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
    if (head == NULL) {
        printf("\nPhonebook is empty!\n");
        return;
    }

    printf("\n=== ALL CONTACTS (%d) ===\n", persons_count);
    printf("ID\tSurname\t\tName\t\tPatronym\tPhone\t\tJob\n");
    printf("------------------------------------------------------------------------\n");

    Persons* current=head;
    int id = 0; 
    while (current != NULL) {
        printf("%d\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
            id,
            current->person->surname,
            current->person->name,
            current->person->patronym ? current->person->patronym : "-",
            current->person->phone[0] ? current->person->phone : "-",
            current->person->job[0] ? current->person->job : "-");

        current = current->next;
        id++;
    }
}