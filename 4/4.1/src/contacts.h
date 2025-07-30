#ifndef CONTACTS_H
#define CONTACTS_H

#include <string.h>
#include <malloc.h>

#define LAST_NAME_LENGTH 52
#define FIRST_NAME_LENGTH 52
#define PATRONIM_LENGTH 52
#define COMPANY_NAME_LENGTH 256
#define POSITION_LENGTH 256
#define MAX_EMAIL_LENGTH 256
#define PHONE_NUMBER_LENGTH 13
#define SOCIALS_LINK_LENGTH 256
#define MAX_SOCIALS_COUNT 5
#define MAX_EMAILS_COUNT 5
#define MAX_CONTACTS_COUNT 100

#ifndef ERR_STREAM
#define ERR_STREAM stderr
#endif

#ifndef RETURN_NULL_WITH_MSG

#define DEBUG_ERR 0

#if DEBUG_ERR
    #define RETURN_NULL_WITH_MSG(formatMsg, ...) \
        { \
            fprintf(ERR_STREAM, formatMsg, ##__VA_ARGS__); \
            return NULL; \
        }
    #else
    #define RETURN_NULL_WITH_MSG(formatMsg, ...) \
        { \
            return NULL; \
        }
    #endif
#endif

typedef struct Work_t
{
    char* workPlace;
    char* position;
} Work_t;

typedef struct PhoneNumbers_t
{
    char* work;
    char* personal;
    char* home;
    char* extra;
} PhoneNumbers_t;

typedef struct Contact_t
{
    char* lastName;
    char* firstName;
    char* patronim;
    PhoneNumbers_t* numbers;
    Work_t* workInfo;
    char** emails;
    size_t emailCount;
    char** socialsLink;
    size_t socialsCount;
} Contact_t;

extern size_t contactsCount;

int compareContacts(const Contact_t contact1, const Contact_t contact2);

Contact_t* clearContact(Contact_t* contact);
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
);

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
);

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
);

Contact_t* createCopy(Contact_t* contact);

Contact_t* contactAddEmail(Contact_t* contact, const char* email);
Contact_t* contactAddSocial(Contact_t* contact, const char* socialLink);
Contact_t* contactDeleteEmail(Contact_t* contact, const size_t index);
Contact_t* contactDeleteSocial(Contact_t* contact, const size_t index);
Contact_t* contactEditEmail(Contact_t* contact, const size_t index, const char* newValue);
Contact_t* contactEditSocial(Contact_t* contact, const size_t index, const char* newValue);

#endif