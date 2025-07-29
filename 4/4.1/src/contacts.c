#include "contacts.h"

// extern size_t contactsCount;

int compareContacts(const Contact_t contact1, const Contact_t contact2)
{
    //int res = strcmp(contact1.lastName, contact2.lastName);
    int res = strcoll(contact1.lastName, contact2.lastName);
    if (res)
        return res;
    //res = strcmp(contact1.firstName, contact2.firstName);
    res = strcoll(contact1.firstName, contact2.firstName);
    return res;
}

#define CLEAR_ALLC(field) \
    if (field) \
    { \
        free(field); \
        field = NULL; \
    }

Contact_t* clearContact(Contact_t* contact)
{
    if (!contact)
        return NULL;
    CLEAR_ALLC(contact->lastName);
    CLEAR_ALLC(contact->firstName);
    CLEAR_ALLC(contact->patronim);
    if (contact->workInfo)
    {
        CLEAR_ALLC(contact->workInfo->workPlace);
        CLEAR_ALLC(contact->workInfo->position);
        CLEAR_ALLC(contact->workInfo);
    }
    if (contact->numbers)
    {
        CLEAR_ALLC(contact->numbers->work);
        CLEAR_ALLC(contact->numbers->personal);
        CLEAR_ALLC(contact->numbers->home);
        CLEAR_ALLC(contact->numbers->extra);
        CLEAR_ALLC(contact->numbers);
    }
    if (contact->emails)
    {
        for (size_t i = 0; i < contact->emailCount; i++)
            contact = contactDeleteEmail(contact, 0);
            // free(contact->emails[i]);
        CLEAR_ALLC(contact->emails);
        contact->emailCount = 0;
    }
    if (contact->socialsLink)
    {
        for (size_t i = 0; i < contact->socialsCount; i++)
            contact = contactDeleteSocial(contact, 0);
        CLEAR_ALLC(contact->socialsLink);
        contact->socialsCount = 0;
    }
    return contact;
}

Contact_t* newContact(
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
    //if (contactsCount + 1 > MAX_CONTACTS_COUNT || !lastName || !firstName)
    if (!lastName || !firstName)
        return NULL;
    if (strlen(lastName) < 1 || strlen(firstName) < 1)
        return NULL;
    Contact_t* contact = (Contact_t*)malloc(sizeof(Contact_t));
    if (!contact)
        return NULL;

    if (!copyToContact(contact,
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emails, socials
    ))
    {
        clearContact(contact);
        free(contact);
        return NULL;
    }
    // contactsCount++;
    return contact;
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
    if (!contact)
        return NULL;

    if (!copyToContact(contact,
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emails, socials
    ))
        return NULL;

    // if (emails)
    // {
    //     if (contact->emails)
    //     {
    //         size_t j = 0;
    //         for (; j < contact->emailCount; j++)
    //             if (emails[j])
    //                 contactEditEmail(contact, j, emails[j]);
    //         for (; emails[j] && j < MAX_EMAILS_COUNT; j++)
    //             contactAddEmail(contact, emails[j]);
    //     }
    //     else
    //         for (size_t i = 0; emails[i] && i < MAX_EMAILS_COUNT; i++)
    //             contactAddEmail(contact, emails[i]);
    // }
    // if (socials)
    // {
    //     if (contact->socialsLink)
    //     {
    //         size_t j = 0;
    //         for (; j < contact->socialsCount; j++)
    //             if (socials[j])
    //                 contactEditSocial(contact, j, socials[j]);
    //         for (; socials[j] && j < MAX_SOCIALS_COUNT; j++)
    //             contactAddSocial(contact, socials[j]);
    //     }
    //     else
    //         for (size_t i = 0; socials[i] && i < MAX_EMAILS_COUNT; i++)
    //             contactAddSocial(contact, emails[i]);
    // }

    return contact;
}

Contact_t* contactAddEmail(Contact_t* contact, const char* email)
{
    if (!contact || !email || contact->emailCount >= MAX_EMAILS_COUNT)
        return NULL;

    size_t length = strlen(email);
    if (length >= MAX_EMAIL_LENGTH)
        length = MAX_EMAIL_LENGTH - 1;

    size_t newCount = contact->emailCount + 1;
    char** newEmails = (char**)realloc(contact->emails, sizeof(char*) * newCount);
    if (!newEmails)
        return NULL;

    contact->emails = newEmails;
    contact->emails[contact->emailCount] = (char*)malloc((length + 1) * sizeof(char));
    if (!contact->emails[contact->emailCount])
        return NULL;

    strncpy(contact->emails[contact->emailCount], email, length);
    contact->emails[contact->emailCount][length] = '\0';
    contact->emailCount++;

    return contact;
}

Contact_t* contactAddSocial(Contact_t* contact, const char* socialLink)
{
    if (!contact || !socialLink || contact->socialsCount >= MAX_SOCIALS_COUNT)
        return NULL;

    size_t length = strlen(socialLink);
    if (length >= SOCIALS_LINK_LENGTH)
        length = SOCIALS_LINK_LENGTH - 1;

    size_t newCount = contact->socialsCount + 1;
    char** newSocials = (char**)realloc(contact->socialsLink, sizeof(char*) * newCount);
    if (!newSocials)
        return NULL;

    contact->socialsLink = newSocials;
    contact->socialsLink[contact->socialsCount] = (char*)malloc((length + 1) * sizeof(char));
    if (!contact->socialsLink[contact->socialsCount])
        return NULL;

    strncpy(contact->socialsLink[contact->socialsCount], socialLink, length);
    contact->socialsLink[contact->socialsCount][length] = '\0';
    contact->socialsCount++;

    return contact;
}

Contact_t* contactEditEmail(Contact_t* contact, const size_t index, const char* newValue)
{
    if (!contact || !newValue || !contact->emails || index >= contact->emailCount)
        return NULL;
    size_t size = strlen(newValue);
    if (size >= MAX_EMAIL_LENGTH)
        size = MAX_EMAIL_LENGTH - 1;
    char * newEmail = (char*)realloc(contact->emails[index], (size + 1) * sizeof(char));
    if (!newEmail)
        return NULL;
    contact->emails[index] = newEmail;

    strncpy(contact->emails[index], newValue, size);
    contact->emails[index][size] = '\0';

    return contact;
}

Contact_t* contactEditSocial(Contact_t* contact, const size_t index, const char* newValue)
{
    if (!contact || !newValue || !contact->socialsLink || index >= contact->socialsCount)
        return NULL;
    size_t size = strlen(newValue);
    if (size >= SOCIALS_LINK_LENGTH)
        size = SOCIALS_LINK_LENGTH - 1;
    char * newSocial = (char*)realloc(contact->socialsLink[index], (size + 1) * sizeof(char));
    if (!newSocial)
        return NULL;
    contact->socialsLink[index] = newSocial;

    strncpy(contact->socialsLink[index], newValue, size);
    contact->socialsLink[index][size] = '\0';

    return contact;
}

Contact_t* contactDeleteEmail(Contact_t* contact, const size_t index)
{
    if (!contact || !contact->emails || index >= contact->emailCount)
        return NULL;
    if (!contact->emails[index])
        return NULL;
    free(contact->emails[index]);
    for (size_t i = index; i < contact->emailCount; i++)
        contact->emails[i] = contact->emails[i + 1];
    contact->emailCount--;
    return contact;
}

Contact_t* contactDeleteSocial(Contact_t* contact, const size_t index)
{
    if (!contact || !contact->socialsLink || index >= contact->socialsCount)
        return NULL;
    if (!contact->socialsLink[index])
        return NULL;
    free(contact->socialsLink[index]);
    for (size_t i = index; i < contact->socialsCount; i++)
        contact->socialsLink[i] = contact->socialsLink[i + 1];
    contact->socialsCount--;
    return contact;
}

static char* changeField(char* fieldPtr, const char* newValue, size_t maxSize)
{
    size_t size;
    if (newValue)
    {
        size = strlen(newValue) + 1;
        if (size >= maxSize)
            size = maxSize;
        fieldPtr = (char *)realloc(fieldPtr, size * sizeof(char));
        if (!fieldPtr)
            return NULL;
        strncpy(fieldPtr, newValue, size);
        fieldPtr[size - 1] = '\0';
    }
    return fieldPtr;
}

Contact_t* copyToContact(
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
    if (!contact)
        return NULL;

    if (
        (lastName && strlen(lastName) < 1) ||
        (firstName && strlen(firstName) < 1)
    )
        return NULL;
    char* buf = NULL;

    if (!(buf = changeField(contact->lastName, lastName, LAST_NAME_LENGTH)))
        return NULL;
    contact->lastName = buf;
    if (!(buf = changeField(contact->firstName, firstName, FIRST_NAME_LENGTH)))
        return NULL;
    contact->firstName = buf;
    if (!(buf = changeField(contact->patronim, patronim, PATRONIM_LENGTH)))
        return NULL;
    contact->patronim = buf;

    if (workPlace || position)
    {
        if (!contact->workInfo)
            contact->workInfo = (Work_t*)calloc(1, sizeof(Work_t));
        if (!(buf = changeField(contact->workInfo->workPlace, workPlace, COMPANY_NAME_LENGTH)))
            return NULL;
        contact->workInfo->workPlace = buf;
        if (!(buf = changeField(contact->workInfo->position, position, POSITION_LENGTH)))
            return NULL;
        contact->workInfo->position = buf;
    }

    if (workPhone || personalPhone || homePhone || extraPhone)
    {
        if (!contact->numbers)
            contact->numbers = (PhoneNumbers_t*)calloc(1, sizeof(PhoneNumbers_t));
        if (!(buf = changeField(contact->numbers->work, workPhone, PHONE_NUMBER_LENGTH)))
            return NULL;
        contact->numbers->work = buf;
        if (!(buf = changeField(contact->numbers->personal, personalPhone, PHONE_NUMBER_LENGTH)))
            return NULL;
        contact->numbers->personal = buf;
        if (!(buf = changeField(contact->numbers->home, homePhone, PHONE_NUMBER_LENGTH)))
            return NULL;
        contact->numbers->home = buf;
        if (!(buf = changeField(contact->numbers->extra, extraPhone, PHONE_NUMBER_LENGTH)))
            return NULL;
        contact->numbers->extra = buf;
    }

    // if (emails)
    //     for (size_t i = 0; emails[i] && i < MAX_EMAILS_COUNT; i++)
    //     {
    //         if (!contactAddEmail(contact, emails[i]))
    //             printf("Не удалось добавить почту %lu (%s)!", i, emails[i]);
    //     }
    // if (socials)
    //     for (size_t i = 0; socials[i] && i < MAX_SOCIALS_COUNT; i++)
    //     {
    //         if (!contactAddSocial(contact, socials[i]))
    //             printf("Не удалось добавить ссылку на соцсеть %lu (%s)!", i, socials[i]);
    //     }

    if (emails)
    {
        if (contact->emails)
        {
            size_t j = 0;
            for (; j < contact->emailCount; j++)
                if (emails[j])
                    contactEditEmail(contact, j, emails[j]);
            for (; emails[j] && j < MAX_EMAILS_COUNT; j++)
                contactAddEmail(contact, emails[j]);
        }
        else
            for (size_t i = 0; emails[i] && i < MAX_EMAILS_COUNT; i++)
                contactAddEmail(contact, emails[i]);
    }
    if (socials)
    {
        if (contact->socialsLink)
        {
            size_t j = 0;
            for (; j < contact->socialsCount; j++)
                if (socials[j])
                    contactEditSocial(contact, j, socials[j]);
            for (; socials[j] && j < MAX_SOCIALS_COUNT; j++)
                contactAddSocial(contact, socials[j]);
        }
        else
            for (size_t i = 0; socials[i] && i < MAX_SOCIALS_COUNT; i++)
                contactAddSocial(contact, socials[i]);
    }

    return contact;
}