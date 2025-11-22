
#ifndef PHONEBOOK_H
#define PHONEBOOK_H


#define MAX_CONTACTS 100
#define JOB_LEN 50
#define PHONE_LEN 13
#define FORMAT_LEN 11
#define NAME_LEN 50

typedef struct {

   // int perID;
    char* name;
    char* surname;
    char* patronym;

    char phone[PHONE_LEN];
    char job[JOB_LEN];

} Person;

typedef struct phonebook
{
    int key;
    Person person;
    struct phonebook *left, *right;

} phonebook;

extern int currentPosition;
extern phonebook *root;

char* copyString(const char* source);

//Операции с деревом
void insTree(phonebook **node,  int key, Person person);
void printTree(phonebook *node);
phonebook *findNode(phonebook *node, int key);
//int treeDel(phonebook **node);

// Логика

int createPerson(char p_name[], char p_surname[], char p_patronym[], phonebook **node);
int deletePerson(int personID, phonebook **node);
int editPerson(int personID, phonebook *node, char format[], ...);

//Интерфейс

int createPerson_ui(phonebook **book);
int deletePerson_ui(phonebook **book);
int editPerson_ui(phonebook *book);
void showAllPersons_ui(phonebook *book);

#endif