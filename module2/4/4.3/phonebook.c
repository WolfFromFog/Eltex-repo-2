#include "phonebook.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// Логика

phonebook *root = NULL;

int currentPosition = 0;

// Сразу выделить/перевыделить память под динамический charник и скопировать туда
char *copyString(const char *source)
{
    char *dest = (char *)malloc(strlen(source) + 1);
    strcpy(dest, source);
    return dest;
}

// Вставка в дерево
void insTree(phonebook **node, Person person)
{
    if (*node == NULL)
    {
        *node = malloc(sizeof(phonebook));
        (*node)->left = (*node)->right = NULL;
        (*node)->person = person;
        return;
    }

    if (strcmp((*node)->person.surname, person.patronym) > 0)
    {
        insTree(&(*node)->left, person);
    }
    else
    {
        insTree(&(*node)->right, person);
    }
}

// Вывод содержимого древа

void printTree(phonebook *node)
{
    if (node == NULL)
        return;
    printTree(node->left);
    printf("%d\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
           node->person.perID,
           node->person.surname,
           node->person.name,
           node->person.patronym ? node->person.patronym : "-",
           node->person.phone[0] ? node->person.phone : "-",
           node->person.job[0] ? node->person.job : "-");
    printTree(node->right);
}
phonebook *findMin(phonebook *node)
{
    while (node->left != NULL)
        node = node->left;
    return node;
}

// Поиск узла
phonebook *findNode(phonebook *node, int perID)
{
    phonebook *result = NULL;
    if (node == NULL)
    {
        return NULL;
    }
    if (node->person.perID == perID)
    {
        return node;
    }
    result = findNode(node->left, perID);
    if (result != NULL)
    {
        return result;
    }
    result = findNode(node->right, perID);
    if (result != NULL)
    {
        return result;
    }
    return result;
}

// Создание персоны
int createPerson(char p_name[], char p_surname[], char p_patronym[], phonebook **node)
{
    if (currentPosition >= MAX_CONTACTS)
    {
        return -1; // Переполнение
    }

    if (!p_name || !p_surname || !p_patronym || strlen(p_name) == 0 || strlen(p_surname) == 0 || strlen(p_patronym) == 0)
    {
        return -2; // Обязательные поля не заполнены
    }

    Person newPerson;

    newPerson.perID = currentPosition;
    newPerson.name = copyString(p_name);
    newPerson.surname = copyString(p_surname);
    newPerson.patronym = copyString(p_patronym);
    strcpy(newPerson.job, "");
    strcpy(newPerson.phone, "");

    insTree(node, newPerson);
    // persons[currentPosition] = newPerson;
    currentPosition++;

    return 0;
}
int deletePerson(int personID, phonebook **node)
{
    int result = 0;

    if (personID < 0 || personID >= currentPosition)
    {
        return result = -1; // Неверный диапозон ID
    }

    if (*node == NULL)
    {
        return result = -2; // нет такого значения в дереве
    }

    if (personID == (*node)->person.perID) // Найден нужный
    {
        // Нет потомков
        if ((*node)->left == NULL && (*node)->right == NULL) // Случай, когда нет потомков
        {
            free((*node)->person.name);
            free((*node)->person.surname);
            free((*node)->person.patronym);
            free(*node);
            printf("Удаление пользователя %d завершено.", personID);
            return 0;
        }
        else
        {
            phonebook *temp = *node;
            result = 1;
            // Есть только правый
            if ((*node)->left == NULL)
            {
                *node = (*node)->right;
                free(temp->person.name);
                free(temp->person.surname);
                free(temp->person.patronym);
                free(temp);

                result = 0;
            }
            // Есть только левый
            else if ((*node)->right == NULL)
            {
                *node = (*node)->left;
                free(temp->person.name);
                free(temp->person.surname);
                free(temp->person.patronym);
                free(temp);

                result = 0;
            }
            // Есть оба
            else
            {
                phonebook *minNode = findMin((*node)->right);

                char *old_name = (*node)->person.name;
                char *old_surname = (*node)->person.surname;
                char *old_patronym = (*node)->person.patronym;

                //(*node)->key = minNode->key;
                (*node)->person.name = copyString(minNode->person.name);
                (*node)->person.surname = copyString(minNode->person.surname);
                (*node)->person.patronym = copyString(minNode->person.patronym);
                strcpy((*node)->person.phone, minNode->person.phone);
                strcpy((*node)->person.job, minNode->person.job);

                free(old_name);
                free(old_surname);
                free(old_patronym);

                result = deletePerson(minNode->person.perID, &(*node)->right);
            }
        }
    }
    else
    {
        result = deletePerson(personID, &(*node)->left);
        if (result == 0)
        {
            return result;
        }

        result = deletePerson(personID, &(*node)->right);
        if (result == 0)
        {
            return result;
        }
    }
    return result;
}
int editPerson(int personID, phonebook *node, char format[], ...)
{

    if (personID < 0 || personID >= currentPosition)
    {
        return -3; // Неверный ID
    }

    va_list args;

    va_start(args, format);

    phonebook *prs = findNode(node, personID);

    for (int i = 0; format[i] != '\0'; i++)
    {

        if (format[i] == '%')
        {
            i++;
            switch (format[i])
            {
            case 'n': // Изменение имени
            {
                const char *new_name = va_arg(args, const char *);
                if (new_name && strlen(new_name) > 0)
                {
                    free(prs->person.name);
                    prs->person.name = copyString(new_name);
                }
            }
            break;
            case 's': // Изменение фамилии
            {
                const char *new_surname = va_arg(args, const char *);
                if (new_surname && strlen(new_surname) > 0)
                {
                    free(prs->person.surname);
                    prs->person.surname = copyString(new_surname);
                }
            }
            break;
            case 'p': // Изменение отчества
            {
                const char *new_patronym = va_arg(args, const char *);
                if (new_patronym && strlen(new_patronym) > 0)
                {
                    free(prs->person.patronym);
                    prs->person.patronym = copyString(new_patronym);
                }
            }
            break;
            case 'j': // Изменение работы
            {
                const char *new_job = va_arg(args, const char *);
                if (new_job && strlen(new_job) > 0)
                {
                    strncpy(prs->person.job, new_job, JOB_LEN);
                }
            }
            break;
            case 'P': // Изменение номера телефона
            {
                const char *new_phone = va_arg(args, const char *);
                if (new_phone && strlen(new_phone) > 0)
                {
                    strncpy(prs->person.phone, new_phone, PHONE_LEN - 1);
                    prs->person.phone[PHONE_LEN - 1] = '\0';
                }
            }
            break;
            default:
                return -4; // Неверный формат
            }
        }
    }

    va_end(args);

    return 0;
}
int deleteAll(phonebook **node)
{
    if (*node == NULL)
    {
        return -1;
    }

    int resultL, resultR;

    if ((*node)->left != NULL)
    {
        resultL = deleteAll(&(*node)->left);
        if (resultL == -1)
            printf("Ошбика удаления пользователя %d завершено. Он уже удалён\n", (*node)->person.perID);
    }

    if ((*node)->right != NULL)
    {
        resultR = deleteAll(&(*node)->right);
        if (resultR == -1)
            printf("Ошбика удаления пользователя %d завершено. Он уже удалён\n", (*node)->person.perID);
    }

    int persID = (*node)->person.perID;
    free((*node)->person.name);
    free((*node)->person.surname);
    free((*node)->person.patronym);
    free(*node);
    printf("Удаление пользователя %d завершено.\n", persID);

    return 0;
}
// Интерфейс

int createPerson_ui(phonebook **book)
{
    // const int buffSize=100;
    char tmpName[NAME_LEN], tmpSurname[NAME_LEN], tmpPatronym[NAME_LEN];

    printf("Enter contact name: ");
    scanf("%s", tmpName);

    printf("Enter contact surname: ");
    scanf("%s", tmpSurname);

    printf("Enter contact patronym: ");
    scanf("%s", tmpPatronym);

    int result = createPerson(tmpName, tmpSurname, tmpPatronym, book);

    switch (result)
    {
    case 0:
        printf("\nContact added succesfully! ID: %d\n", currentPosition - 1);
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
    if (currentPosition == 0)
    {
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

    if (currentPosition == 0)
    {
        printf("\nPhonebook is empty!\n");
        return -4;
    }

    int personID;
    printf("\nEnter contact ID to edit: ");
    scanf("%d", &personID);

    if (personID < 0 || personID >= currentPosition)
    {
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

    for (int i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            i++;
            if (format[i] == '\0')
                break;

            switch (format[i])
            {
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

    const char *arguments[5];

    int arg_count = 0;

    int result = -1;

    for (int i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            i++;
            switch (format[i])
            {
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
    if (currentPosition == 0)
    {
        printf("\nPhonebook is empty!\n");
        return;
    }

    printf("\n=== ALL CONTACTS (%d) ===\n", currentPosition);
    printf("ID\tSurname\t\tName\t\tPatronym\tPhone\t\tJob\n");
    printf("------------------------------------------------------------------------\n");

    printTree(book);
}

// Вывод древа в виде древа
void printTreeAsTree(phonebook *node, int level)
{
    if (node == NULL)
    {
        return;
    }

    printTreeAsTree(node->right, level + 1);
    for (int i = 0; i < level; i++)
        printf("    ");
    printf("%d\n", node->person.perID);
    printTreeAsTree(node->left, level + 1);
}

Person copyPerson(const Person *src)
{
    Person dest;
    dest.perID = src->perID;

    dest.name = src->name ? copyString(src->name) : NULL;
    dest.surname = src->surname ? copyString(src->surname) : NULL;
    dest.patronym = src->patronym ? copyString(src->patronym) : NULL;

    strncpy(dest.phone, src->phone, PHONE_LEN - 1);
    dest.phone[PHONE_LEN - 1] = '\0';

    strncpy(dest.job, src->job, JOB_LEN - 1);
    dest.job[JOB_LEN - 1] = '\0';

    return dest;
}

phonebook *buildBalancedTree(Person *arr[], int start, int end)
{

    if (start > end)
    {
        return NULL;
    }

    int mid = (start + end) / 2;
    phonebook *root = (phonebook *)malloc(sizeof(phonebook));

    if (!root)
        return NULL;

    root->person = copyPerson(arr[mid]);
    root->left = buildBalancedTree(arr, start, mid - 1);
    root->right = buildBalancedTree(arr, mid + 1, end);

    return root;
}

phonebook *balanceBST(phonebook *root)
{

    return NULL;
}

void printLevelOrder(phonebook *root)
{
}

void storeInOrder(phonebook *node, Person *arr[], int *index)
{
    if (node == NULL)
        return;

    storeInOrder(node->left, arr, index);
    arr[(*index)++] = &(node->person);
    storeInOrder(node->right, arr, index);
}
