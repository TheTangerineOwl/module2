#include <stdio.h>
#include "contacts.h"

#define CLEAR_BUFFER() while (getchar() != '\n')
#define PRINT_FIELD_IFNZERO(field, fieldName) \
    if (field && field[0] != 0) printf("%s: %s\n", fieldName, field)

int contactsCount = 0;

int enterStrField(char* field, size_t fieldLength)
{
    if (!fgets(field, fieldLength, stdin))
    {
        puts("Не удалось прочитать строку из потока ввода!");
        CLEAR_BUFFER();
        return 0;
    }
    field[strcspn(field, "\n")] = '\0';
    if (strlen(field) == fieldLength - 1)
        CLEAR_BUFFER();
    return 1;
}

void printContact(const Contact_t contact)
{
    printf("Фамилия: %s\n", contact.lastName);
    printf("Имя: %s\n", contact.firstName);
    PRINT_FIELD_IFNZERO(contact.patronim, "Отчество");
    PRINT_FIELD_IFNZERO(contact.workInfo.workPlace, "Место работы");
    PRINT_FIELD_IFNZERO(contact.workInfo.position, "Должность");
    PRINT_FIELD_IFNZERO(contact.numbers.work, "Рабочий телефон");
    PRINT_FIELD_IFNZERO(contact.numbers.personal, "Личный телефон");
    PRINT_FIELD_IFNZERO(contact.numbers.home, "Домашний телефон");
    PRINT_FIELD_IFNZERO(contact.numbers.extra, "Дополнительный телефон");
    if (strlen(contact.emails[0]) > 0)
    {
        printf("Электронные почты: \n");
        for (size_t i = 0; i < MAX_EMAILS_COUNT && strlen(contact.emails[i]) > 0; i++)
            printf("%ld. %s\n", i + 1, contact.emails[i]);
    }
    if (strlen(contact.socialsLink[0]) > 0)
    {
        printf("Социальные сети и мессенджеры: \n");
        for (size_t j = 0; j < MAX_EMAILS_COUNT && strlen(contact.socialsLink[j]) > 0; j++)
            printf("%ld. %s\n", j + 1, contact.socialsLink[j]);
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
    char lastName[LAST_NAME_LENGTH];
    char firstName[FIRST_NAME_LENGTH];
    char patronim[PATRONIM_LENGTH];
    char work[PHONE_NUMBER_LENGTH];
    char personal[PHONE_NUMBER_LENGTH];
    char home[PHONE_NUMBER_LENGTH];
    char extra[PHONE_NUMBER_LENGTH];
    char workPlace[COMPANY_NAME_LENGTH];
    char position[POSITION_LENGTH];
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH];
    char socialsLink[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH];

    printf("Введите фамилию: ");
    enterStrField(lastName, LAST_NAME_LENGTH);
    while (strlen(lastName) == 0)
    {
        puts("Поле \"фамилия\" должно быть заполнено!");
        enterStrField(lastName, LAST_NAME_LENGTH);
    }
    printf("Введите имя: ");
    enterStrField(firstName, FIRST_NAME_LENGTH);
    while (strlen(firstName) == 0)
    {
        puts("Поле \"имя\" должно быть заполнено!");
        enterStrField(lastName, LAST_NAME_LENGTH);
    }

    printf("Введите отчество (пустая строка для пропуска): ");
    enterStrField(patronim, PATRONIM_LENGTH);
    printf("Введите место работы (пустая строка для пропуска): "); 
    enterStrField(workPlace, COMPANY_NAME_LENGTH);
    printf("Введите должность (пустая строка для пропуска): ");
    enterStrField(position, POSITION_LENGTH);
    printf("Введите рабочий телефон (пустая строка для пропуска): ");
    enterStrField(work, PHONE_NUMBER_LENGTH);
    printf("Введите личный телефон (пустая строка для пропуска): ");
    enterStrField(personal, PHONE_NUMBER_LENGTH);
    printf("Введите домашний телефон (пустая строка для пропуска): ");
    enterStrField(home, PHONE_NUMBER_LENGTH);
    printf("Введите дополнительный телефон (пустая строка для пропуска): ");
    enterStrField(extra, PHONE_NUMBER_LENGTH);
    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
    {
        printf("Введите электронную почту %ld (пустая строка для пропуска): ", i + 1);
        enterStrField(emails[i], MAX_EMAIL_LENGTH);
        if (strlen(emails[i]) == 0)
            break;
    }
    for (size_t i = 0; i < MAX_SOCIALS_COUNT; i++)
    {
        printf("Введите ссылку на соцсети или профиль %ld (пустая строка для пропуска): ", i + 1);
        enterStrField(socialsLink[i], SOCIALS_LINK_LENGTH);
        if (strlen(socialsLink[i]) == 0)
            break;
    }

    Contact_t* contact = addContact(contacts,
        lastName, firstName, patronim,
        workPlace, position,
        work, personal, home, extra,
        emails, socialsLink
    );
    printContact(*contact);
    return 1;
}

int main()
{
    Contact_t contacts[MAX_CONTACTS_COUNT];
    enterContactInfo(contacts);
    char poof[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH] = {""};
    char spoof[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH] = {""};
    addContact(
        contacts,
        "lastName", "firstName", "patronim",
        "workPlace", "position",
        "83334445555", "89994449999",
        "123456", "87776665555",
        poof, spoof
    );
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH] = { "wavydavy@foodgram.ru", "isonfire@mail.ru" };
    char socials[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH] = { "tg.com/l0", "vk.ru/id0", "contact.ru/contact" };
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
        "Isonfireee", "",
        "", "",
        "", "", "",
        poof, spoof
    );
    printAllContacts(contacts);
    deleteContact(contacts, 0);
    printf("После удаления: \n");
    printAllContacts(contacts);

    return 0;
}