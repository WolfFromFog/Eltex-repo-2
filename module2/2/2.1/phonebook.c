#include "phonebook.h"
#include <stdio.h>
#include <string.h>

//Логика

void createPerson(Person* persons, int currentPosition, char p_name[], char p_surname[], char p_patronym[])
{
    Person p;
    p.name = p_name;
    p.surname = p_surname;
    p.patronym = p_patronym;

    persons[currentPosition + 1] = p;
}
void deletePerson(int personID)
{

}
void editPerson(int personID, char format[], ...)
{

}


//Интерфейс

int createPerson_ui(Person* persons,int currentPosition)
{
    const int buffSize=100;
    char tmpName[100], tmpSurname[100], tmpPatronym[100];
    int nameSize, suernameSize, patronymSize;

    printf("Enter contact name: ");
    scanf("%s",tmpName);
    nameSize = strlen(tmpName);
    printf("Name is: %s , size is: %d \n", tmpName, nameSize);

    printf("Enter contact surname: ");
    scanf("%s",tmpSurname);
    suernameSize = strlen(tmpSurname);
    printf("Surname is: %s , size is: %d \n", tmpSurname, suernameSize);

    printf("Enter contact patronym: ");
    scanf("%s",tmpPatronym);
    patronymSize = strlen(tmpPatronym);
    printf("Patronym is: %s , size is: %d \n", tmpPatronym, patronymSize);

    char name[nameSize], surname[suernameSize], patronym[patronymSize];
    strcopy(name, tmpName, nameSize);
    strcopy(surname, tmpSurname, suernameSize);
    strcopy(patronym, tmpPatronym, patronymSize);

    createPerson(persons, currentPosition, name, surname, patronym);

    return 0;
}
int deletePerson_ui(int currentPosition)
{
    printf("Enter contact ID: ");

    return 0;
}
int editPerson_ui(int currentPosition)
{
    printf("Enter contact ID: ");

    return 0;
}