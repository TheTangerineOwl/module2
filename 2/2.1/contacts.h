#ifndef CONTACTS_H
#define CONTACTS_H

#define NAME_PERSON_LENGTH 256
#define NAME_COMPANY_LENGTH 256
#define MAX_EMAIL_LENGTH 256
#define PHONE_NUMBER_LENGTH 12
#define SOCIALS_LINK_LENGTH 256
#define MAX_SOCIALS_COUNT 5
#define MAX_EMAILS_COUNT 5
#define MAX_CONTACTS_COUNT 100

typedef struct
{
    char workPlace[NAME_COMPANY_LENGTH];
    char position[NAME_COMPANY_LENGTH];
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
    char lastName[NAME_PERSON_LENGTH];
    char firstName[NAME_PERSON_LENGTH];
    char patronim[NAME_PERSON_LENGTH];
    PhoneNumbers_t numbers;
    Work_t workInfo;
    char emails[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH];
    char socialsLink[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH];
} Contact_t;

extern int contactsCount;

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
);

int deleteContact(
    Contact_t* contacts,
    int index
);

#endif