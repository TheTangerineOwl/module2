#include <stdio.h>
#include <string.h>
#include "contacts.h"

int contactsCount = 0;

void printContact(const Contact_t contact)
{
    printf("Фамилия: %s\n", contact.lastName);
    printf("Имя: %s\n", contact.firstName);
    if (contact.patronim[0] != 0) printf("Отчество: %s\n", contact.patronim);
    if (contact.numbers.work[0] != 0) printf("Рабочий телефон: %s\n", contact.numbers.work);
    if (contact.numbers.personal[0] != 0) printf("Личный телефон: %s\n", contact.numbers.personal);
    if (contact.numbers.home[0] != 0) printf("Домашний телефон: %s\n", contact.numbers.home);
    if (contact.numbers.extra[0] != 0) printf("Дополнительный телефон: %s\n", contact.numbers.extra);
    if (strlen(contact.emails[0]) > 0)
    {
        printf("Электронные почты: \n");
        for (size_t i = 0; i < MAX_EMAILS_COUNT && strlen(contact.emails[i]) > 0; i++)
        {
            printf("%ld. %s\n", i + 1, contact.emails[i]);
        }
    }
    if (strlen(contact.socialsLink[0]) > 0)
    {
        printf("Социальные сети и мессенджеры: \n");
        for (size_t j = 0; j < MAX_EMAILS_COUNT && strlen(contact.socialsLink[j]) > 0; j++)
        {
            printf("%ld. %s\n", j + 1, contact.socialsLink[j]);
        }
    }
}

void printAllContacts(const Contact_t* contacts)
{
    for (size_t i = 0; i < contactsCount; i++)
    {
        printf("%ld. ", i + 1);
        printContact(contacts[i]);
    }
}

int enterContactInfo(Contact_t* contacts)
{
    if (contactsCount + 1 > MAX_CONTACTS_COUNT)
    {
        printf("Нельзя добавить контакт!\n");
        return 0;
    }
    char lastName[NAME_PERSON_LENGTH];
    char firstName[NAME_PERSON_LENGTH];
    char patronim[NAME_PERSON_LENGTH];
    char work[PHONE_NUMBER_LENGTH];
    char personal[PHONE_NUMBER_LENGTH];
    char home[PHONE_NUMBER_LENGTH];
    char extra[PHONE_NUMBER_LENGTH];
    char workPlace[NAME_COMPANY_LENGTH];
    char position[NAME_COMPANY_LENGTH];
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH];
    char socialsLink[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH];
    printf("Введите фамилию: ");
    // while (!scanf("%s", lastName) || strlen(lastName) == 0)
    while (!fgets(lastName, NAME_PERSON_LENGTH, stdin) || strlen(lastName) == 0)
        printf("Некорректный ввод! Попробуйте еще раз: ");
    printf("Введите имя: ");
    while (!fgets(firstName, NAME_PERSON_LENGTH, stdin) || strlen(firstName) == 0)
    // while (!scanf("%s", firstName) || strlen(firstName) == 0)
        printf("Некорректный ввод! Попробуйте еще раз: ");
    printf("%s\n", lastName);
    printf("%s\n", firstName);
    return 1;
}

int main()
{
    Contact_t contacts[MAX_CONTACTS_COUNT];
    enterContactInfo(contacts);
    strncpy(contacts[0].lastName, "lastName", NAME_PERSON_LENGTH);
    strncpy(contacts[0].firstName, "firstName", NAME_PERSON_LENGTH);
    strncpy(contacts[0].patronim, "patronim", NAME_PERSON_LENGTH);
    strncpy(contacts[0].workInfo.workPlace, "workPlace", NAME_COMPANY_LENGTH);
    strncpy(contacts[0].workInfo.position, "position", NAME_COMPANY_LENGTH);
    strncpy(contacts[0].numbers.work, "workPhone", PHONE_NUMBER_LENGTH);
    strncpy(contacts[0].numbers.personal, "persPhone", PHONE_NUMBER_LENGTH);
    strncpy(contacts[0].numbers.home, "homePhone", PHONE_NUMBER_LENGTH);
    strncpy(contacts[0].numbers.extra, "extraPhone", PHONE_NUMBER_LENGTH);
    if (contacts[0].emails)
    {
        //char** email = contacts[0].emails;
        for (size_t i = 0; i < MAX_EMAILS_COUNT - 3; i++)
        {
            strncpy(contacts[0].emails[i], "1", MAX_EMAIL_LENGTH);
            // *email++;
        }
    }
    if (contacts[0].socialsLink)
    {
        //char** social = contacts[0].socialsLink;
        for (size_t j = 0; j < MAX_SOCIALS_COUNT - 3; j++)
        {
            strncpy(contacts[0].socialsLink[j], "2", SOCIALS_LINK_LENGTH);
        }
    }

    contactsCount++;
    char* emails[MAX_EMAIL_LENGTH] = { "wavydavy@foodgram.ru", "isonfire@mail.ru" };
    char* socials[SOCIALS_LINK_LENGTH] = { "tg.com/l0", "vk.ru/id0", "contact.ru/contact" };
    Contact_t* res = addContact(contacts,
        "Wavy", "Davy",
        "Isonfire", "APAC", "soldier",
        "88005555335",
        "88889993333",
        "123445", "89993334444",
        emails,
        socials
    );
    printf("Все контакты: \n");
    printAllContacts(contacts);
    printf("Редактирование: \n");
    editContact(
        &contacts[1],
        "Wavvyyyy", "Davyyy",
        "Isonfireee", NULL,
        NULL, NULL,
        NULL, NULL, NULL,
        NULL, NULL
    );
    printAllContacts(contacts);
    deleteContact(contacts, 0);
    printf("После удаления: \n");
    printAllContacts(contacts);

    return 0;
}