
#ifndef PHONEBOOK_H
#define PHONEBOOK_H


#define MAX_CONTACTS 100
#define JOB_LEN 50
#define PHONE_LEN 12

typedef struct {

    int perID;
    char* name;
    char* surname;
    char* patronym;

    char  phone[PHONE_LEN];
    char job[JOB_LEN];

} Person;



//Логика

void createPerson(Person persons[], int currentPosition, char p_name[], char p_surname[], char p_patronym[]);
void deletePerson(int personID);
void editPerson(int personID, char format[], ...);


//Интерфейс

int createPerson_ui(int currentPosition);
int deletePerson_ui(int currentPosition);
int editPerson_ui(int currentPosition);

#endif