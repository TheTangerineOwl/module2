#include "contacts.h"

Contact_t* clearContact(Contact_t* contact)
{
    if (!contact)
        return NULL;
    if (contact->lastName) free(contact->lastName);
    if (contact->firstName) free(contact->firstName);
    if (contact->patronim) free(contact->patronim);
    if (contact->workInfo)
    {
        if (contact->workInfo->workPlace) free(contact->workInfo->workPlace);
        if (contact->workInfo->position) free(contact->workInfo->position);
        free(contact->workInfo);
    }
    if (contact->numbers)
    {
        if (contact->numbers->work) free(contact->numbers->work);
        if (contact->numbers->personal) free(contact->numbers->personal);
        if (contact->numbers->home) free(contact->numbers->home);
        if (contact->numbers->extra) free(contact->numbers->extra);
        free(contact->numbers);
    }
    if (contact->emails)
    {
        for (size_t i = 0; contact->emails[i]; i++)
            free(contact->emails[i]);
        free(contact->emails);
        contact->emailCount = 0;
    }
    if (contact->socialsLink)
    {
        for (size_t j = 0; contact->socialsLink[j]; j++)
            free(contact->socialsLink[j]);
        free(contact->socialsLink);
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
    if (!lastName || !firstName)
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
        NULL, NULL
    ))
        return NULL;

    if (emails)
    {
        if (contact->emails)
            while (contactDeleteEmail(contact, 0));
        for (size_t i = 0; emails[i] && i < MAX_EMAILS_COUNT; i++)
            contactAddEmail(contact, emails[i]);
    }
    if (socials)
    {
        if (contact->socialsLink)
            while (contactDeleteSocial(contact, 0));
        for (size_t i = 0; socials[i] && i < MAX_SOCIALS_COUNT; i++)
            contactAddSocials(contact, socials[i]);
    }

    return contact;
}

Contact_t* contactAddEmail(Contact_t* contact, const char* email)
{
    if (!contact || !email || contact->emailCount >= MAX_EMAILS_COUNT)
        return contact;

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

Contact_t* contactAddSocials(Contact_t* contact, const char* socialLink)
{
    if (!contact || !socialLink || contact->socialsCount >= MAX_SOCIALS_COUNT)
        return contact;

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

    if (emails)
        for (size_t i = 0; emails[i] && i < MAX_EMAILS_COUNT; i++)
        {
            if (!contactAddEmail(contact, emails[i]))
                printf("Не удалось добавить почту %lu (%s)!", i, emails[i]);
        }
    if (socials)
        for (size_t i = 0; socials[i] && i < MAX_SOCIALS_COUNT; i++)
        {
            if (!contactAddSocials(contact, socials[i]))
                printf("Не удалось добавить ссылку на соцсеть %lu (%s)!", i, socials[i]);
        }

    return contact;
}