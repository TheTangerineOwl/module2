#ifndef CONTACT_LIST_IO_H
#define CONTACT_LIST_IO_H

#include "contacts.h"
#include "contactList.h"

extern List_t contactList;
extern size_t contactsCount;

typedef struct
{
    const char* name;
    const size_t fieldLength;
    const size_t itemCount;
} ContactField;

enum ContactFieldType
{
    LAST_NAME, FIRST_NAME, PATRONIM,
    WORK_PLACE, POSITION,
    WORK_PHONE, PERSONAL_PHONE, HOME_PHONE, EXTRA_PHONE,
    EMAIL, SOCIAL_LINK
};

static const ContactField menuItems[] = {
    {"Фамилия", LAST_NAME_LENGTH, 1},
    {"Имя", FIRST_NAME_LENGTH, 1},
    {"Отчество", PATRONIM_LENGTH, 1},
    {"Место работы", COMPANY_NAME_LENGTH, 1},
    {"Должность", POSITION_LENGTH, 1},
    {"Рабочий телефон", PHONE_NUMBER_LENGTH, 1},
    {"Личный телефон", PHONE_NUMBER_LENGTH, 1},
    {"Домашний телефон", PHONE_NUMBER_LENGTH, 1},
    {"Дополнительный телефон", PHONE_NUMBER_LENGTH, 1},
    {"Электронная почта", MAX_EMAIL_LENGTH, MAX_EMAILS_COUNT},
    {"Ссылка на соцсеть", SOCIALS_LINK_LENGTH, MAX_SOCIALS_COUNT}
};

static const size_t menuCount = sizeof(menuItems) / sizeof(ContactField);

char* enterStrField(char* field, size_t fieldLength);
void printContact(const Contact_t contact);
void printAllContacts(const Contact_t* contacts);
int enterContactInfo(Contact_t* contacts);
int editContactInfo(Contact_t *contact);
char* changeField(char* oldField, const ContactField field);
void printShortContacts(const Contact_t* contacts);
Contact_t* chooseContact(Contact_t* contacts);

#endif