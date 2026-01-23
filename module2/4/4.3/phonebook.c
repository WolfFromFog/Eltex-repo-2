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
/*
void fixheight(phonebook *node)
{
    if (node == NULL)
    {
        return;
    }

    int hl = getHeight(node->left);
    int hr = getHeight(node->right);
    node->height = (hl > hr ? hl : hr) + 1;
}
*/

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

/*
//Поиск узла
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

//Получение высоты
int getHeight(phonebook *node)
{
    return node != NULL ? node->height : 0 ;
}
//Разница высот между левым и правыми потомками
int getBalance(phonebook *node)
{
    if(node==NULL)
    {
        return 0;
    }
    return getHeight(node->left)-getHeight(node->right);
}
//Правый поворот
phonebook *rotateRight(phonebook *y)
{
    phonebook *x = y->left;
    phonebook *T2 = x->right;

    x->right = y;
    y->left = T2;

    fixheight(y);
    fixheight(x);

    return x;
}
//Левый поворот
phonebook *rotateLeft(phonebook *x)
{
    phonebook *y = x->right;
    phonebook *T2 = y->left;

    x->right = y;
    y->left = T2;

    fixheight(x);
    fixheight(y);

    return y;
}
//Балансировка узла
phonebook *balanceTree(phonebook *node)
{
    if(node == NULL)
    {
        return NULL;
    }
    fixheight(node);

    int balance = getBalance(node);


    if (balance > 1 && getBalance(node->left) >=0 )
    {
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
    {
        return rotateLeft(node);
    }

    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}
//Сбор узлов в масив, чтоб перестроить
void storeNodesInOrder(phonebook *node, phonebook **nodes, int *index)
{
    if (node == NULL)
        return;

    storeNodesInOrder(node->left, nodes, index);
    nodes[(*index)++] = node;
    storeNodesInOrder(node->right, nodes, index);
}
//Построение древа из массива
phonebook *buildBalancedTree(phonebook **nodes, int start, int end)
{
    if (start > end)
        return NULL;

    int mid = (start + end) / 2;
    phonebook *root = nodes[mid];

    root->left = buildBalancedTree(nodes, start, mid - 1);
    root->right = buildBalancedTree(nodes, mid + 1, end);

    fixheight(root);

    return root;
}
//Перестройка дерева
void rebuildBalancedTree(phonebook **root)
{
    if (*root == NULL)
        return;

    phonebook **nodes = (phonebook **)malloc(currentPosition * sizeof(phonebook *));
    int index = 0;


    storeNodesInOrder(*root, nodes, &index);


    *root = buildBalancedTree(nodes, 0, index - 1);

    free(nodes);
}
*/
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
    /*if (personID < (*node)->person.perID)
    {
        int result = deletePerson(personID, &(*node)->left);
        if (result)
        {
            fixheight(*node);
            *node = balanceTree(*node);
        }
        return result;
    }
    else if (personID > (*node)->key)
    {
        int result = deletePerson(personID, &(*node)->right);
        if (result)
        {
            fixheight(*node);
            *node = balanceTree(*node);
        }
        return result;
    }
    else
    {
        phonebook *temp = *node;
        int result = 1;
        // Есть только правый
        if (((*node)->left == NULL))
        {
            *node = (*node)->right;
            free(temp->person.name);
            free(temp->person.surname);
            free(temp->person.patronym);
            free(temp);
        }
        // Если только левый потомок
        else if ((*node)->right == NULL)
        {
            *node = (*node)->left;
            free(temp->person.name);
            free(temp->person.surname);
            free(temp->person.patronym);
            free(temp);
        }
        // Если оба
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

            deletePerson(minNode->key, &(*node)->right);
        }
        if (*node != NULL)
        {
            fixheight(*node);
            *node = balanceTree(*node);
        }
        return result; */
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

    // Проверка целостности дерева
    // int actualCount = 0;
    // verifyTree(book, &actualCount);

    /*printf("\n=== ALL CONTACTS (currentPosition: %d ===\n",
           currentPosition, actualCount);

    if (actualCount != currentPosition)
    {
        printf("WARNING: Tree integrity issue! Some contacts are missing from tree.\n");
    }
    */
    printf("\n=== ALL CONTACTS (%d) ===\n", currentPosition);
    printf("ID\tSurname\t\tName\t\tPatronym\tPhone\t\tJob\n");
    printf("------------------------------------------------------------------------\n");

    printTree(book);
}

// Вывод древа в виде древа
void printTreeAsTree(phonebook *node, int level)
{
    if (node != NULL)
    {
        printTreeAsTree(node->right, level + 1);
        for (int i = 0; i < level; i++)
            printf("   ");
        printf("%d", node->person.perID);
        printTreeAsTree(node->left, level + 1);
        printf("\n");
    }
}

/*
void balanceTree_ui(phonebook **book)
{
    if (*book == NULL)
    {
        printf("\nСписок пуст!\n");
        return;
    }

    printf("\n=== AVL БАЛАНСИРОВКА ===\n");

    // Проверка перед балансировкой
    printf("Дерево до операций:\n");
    printTreeAsTree(*book, 0);
    verifyAllIDs(*book);

    printf("\nДерево автоматически балансируется при каждой операции\n");
    printf("Текущий баланс-фактор корня: %d\n", getBalance(*book));

    // Покажем также таблицу контактов для проверки
    printf("\nВсе контакты:\n");
    showAllPersons_ui(*book);
}

void verifyTree(phonebook *node, int *count)
{
    if (node == NULL)
        return;

    (*count)++;
    verifyTree(node->left, count);
    verifyTree(node->right, count);
}

// Функция для проверки, что все ID от 0 до currentPosition-1 присутствуют в дереве
void verifyAllIDs(phonebook *node)
{
    printf("Проверка целостности ID в дереве:\n");
    int missing_count = 0;

    for (int i = 0; i < currentPosition; i++)
    {
        phonebook *found = findNode(node, i);
        if (found == NULL)
        {
            printf("  ОШИБКА: ID %d отсутствует в дереве!\n", i);
            missing_count++;
        }
    }

    if (missing_count == 0)
    {
        printf("  Все ID присутствуют в дереве ✓\n");
    }
    else
    {
        printf("  Найдено отсутствующих ID: %d\n", missing_count);
    }
}


*/
