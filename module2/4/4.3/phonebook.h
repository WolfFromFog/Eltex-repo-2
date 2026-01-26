
#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#define MAX_CONTACTS 100
#define JOB_LEN 50
#define PHONE_LEN 13
#define FORMAT_LEN 11
#define NAME_LEN 50

typedef struct
{

    int perID;
    char *name;
    char *surname;
    char *patronym;

    char phone[PHONE_LEN];
    char job[JOB_LEN];

} Person;

typedef struct phonebook
{
    Person person;
    struct phonebook *left, *right;

} phonebook;

extern int currentPosition;
extern phonebook *root;

char *copyString(const char *source);

// Операции с деревом
void printTreeAsTree(phonebook *node, int level);

// Функции балансировки древа

void storeInOrder(phonebook *root, phonebook nodes);

// Логика

int createPerson(char p_name[], char p_surname[], char p_patronym[], phonebook **node);
int deletePerson(int personID, phonebook **node);
int editPerson(int personID, phonebook *node, char format[], ...);
int deleteAll(phonebook **node);

// Интерфейс

int createPerson_ui(phonebook **book);
int deletePerson_ui(phonebook **book);
int editPerson_ui(phonebook *book);
void showAllPersons_ui(phonebook *book);

#endif