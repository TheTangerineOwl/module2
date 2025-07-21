#include <string.h>
#include "contacts.h"

#include <stdio.h>

Contact_t* addContact(
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
    if (contacts == NULL || (contactsCount + 1 > MAX_CONTACTS_COUNT) ||
        lastName == NULL || firstName == NULL)
        return NULL;
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
            strncpy(contact->emails[i], *email++, MAX_EMAIL_LENGTH - 1);
    }
    if (socials)
    {
        char** social = socials;
        for (size_t j = 0; *social && MAX_SOCIALS_COUNT; j++)
            strncpy(contact->socialsLink[j], *social++, SOCIALS_LINK_LENGTH - 1);
    }

    contactsCount++;
    return contact;
}

int deleteContact(
    Contact_t* contacts,
    int index
)
{
    if (index >= MAX_CONTACTS_COUNT || index >= contactsCount)
        return 0;
    for (size_t i = index; i < (size_t)contactsCount; i++)
        contacts[i] = contacts[i + 1];
    contactsCount--;
    return 1;
}

Contact_t* editContact(
    Contact_t* contact,
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
    if (contact == NULL || lastName == NULL || firstName == NULL)
        return NULL;

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
            strncpy(contact->emails[i], *email++, MAX_EMAIL_LENGTH - 1);
    }
    if (socials)
    {
        char** social = socials;
        for (size_t j = 0; *social && MAX_SOCIALS_COUNT; j++)
            strncpy(contact->socialsLink[j], *social++, SOCIALS_LINK_LENGTH - 1);
    }

    return contact;
}