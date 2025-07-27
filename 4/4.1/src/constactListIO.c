#include "contactListIO.h"

char* enterStrField(char* field, size_t fieldLength);

void printContact(const Contact_t contact)
{
    printf("%s: %s\n", menuItems[LAST_NAME].name, contact.lastName);
    printf("%s: %s\n", menuItems[FIRST_NAME].name, contact.firstName);
    if (contact.patronim) printf("%s: %s\n", menuItems[PATRONIM].name, contact.patronim);

    if (contact.workInfo)
    {
        if (contact.workInfo->workPlace)
            printf("%s: %s\n", menuItems[WORK_PLACE].name, contact.workInfo->workPlace);
        if (contact.workInfo->position)
            printf("%s: %s\n", menuItems[POSITION].name, contact.workInfo->position);
    }

    if (contact.numbers)
    {
        if (contact.numbers->work)
            printf("%s: %s\n", menuItems[WORK_PHONE].name, contact.numbers->work);
        if (contact.numbers->personal)
            printf("%s: %s\n", menuItems[PERSONAL_PHONE].name, contact.numbers->personal);
        if (contact.numbers->home)
            printf("%s: %s\n", menuItems[HOME_PHONE].name, contact.numbers->home);
        if (contact.numbers->extra)
            printf("%s: %s\n", menuItems[EXTRA_PHONE].name, contact.numbers->extra);
    }

    if (contact.emails)
    {
        for (size_t i = 0; contact.emails[i] && i < contact.emailCount; i++)
            printf("%s %lu: %s\n", menuItems[EMAIL].name, i + 1, contact.emails[i]);
    }

    if (contact.socialsLink)
    {
        for (size_t i = 0; contact.socialsLink[i] && i < contact.socialsCount; i++)
            printf("%s %lu: %s\n", menuItems[SOCIAL_LINK].name, i + 1, contact.socialsLink[i]);
    }
}

void printAllContacts(const Contact_t* contacts);
int enterContactInfo(Contact_t* contacts);
int editContactInfo(Contact_t *contact);
char* changeField(char* oldField, const ContactField field);
void printShortContacts(const Contact_t* contacts);
Contact_t* chooseContact(Contact_t* contacts);