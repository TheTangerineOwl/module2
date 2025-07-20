#include <string.h>
#include "contacts.h"

#include <stdio.h>

/*
Программа
должна предоставлять возможность добавления, редактирования и удаления
контакта.
*/

// contactsCount = 0;

int addContact(
    Contact_t* contacts,
    const char* lastName,
    const char* firstName,
    const char* patronim,
    const char* workPlace,
    const char* position,
    const char* workPhone,
    const char* personalPhone,
    const char* homePhone,
    const char* extraPhone,
    char** emails,
    char** socials    
)
{
    if (contacts == NULL)
        return 1;
    if (contactsCount + 1 > MAX_CONTACTS_COUNT)
        return 2;
    if (lastName == NULL)
        return 3;
    if (firstName == NULL)
        return 4;
    Contact_t* contact = &contacts[contactsCount];

    strncpy(contact->lastName, lastName, NAME_PERSON_LENGTH - 1);
    strncpy(contact->firstName, firstName, NAME_PERSON_LENGTH - 1);
    if (patronim) strncpy(contact->patronim, patronim, NAME_PERSON_LENGTH - 1);
    if (workPlace) strncpy(contact->workInfo.workPlace, workPlace, NAME_COMPANY_LENGTH) - 1;
    if (position) strncpy(contact->workInfo.position, position, NAME_COMPANY_LENGTH - 1);
    if (workPhone) strncpy(contact->numbers.work, workPhone, PHONE_NUMBER_LENGTH - 1);
    if (personalPhone) strncpy(contact->numbers.personal, personalPhone, PHONE_NUMBER_LENGTH - 1);
    if (homePhone) strncpy(contact->numbers.home, homePhone, PHONE_NUMBER_LENGTH - 1);
    if (extraPhone) strncpy(contact->numbers.extra, extraPhone, PHONE_NUMBER_LENGTH - 1);
    if (emails)
    {
        char** email = emails;
        for (size_t i = 0; *email && i < MAX_EMAILS_COUNT; i++)
        {
            strncpy(contact->emails[i], *email++, MAX_EMAIL_LENGTH - 1);
            // *email++;
        }
    }
    if (socials)
    {
        char** social = socials;
        for (size_t j = 0; *social && MAX_SOCIALS_COUNT; j++)
        {
            strncpy(contact->socialsLink[j], *social++, SOCIALS_LINK_LENGTH - 1);
        }
    }

    contactsCount++;
    return 0;
}

int deleteContact(
    Contact_t* contacts,
    int index
)
{
    if (index >= MAX_CONTACTS_COUNT)
        return 1;
    if (index >= contactsCount)
        return 0;
    for (size_t i = index; i < (size_t)contactsCount; i++)
    {
        contacts[i] = contacts[i + 1];
    }
    contactsCount--;
    return 0;
}

// int editContact(
//     Contact_t* contact,
//     const char* lastName,
//     const char* firstName,
//     const char* patronim,
//     const char* workPlace,
//     const char* position,
//     const char* workPhone,
//     const char* personalPhone,
//     const char* homePhone,
//     const char* extraPhone,
//     char** emails,
//     char** socials
// )
// {
//     if (!contact)
//         return -1;
    
// }