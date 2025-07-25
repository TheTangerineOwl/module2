#include "contacts.h"

int contactsCount = 0;

int main()
{
    Contact_t contacts[MAX_CONTACTS_COUNT];
    // enterContactInfo(contacts);
    // char poof[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH] = {""};
    // char spoof[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH] = {""};
    // addContact(
    //     contacts,
    //     "lastName", "firstName", "patronim",
    //     "workPlace", "position",
    //     "83334445555", "89994449999",
    //     "123456", "87776665555",
    //     poof, spoof
    // );
    // char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH] = { "wavydavy@foodgram.ru", "isonfire@mail.ru" };
    // char socials[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH] = { "tg.com/l0", "vk.ru/id0", "contact.ru/contact" };
    // Contact_t* res = addContact(contacts,
    //     "Wavy", "Davy",
    //     "Isonfire", "APAC", "soldier",
    //     "88005555335",
    //     "88889993333",
    //     "123445", "89993334444",
    //     emails,
    //     socials
    // );
    // printf("Все контакты: \n");
    // printAllContacts(contacts);
    // printf("Редактирование: \n");
    // // editContact(
    // //     &contacts[1],
    // //     "Wavvyyyy", "Davyyy",
    // //     "Isonfireee", "",
    // //     "", "",
    // //     "", "", "",
    // //     poof, spoof
    // // );
    // printAllContacts(contacts);
    // deleteContact(contacts, 0);
    // printf("После удаления: \n");
    // printAllContacts(contacts);

    int choice, exitFlag = 0;
    Contact_t* currentContact;

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

        while (!scanf("%d", &choice) || choice < 1 || choice > 6)
        {
            puts("Некорректный выбор! Попробуйте еще раз.\n");
            while ((c = getchar() != '\n') && c != EOF);
        }
        while ((c = getchar()) != '\n' && c != EOF);

        switch (choice)
        {
        case 1:
            enterContactInfo(contacts);
            break;
        case 2:
        {
            currentContact = chooseContact(contacts);
            if (!currentContact)
            {
                puts("Ошибка выбора контакта!");
                continue;
            }
            editContactInfo(contacts);
        }
        break;
        case 3:
        {
            if (!contactsCount)
            {
                puts("Нет контактов для удаления!");
                continue;
            }
            puts("Все контакты:");
            printShortContacts(contacts);
            printf("Выберите индекс контакта для удаления: ");
            int choice;
            while (!scanf("%d", &choice) || choice < 1 || choice > contactsCount)
            {
                printf("Некорректный выбор! Попробуйте еще раз.\n");
                while ((c = getchar() != '\n') && c != EOF);
            }
            while ((c = getchar()) != '\n' && c != EOF);
            deleteContact(contacts, choice - 1);
        }
        break;
        case 4:
        {
            currentContact = chooseContact(contacts);
            if (!currentContact)
            {
                puts("Ошибка выбора контакта!");
                continue;
            }
            printContact(*currentContact);
        }
        break;
        case 5:
            printShortContacts(contacts);
            break;
        case 6:
            exitFlag = 1;
            break;
        default:
            printf("Некорректный выбор! Попробуйте еще раз.\n");
            break;
        }
    }

    return 0;
}