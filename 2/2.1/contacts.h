#ifndef CONTACTS_H
#define CONTACTS_H

#include <string.h>

#define LAST_NAME_LENGTH 50
#define FIRST_NAME_LENGTH 50
#define PATRONIM_LENGTH 50
#define COMPANY_NAME_LENGTH 256
#define POSITION_LENGTH 256
#define MAX_EMAIL_LENGTH 256
#define PHONE_NUMBER_LENGTH 12
#define SOCIALS_LINK_LENGTH 256
#define MAX_SOCIALS_COUNT 5
#define MAX_EMAILS_COUNT 5
#define MAX_CONTACTS_COUNT 100

typedef struct
{
    char workPlace[COMPANY_NAME_LENGTH];
    char position[POSITION_LENGTH];
} Work_t;

typedef struct
{
    char work[PHONE_NUMBER_LENGTH];
    char personal[PHONE_NUMBER_LENGTH];
    char home[PHONE_NUMBER_LENGTH];
    char extra[PHONE_NUMBER_LENGTH];
} PhoneNumbers_t;

typedef struct
{
    char lastName[LAST_NAME_LENGTH];
    char firstName[FIRST_NAME_LENGTH];
    char patronim[PATRONIM_LENGTH];
    PhoneNumbers_t numbers;
    Work_t workInfo;
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH];
    char socialsLink[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH];
} Contact_t;

extern int contactsCount;

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
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH],
    char socials[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH] 
);

int deleteContact(
    Contact_t* contacts,
    int index
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
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH],
    char socials[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH]
);

Contact_t* copyContactInfo(
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
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH],
    char socials[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH]  
);

#endif