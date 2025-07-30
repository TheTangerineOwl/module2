#ifndef CONTACT_LIST_IO_H
#define CONTACT_LIST_IO_H

#include "contacts.h"
#include "contactList.h"
#include <stdbool.h>

extern List_t contactList;
extern size_t contactsCount;

typedef struct
{
    const char* name;
    const size_t fieldLength;
    const size_t itemCount;
    const bool canBeNull;
} ContactField;

enum ContactFieldType
{
    LAST_NAME, FIRST_NAME, PATRONIM,
    WORK_PLACE, POSITION,
    WORK_PHONE, PERSONAL_PHONE, HOME_PHONE, EXTRA_PHONE,
    EMAIL, SOCIAL_LINK
};

static const ContactField menuItems[] = {
    {"Фамилия", LAST_NAME_LENGTH, 1, false},
    {"Имя", FIRST_NAME_LENGTH, 1, false},
    {"Отчество", PATRONIM_LENGTH, 1, true},
    {"Место работы", COMPANY_NAME_LENGTH, 1, true},
    {"Должность", POSITION_LENGTH, 1, true},
    {"Рабочий телефон", PHONE_NUMBER_LENGTH, 1, true},
    {"Личный телефон", PHONE_NUMBER_LENGTH, 1, true},
    {"Домашний телефон", PHONE_NUMBER_LENGTH, 1, true},
    {"Дополнительный телефон", PHONE_NUMBER_LENGTH, 1, true},
    {"Электронная почта", MAX_EMAIL_LENGTH, MAX_EMAILS_COUNT, true},
    {"Ссылка на соцсеть", SOCIALS_LINK_LENGTH, MAX_SOCIALS_COUNT, true}
};

static const size_t menuCount = sizeof(menuItems) / sizeof(ContactField);

char* enterStrField(char* field, size_t fieldLength);
void printContact(const Contact_t contact);
void printAllContacts(const List_t list);
void printShortContacts(const List_t list);
Item_t* enterContactInfo(List_t* list);
char* changeField(char* oldField, const ContactField fieldAttr);
Item_t* editContactInfo(List_t* list, Item_t *contact);
Item_t* chooseContact(const List_t list);

#endif