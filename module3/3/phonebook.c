#include "phonebook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//Логика

Person persons[MAX_CONTACTS];
int currentPosition = 0;

//Сразу выделить/перевыделить память под динамический charник и скопировать туда
char* copyString(const char* source) {
    char* dest = (char*)malloc(strlen(source) + 1);
    strcpy(dest, source);
    return dest;
}

int readFileToArr(int filedescriptor)
{
    lseek(filedescriptor, 0,SEEK_CUR);

    currentPosition = 0;

    int bytesRead;
    Person tempPerson;

    while (currentPosition < MAX_CONTACTS)
    {
        bytesRead = read(filedescriptor, &tempPerson, sizeof(Person));

        if (bytesRead == 0)
        {
            break;
        }

        if (bytesRead == sizeof(Person))
        {
            persons[currentPosition++] = tempPerson;
        }
        else
        {
            return -1;
        }
    }
    

    return 0;
}

int writeArrToFile(int filedescriptor)
{
    //ftruncate(filedescriptor, 0);
    lseek(filedescriptor, 0, SEEK_SET);

    for (int i = 0; i < currentPosition; i++)
    {
        ssize_t bWritten = write(filedescriptor, &persons[i],sizeof(Person));
        if(bWritten != sizeof(Person))
        {
            return -1;
        }
    }
    return 0;
}

int isFileEmpty(int filedescriptor)
{
    off_t cur_pos = lseek(filedescriptor, 0, SEEK_CUR);
    off_t file_size = lseek(filedescriptor, 0, SEEK_END);
    lseek(filedescriptor, cur_pos, SEEK_SET);
    return file_size == 0;
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

    strcpy(newPerson.name, p_name);
    strcpy(newPerson.surname, p_surname);
    strcpy(newPerson.patronym, p_patronym);
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

    for (int i = personID; i < currentPosition - 1; i++)
    {
        persons[i]=persons[i+1];
    }
    
    if (currentPosition > 0) {
        memset(&persons[currentPosition-1],0,sizeof(Person));
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
                        strncpy(persons[personID].name, new_name, NAME_LEN);
                    }
                }
                break;
            case 's'://Изменение фамилии
                {
                    const char* new_surname = va_arg(args, const char*);
                    if (new_surname && strlen(new_surname) > 0) {
                        strncpy(persons[personID].surname, new_surname, NAME_LEN);
                    }
                }
                break;
            case 'p': //Изменение отчества
                {
                    const char* new_patronym = va_arg(args, const char*);
                    if (new_patronym && strlen(new_patronym) > 0) {
                        strncpy(persons[personID].patronym, new_patronym, NAME_LEN);
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

int createPerson_ui(int filedescriptor)
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
int deletePerson_ui(int filedescriptor)
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
int editPerson_ui(int filedescriptor)
{
    
    if (currentPosition == 0) {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to edit: ");
    scanf("%d", &personID);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    if (personID < 0 || personID >= currentPosition) {
        printf("\nWrong ID!\n");
        return -3; // Неверный ID
    }

    char format[FORMAT_LEN];
    printf("\nEnter format of editng: ");
    scanf("%s", format);

    // Очищаем буфер stdin после scanf
    while ((c = getchar()) != '\n' && c != EOF);

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
                printf("\nВведиет новое имя: ");
                fgets(new_name, NAME_LEN, stdin);
                new_name[strcspn(new_name, "\n")] = 0;
                break;
            case 's':
                printf("\nВведиет новую фамилию: ");
                fgets(new_surname, NAME_LEN, stdin);
                new_surname[strcspn(new_surname, "\n")] = 0;
                break;
            case 'p':
                printf("\nВведиет новое отчество: ");
                fgets(new_patronym, NAME_LEN, stdin);
                new_patronym[strcspn(new_patronym, "\n")] = 0;
                break;
            case 'j':
                printf("\nВведиет новую работу: ");
                fgets(new_job, JOB_LEN, stdin);
                new_job[strcspn(new_job, "\n")] = 0;
                break;
            case 'P':
                printf("\nВведиет новый телефон: ");
                fgets(new_phone, PHONE_LEN, stdin);
                new_phone[strcspn(new_phone, "\n")] = 0;
                break;
            default:
                printf("\nНеверный формат\n");
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
        printf("\nНеверный ID!\n");
        break;
    default:
        break;
    }

    return result;
}

void showAllPersons_ui(int filedescriptor)
{
    if (currentPosition == 0)
    {
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
            persons[i].patronym[0] ? persons[i].patronym : "-",
            persons[i].phone[0] ? persons[i].phone : "-",
            persons[i].job[0] ? persons[i].job : "-");
    }
}