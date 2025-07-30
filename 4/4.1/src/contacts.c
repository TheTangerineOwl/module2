#include "contacts.h"

#define CHANGE_IF_GIVEN(buffer, fieldName, fieldPtr, maxSize, newValue) \
    if (newValue) { \
        buf = NULL; \
        if (fieldPtr) fieldPtr = NULL; \
        if (!(buf = changeField(fieldPtr, newValue, maxSize))) \
            RETURN_NULL_WITH_MSG("copyToContact:can't change field '%s'\n", fieldName); \
        fieldPtr = buf; \
    }

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
        RETURN_NULL_WITH_MSG("clearContact: got parameter 'contact' == NULL\n");
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
        // for (size_t i = 0; i < contact->emailCount; i++)
        //     contact = contactDeleteEmail(contact, 0);
        while (contactDeleteEmail(contact, 0));
        CLEAR_ALLC(contact->emails);
        contact->emailCount = 0;
    }
    if (contact->socialsLink)
    {
        // for (size_t i = 0; i < contact->socialsCount; i++)
        //     contact = contactDeleteSocial(contact, 0);
        while (contactDeleteSocial(contact, 0));
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
    const size_t emailCount,
    char** emails,
    const size_t socialsCount,
    char** socials 
)
{
    if (!lastName || !firstName)
        RETURN_NULL_WITH_MSG("newContact:'lastName' or 'firstName' was NULL\n");
    if (strlen(lastName) < 1 || strlen(firstName) < 1)
        RETURN_NULL_WITH_MSG("newContact:'lastName' or 'firstName' was empty\n");
    Contact_t* contact = (Contact_t*)malloc(sizeof(Contact_t));
    if (!contact)
        RETURN_NULL_WITH_MSG("newContact:'contact' was NULL\n");

    if (!copyToContact(contact,
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emailCount, emails, socialsCount, socials
    ))
    {
        clearContact(contact);
        free(contact);
        RETURN_NULL_WITH_MSG("newContact:'copyToContact' returned NULL\n");
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
    const size_t emailCount,
    char** emails,
    const size_t socialsCount,
    char** socials
)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("editContact:'contact' was NULL\n");

    if (!copyToContact(contact,
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emailCount, emails, socialsCount, socials
    ))
        RETURN_NULL_WITH_MSG("editContact:'copyToContact' returned NULL\n");

    return contact;
}

Contact_t* contactAddEmail(Contact_t* contact, const char* email)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("contactAddEmail:'contact' was NULL\n");
    if (!email)
        RETURN_NULL_WITH_MSG("contactAddEmail:'email' was NULL\n");
    if (contact->emailCount >= MAX_EMAILS_COUNT)
        RETURN_NULL_WITH_MSG("contactAddEmail:'emailCount' at max\n");

    size_t length = strlen(email);
    if (length >= MAX_EMAIL_LENGTH)
        length = MAX_EMAIL_LENGTH - 1;

    size_t newCount = contact->emailCount + 1;
    char** newEmails = (char**)realloc(contact->emails, sizeof(char*) * newCount);
    if (!newEmails)
        RETURN_NULL_WITH_MSG("contactAddEmail:'newEmails' was NULL\n");

    contact->emails = newEmails;
    contact->emails[contact->emailCount] = (char*)malloc((length + 1) * sizeof(char));
    if (!contact->emails[contact->emailCount])
        RETURN_NULL_WITH_MSG("contactAddEmail:'contact->emails[contact->emailCount]' was NULL\n");

    strncpy(contact->emails[contact->emailCount], email, length);
    contact->emails[contact->emailCount][length] = '\0';
    contact->emailCount++;

    return contact;
}

Contact_t* contactAddSocial(Contact_t* contact, const char* socialLink)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("contactAddSocial:'contact' was NULL\n");
    if (!socialLink)
        RETURN_NULL_WITH_MSG("contactAddSocial:'socialLink' was NULL\n");
    if (contact->socialsCount >= MAX_SOCIALS_COUNT)
        RETURN_NULL_WITH_MSG("contactAddSocial:socialsCount at max\n");

    size_t length = strlen(socialLink);
    if (length >= SOCIALS_LINK_LENGTH)
        length = SOCIALS_LINK_LENGTH - 1;

    size_t newCount = contact->socialsCount + 1;
    char** newSocials = (char**)realloc(contact->socialsLink, sizeof(char*) * newCount);
    if (!newSocials)
        RETURN_NULL_WITH_MSG("contactAddSocial:'newSocials' was NULL\n");

    contact->socialsLink = newSocials;
    contact->socialsLink[contact->socialsCount] = (char*)malloc((length + 1) * sizeof(char));
    if (!contact->socialsLink[contact->socialsCount])
        RETURN_NULL_WITH_MSG("contactAddSocial:'contact->socialsLink[contact->socialsCount]' was NULL\n");

    strncpy(contact->socialsLink[contact->socialsCount], socialLink, length);
    contact->socialsLink[contact->socialsCount][length] = '\0';
    contact->socialsCount++;

    return contact;
}

Contact_t* contactEditEmail(Contact_t* contact, const size_t index, const char* newValue)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("contactEditEmail:'contact' was NULL\n");
    if (!newValue)
        RETURN_NULL_WITH_MSG("contactEditEmail:'newValue' was NULL\n");
    if (!contact->emails)
        RETURN_NULL_WITH_MSG("contactEditEmail:'contact->emails' was NULL\n");
    if (index >= contact->emailCount)
        RETURN_NULL_WITH_MSG("contactEditEmail:index out of range\n");
    size_t size = strlen(newValue);
    if (size >= MAX_EMAIL_LENGTH)
        size = MAX_EMAIL_LENGTH - 1;
    char * newEmail = (char*)realloc(contact->emails[index], (size + 1) * sizeof(char));
    if (!newEmail)
        RETURN_NULL_WITH_MSG("contactEditEmail:'newEmail' was NULL\n");
    contact->emails[index] = newEmail;

    strncpy(contact->emails[index], newValue, size);
    contact->emails[index][size] = '\0';

    return contact;
}

Contact_t* contactEditSocial(Contact_t* contact, const size_t index, const char* newValue)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("contactEditSocial:'contact' was NULL\n");
    if (!newValue)
        RETURN_NULL_WITH_MSG("contactEditSocial:'newValue' was NULL\n");
    if (!contact->socialsLink)
        RETURN_NULL_WITH_MSG("contactEditSocial:'contact->socialsLink' was NULL\n");
    if (index >= contact->socialsCount)
        RETURN_NULL_WITH_MSG("contactEditSocial:index out of range\n");
    size_t size = strlen(newValue);
    if (size >= SOCIALS_LINK_LENGTH)
        size = SOCIALS_LINK_LENGTH - 1;
    char * newSocial = (char*)realloc(contact->socialsLink[index], (size + 1) * sizeof(char));
    if (!newSocial)
        RETURN_NULL_WITH_MSG("contactEditSocial:'newSocial' was NULL\n");
    contact->socialsLink[index] = newSocial;

    strncpy(contact->socialsLink[index], newValue, size);
    contact->socialsLink[index][size] = '\0';

    return contact;
}

Contact_t* contactDeleteEmail(Contact_t* contact, const size_t index)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("contactDeleteEmail:'contact' was NULL\n");
    if (!contact->emails)
        RETURN_NULL_WITH_MSG("contactDeleteEmail:'contact->emails' was NULL\n");
    if (!contact->emailCount)
        return NULL;
    if (index >= contact->emailCount)
        RETURN_NULL_WITH_MSG("contactDeleteEmail:index out of range\n");
    if (!contact->emails[index])
        RETURN_NULL_WITH_MSG("contactDeleteEmail:email at index doen't exist\n");
    free(contact->emails[index]);
    contact->emails[index] = NULL;
    for (size_t i = index; i < contact->emailCount; i++)
        contact->emails[i] = contact->emails[i + 1];
    contact->emailCount--;
    if (contact->emailCount)
    {
        contact->emails = (char**)realloc(contact->emails, sizeof(char*) * contact->emailCount);
        if (!contact->emails)
            RETURN_NULL_WITH_MSG("contactDeleteEmail:'contact->emails' couldn't be reallocated!\n")
    }
    else
        CLEAR_ALLC(contact->emails);
    return contact;
}

Contact_t* contactDeleteSocial(Contact_t* contact, const size_t index)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("contactDeleteSocial:'contact' was NULL\n");
    if (!contact->socialsLink)
        RETURN_NULL_WITH_MSG("contactDeleteSocial:'contact->socialsLink' was NULL\n");
    if (!contact->socialsCount)
        return NULL;
    if (index >= contact->socialsCount)
        RETURN_NULL_WITH_MSG("contactDeleteSocial:index out of range\n");
    if (!contact->socialsLink[index])
        RETURN_NULL_WITH_MSG("contactDeleteSocial:socials link at index doen't exist\n");
    free(contact->socialsLink[index]);
    contact->socialsLink[index] = NULL;
    for (size_t i = index; i < contact->socialsCount; i++)
        contact->socialsLink[i] = contact->socialsLink[i + 1];
    contact->socialsCount--;
    if (contact->socialsCount)
    {
        contact->socialsLink = (char**)realloc(contact->socialsLink, sizeof(char*) * contact->socialsCount);
        if (!contact->socialsLink)
            RETURN_NULL_WITH_MSG("contactDeleteSocial:'contact->emails' couldn't be reallocated!\n")
    }
    else
        CLEAR_ALLC(contact->socialsLink);
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
        if (fieldPtr)
            fieldPtr = (char*)realloc(fieldPtr, size * sizeof(char));
        else
            fieldPtr = (char*)malloc(size * sizeof(char));
        if (!fieldPtr)
            RETURN_NULL_WITH_MSG("changeField:'fieldPtr' couldn't be allocated\n");
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
    const size_t emailCount,
    char** emails,
    const size_t socialsCount,
    char** socials
)
{
    if (!contact)
        RETURN_NULL_WITH_MSG("copyToContact:'contact' was NULL\n");

    if (!lastName || strlen(lastName) < 1)
        RETURN_NULL_WITH_MSG("copyToContact:'lastName' was NULL or empty\n");
    if (!firstName || strlen(firstName) < 1)
        RETURN_NULL_WITH_MSG("copyToContact:'firstName' was NULL or empty\n");
    char* buf = NULL;

    CHANGE_IF_GIVEN(buf, "lastName", contact->lastName, LAST_NAME_LENGTH, lastName);
    CHANGE_IF_GIVEN(buf, "firstName", contact->firstName, FIRST_NAME_LENGTH, firstName);
    CHANGE_IF_GIVEN(buf, "patronim", contact->patronim, PATRONIM_LENGTH, patronim);

    if (workPlace || position)
    {
        if (!contact->workInfo)
            contact->workInfo = (Work_t*)calloc(1, sizeof(Work_t));
        CHANGE_IF_GIVEN(buf, "workInfo->workPlace", contact->workInfo->workPlace,
            COMPANY_NAME_LENGTH, workPlace);
        CHANGE_IF_GIVEN(buf, "workInfo->position", contact->workInfo->position,
            POSITION_LENGTH, position);
    }

    if (workPhone || personalPhone || homePhone || extraPhone)
    {
        if (!contact->numbers)
            contact->numbers = (PhoneNumbers_t*)calloc(1, sizeof(PhoneNumbers_t));

        CHANGE_IF_GIVEN(buf, "numbers->work", contact->numbers->work,
            PHONE_NUMBER_LENGTH, workPhone);
        CHANGE_IF_GIVEN(buf, "numbers->personalPhone", contact->numbers->personal,
            PHONE_NUMBER_LENGTH, personalPhone);
        CHANGE_IF_GIVEN(buf, "numbers->homePhone", contact->numbers->home,
            PHONE_NUMBER_LENGTH, homePhone);
        CHANGE_IF_GIVEN(buf, "numbers->extraPhone", contact->numbers->extra,
            PHONE_NUMBER_LENGTH, extraPhone);
    }

    if (emails)
    {
        if (contact->emails)
        {
            size_t minCount = contact->emailCount > emailCount ? emailCount : contact->emailCount;
            size_t j = 0;
            for (; j < minCount && j < MAX_EMAILS_COUNT; j++)
                if (emails[j])
                    contactEditEmail(contact, j, emails[j]);
            for (size_t j = minCount; j < emailCount && j < MAX_EMAILS_COUNT; j++)
                if (emails[j])
                    contactAddEmail(contact, emails[j]);
        }
        else
            for (size_t i = 0; i < emailCount && emails[i] && i < MAX_EMAILS_COUNT; i++)
                contactAddEmail(contact, emails[i]);
    }
    if (socials)
    {
        if (contact->socialsLink)
        {
            size_t minCount = contact->socialsCount > socialsCount ? socialsCount : contact->socialsCount;
            size_t j = 0;
            for (; j < minCount && j < MAX_SOCIALS_COUNT; j++)
                if (socials[j])
                    contactEditSocial(contact, j, socials[j]);
            for (size_t j = minCount; j < socialsCount && j < MAX_SOCIALS_COUNT; j++)
                if (socials[j])
                    contactAddSocial(contact, socials[j]);
        }
        else
            for (size_t i = 0; i < socialsCount && socials[i] && i < MAX_SOCIALS_COUNT; i++)
                contactAddSocial(contact, socials[i]);
    }

    return contact;
}

Contact_t* createCopy(Contact_t* contact)
{
    Contact_t* copy = (Contact_t*)malloc(sizeof(Contact_t));
    if (!copy)
        RETURN_NULL_WITH_MSG("createCopy: не удалось создать копию!\n");

    copy = copyToContact(
        copy,
        contact->lastName, contact->firstName, contact->patronim,
        contact->workInfo ? contact->workInfo->workPlace : NULL,
        contact->workInfo ? contact->workInfo->position : NULL,
        contact->numbers ? contact->numbers->work : NULL,
        contact->numbers ? contact->numbers->personal : NULL,
        contact->numbers ? contact->numbers->home : NULL,
        contact->numbers ? contact->numbers->extra : NULL,
        contact->emailCount, contact->emails,
        contact->socialsCount, contact->socialsLink);
    if (!copy)
        RETURN_NULL_WITH_MSG("createCopy: не удалось скопировать данные контакта!\n");

    return copy;
}