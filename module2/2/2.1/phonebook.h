
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

extern int currentPosition;
extern Person persons[];

char* copyString(const char* source);

//Логика

int createPerson(char p_name[], char p_surname[], char p_patronym[]);
int deletePerson(int personID);
int editPerson(int personID, char format [], ...);


//Интерфейс

int createPerson_ui();
int deletePerson_ui();
int editPerson_ui();
void showAllPersons_ui();

#endif