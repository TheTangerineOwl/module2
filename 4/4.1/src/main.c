#include "contactListIO.h"

List_t contactList;
// size_t contactsCount = 0;

int main(void)
{
    listInit(&contactList);

    size_t choice;
    int exitFlag = 0;
    Item_t* currentContact;

    char c;

    while (!exitFlag)
    {
        puts("\nВыберите опцию:");
        puts("1. Добавление контакта;");
        puts("2. Редактирование контакта;");
        puts("3. Удаление контакта;");
        puts("4. Вывод информации о контакте;");
        puts("5. Вывести все контакты;");
        printf("6. Выход. Ваш выбор: ");

        while (!scanf("%ld", &choice) || choice < 1 || choice > 6)
        {
            puts("Некорректный выбор! Попробуйте еще раз.\n");
            while ((c = getchar() != '\n') && c != EOF);
        }
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice)
        {
        case 1:
            enterContactInfo(&contactList);
            break;
        case 2:
        {
            currentContact = chooseContact(contactList);
            if (!currentContact)
            {
                puts("Ошибка выбора контакта!");
                continue;
            }
            editContactInfo(&contactList, currentContact);
        }
        break;
        case 3:
        {
            if (!contactList.length)
            {
                puts("Нет контактов для удаления!");
                continue;
            }
            puts("Все контакты:");
            printShortContacts(contactList);
            printf("Выберите индекс контакта для удаления: ");
            size_t choice;
            while (!scanf("%ld", &choice) || choice < 1 || choice > contactList.length)
            {
                printf("Некорректный выбор! Попробуйте еще раз.\n");
                while ((c = getchar() != '\n') && c != EOF);
            }
            while ((c = getchar()) != '\n' && c != EOF);
            Contact_t* removed = listRemoveAt(&contactList, choice - 1);
            if (removed)
            {
                clearContact(removed);
                free(removed);
            }
        }
        break;
        case 4:
        {
            currentContact = chooseContact(contactList);
            if (!currentContact)
            {
                puts("Ошибка выбора контакта!");
                continue;
            }
            printContact(*currentContact->contact);
        }
        break;
        case 5:
            printShortContacts(contactList);
            break;
        case 6:
            exitFlag = 1;
            break;
        default:
            printf("Некорректный выбор! Попробуйте еще раз.\n");
            break;
        }
    }

    listClear(&contactList);

    return 0;
}