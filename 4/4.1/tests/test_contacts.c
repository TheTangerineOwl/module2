#include "../unity/unity.h"
#include "../src/contacts.h"

#include "../src/contactListIO.h"

#include <stdarg.h>

Contact_t* contact1;
Contact_t* contact2;
Contact_t* contact3;

// size_t contactsCount;

#define FIELD_ALLC_CHECK(field, fieldname) \
    if (!field) \
    { \
        char msg[100]; \
        snprintf(msg, 99, "Failed to allocate \"%s\"", fieldname); \
        TEST_MESSAGE(msg); \
        TEST_ABORT(); \
    }

static char* initStrField(char* field, size_t maxSize, char* format, ...)
{
    char* buffer = (char*)malloc(maxSize * sizeof(char));
    FIELD_ALLC_CHECK(buffer, "initStrBuffer");

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, maxSize, format, args);
    va_end(args);
    size_t size = strlen(buffer);
    if (field)
        field = (char*)realloc(field, sizeof(char) * (size + 1));
    else
        field = (char*)malloc(sizeof(char) * (size + 1));
    FIELD_ALLC_CHECK(field, "initStrField");

    strncpy(field, buffer, size);
    field[size] = '\0';

    free(buffer);
    return field;
}

static Contact_t* initTestItem(size_t index)
{
    Contact_t* contact = (Contact_t*)malloc(sizeof(Contact_t));
    FIELD_ALLC_CHECK(contact, "contact");

    contact->numbers = (PhoneNumbers_t*)malloc(sizeof(PhoneNumbers_t));
    FIELD_ALLC_CHECK(contact->numbers, "numbers");
    contact->workInfo = (Work_t*)malloc(sizeof(Work_t));
    FIELD_ALLC_CHECK(contact->workInfo, "workInfo");

    contact->emails = (char**)calloc(MAX_EMAILS_COUNT, sizeof(char*));
    FIELD_ALLC_CHECK(contact->emails, "emails");
    contact->emailCount = 0;
    contact->socialsLink = (char**)calloc(MAX_SOCIALS_COUNT, sizeof(char*));
    FIELD_ALLC_CHECK(contact->socialsLink, "socialsLink");
    contact->socialsCount = 0;

    contact->lastName = NULL;
    contact->lastName = initStrField(contact->lastName, LAST_NAME_LENGTH, "%luLastName%lu", index, index);
    contact->firstName = NULL;
    contact->firstName = initStrField(contact->firstName, FIRST_NAME_LENGTH, "%luFirstName%lu", index, index);
    contact->patronim = NULL;
    contact->patronim = initStrField(contact->patronim, PATRONIM_LENGTH, "%luPatronim%lu", index, index);

    contact->workInfo->workPlace = NULL;
    contact->workInfo->workPlace = initStrField(contact->workInfo->workPlace,
        COMPANY_NAME_LENGTH, "%luCompany%lu", index, index);
    contact->workInfo->position = NULL;
    contact->workInfo->position = initStrField(contact->workInfo->position,
        POSITION_LENGTH, "%luPosition%lu", index, index);
    
    contact->numbers->work = NULL;
    contact->numbers->work = initStrField(contact->numbers->work,
        PHONE_NUMBER_LENGTH, "%lu111111111%lu", index, index);
    contact->numbers->personal = NULL;
    contact->numbers->personal = initStrField(contact->numbers->personal,
        PHONE_NUMBER_LENGTH, "%lu222222222%lu", index, index);
    contact->numbers->home = NULL;
    contact->numbers->home = initStrField(contact->numbers->home,
        PHONE_NUMBER_LENGTH, "%lu3333%lu", index, index);
    contact->numbers->extra = NULL;
    contact->numbers->extra = initStrField(contact->numbers->extra,
        PHONE_NUMBER_LENGTH, "%lu444444444%lu", index, index);

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
    {
        contact->emails[i] = NULL;
        contact->emails[i] = initStrField(contact->emails[i],
            MAX_EMAIL_LENGTH, "%lu@%lu.ru", index, index);
        contact->emailCount++;
    }

    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
    {
        contact->socialsLink[j] = NULL;
        contact->socialsLink[j] = initStrField(contact->socialsLink[j],
            SOCIALS_LINK_LENGTH, "%lu.ru/id%lu", index, index);
        contact->socialsCount++;
    }
    return contact;
}

static Contact_t* cleanTestItem(Contact_t* item)
{
    if (!item)
        return NULL;
    clearContact(item);
    return item;
}

void setUp(void)
{
    contact1 = initTestItem(0);
    contact2 = initTestItem(1);
    contact3 = initTestItem(2);
}

void tearDown(void)
{
    cleanTestItem(contact1);
    free(contact1);
    cleanTestItem(contact2);
    free(contact2);
    cleanTestItem(contact3);
    free(contact3);
}

void isEmptyContact(const Contact_t* actual)
{
    TEST_ASSERT_NULL(actual->lastName);
    TEST_ASSERT_NULL(actual->firstName);
    TEST_ASSERT_NULL(actual->patronim);

    TEST_ASSERT_NULL(actual->workInfo);

    TEST_ASSERT_NULL(actual->numbers);

    TEST_ASSERT_NULL(actual->emails);
    TEST_ASSERT_EQUAL_size_t(0, actual->emailCount);
    TEST_ASSERT_NULL(actual->socialsLink);
    TEST_ASSERT_EQUAL_size_t(0, actual->socialsCount);

}

void compareContactsFull(const Contact_t* expected, const Contact_t* actual)
{
    TEST_ASSERT_EQUAL_STRING(expected->lastName, actual->lastName);
    TEST_ASSERT_EQUAL_STRING(expected->firstName, actual->firstName);
    TEST_ASSERT_EQUAL_STRING(expected->patronim, actual->patronim);

    TEST_ASSERT_EQUAL_STRING(expected->workInfo->workPlace, actual->workInfo->workPlace);
    TEST_ASSERT_EQUAL_STRING(expected->workInfo->position, actual->workInfo->position);

    TEST_ASSERT_EQUAL_STRING(expected->numbers->work, actual->numbers->work);
    TEST_ASSERT_EQUAL_STRING(expected->numbers->personal, actual->numbers->personal);
    TEST_ASSERT_EQUAL_STRING(expected->numbers->home, actual->numbers->home);
    TEST_ASSERT_EQUAL_STRING(expected->numbers->extra, actual->numbers->extra);

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        TEST_ASSERT_EQUAL_STRING(expected->emails[i], actual->emails[i]);

    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        TEST_ASSERT_EQUAL_STRING(expected->socialsLink[j], actual->socialsLink[j]);
}

void test_clearContact(void)
{
    Contact_t* test = clearContact(NULL);
    TEST_ASSERT_NULL(test);
    clearContact(contact1);
    TEST_ASSERT_NOT_NULL(contact1);
    isEmptyContact(contact1);
}

void test_copyToContact(void)
{
    TEST_ASSERT_NULL(copyToContact(
        NULL,
        "1", "1", "", "", "", "", "", "", "",
        NULL, NULL));
    TEST_ASSERT_NULL(copyToContact(
        contact1,
        "", "1", "", "", "", "", "", "", "",
        NULL, NULL));
    TEST_ASSERT_NULL(copyToContact(
        contact1,
        "1", "", "", "", "", "", "", "", "",
        NULL, NULL));

    Work_t* testWork = (Work_t*)malloc(sizeof(Work_t));
    PhoneNumbers_t* testNumbers = (PhoneNumbers_t*)malloc(sizeof(PhoneNumbers_t));

    Contact_t* testContact = (Contact_t*)malloc(sizeof(Contact_t));
    if (!testContact || !testWork || !testNumbers)
        TEST_FAIL_MESSAGE("Can't create test contact");

    memset(testWork, 0, sizeof(Work_t));
    memset(testNumbers, 0, sizeof(PhoneNumbers_t));
    memset(testContact, 0, sizeof(Contact_t));

    testContact->numbers = testNumbers;
    testContact->workInfo = testWork;

    testContact->emails = (char**)malloc(sizeof(char*) * MAX_EMAILS_COUNT);
    testContact->socialsLink = (char**)malloc(sizeof(char*) * MAX_SOCIALS_COUNT);

    testContact->lastName = initStrField(testContact->lastName, LAST_NAME_LENGTH, "TestLastName");
    testContact->firstName = initStrField(testContact->firstName, FIRST_NAME_LENGTH, "TestFirstName");
    testContact->patronim = initStrField(testContact->patronim, PATRONIM_LENGTH, "TestPatronim");

    testWork->workPlace = initStrField(testWork->workPlace, COMPANY_NAME_LENGTH, "TestCompany");
    testWork->position = initStrField(testWork->position, POSITION_LENGTH, "TestPosition");

    testNumbers->work = initStrField(testNumbers->work, PHONE_NUMBER_LENGTH, "00000000000");
    testNumbers->personal = initStrField(testNumbers->personal, PHONE_NUMBER_LENGTH, "11111111111");
    testNumbers->home = initStrField(testNumbers->home, PHONE_NUMBER_LENGTH, "222222");
    testNumbers->extra = initStrField(testNumbers->extra, PHONE_NUMBER_LENGTH, "33333333333");

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        contactAddEmail(testContact, "test1@test1.ru");
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        contactAddSocial(testContact, "test1.ru/test1");

    Contact_t* copiedContact = copyToContact(
        contact1,
        testContact->lastName, testContact->firstName, testContact->patronim,
        testContact->workInfo->workPlace, testContact->workInfo->position,
        testContact->numbers->work, testContact->numbers->personal,
        testContact->numbers->home, testContact->numbers->extra,
        testContact->emails, testContact->socialsLink
    );
    TEST_ASSERT_NOT_NULL(copiedContact);
    TEST_ASSERT_EQUAL_PTR(contact1, copiedContact);

    compareContactsFull(testContact, copiedContact);
    
    testContact->lastName = initStrField(testContact->lastName, LAST_NAME_LENGTH, "TestNewLastName");
    testContact->firstName = initStrField(testContact->firstName, FIRST_NAME_LENGTH, "TestNewFirstName");

    copiedContact = copyToContact(
        contact1,
        "TestNewLastName", "TestNewFirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );
    
    TEST_ASSERT_NOT_NULL(copiedContact);
    TEST_ASSERT_EQUAL_PTR(contact1, copiedContact);
    TEST_ASSERT_EQUAL_STRING("TestNewLastName", copiedContact->lastName);
    TEST_ASSERT_EQUAL_STRING("TestNewFirstName", copiedContact->firstName);

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        contactEditEmail(testContact, i, "test2@test1.ru");
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        contactEditSocial(testContact, j, "test2.ru/test1");

    copiedContact = copyToContact(
        contact1,
        testContact->lastName, testContact->firstName, NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        testContact->emails, testContact->socialsLink
    );
    
    TEST_ASSERT_NOT_NULL(copiedContact);
    TEST_ASSERT_EQUAL_PTR(contact1, copiedContact);
    compareContactsFull(testContact, contact1);
    clearContact(testContact);
    free(testContact);
}

void test_newContact(void)
{
    TEST_ASSERT_NULL(newContact("", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
    TEST_ASSERT_NULL(newContact("1", "", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));

    Work_t* testWork = (Work_t*)malloc(sizeof(Work_t));
    PhoneNumbers_t* testNumbers = (PhoneNumbers_t*)malloc(sizeof(PhoneNumbers_t));

    Contact_t* testContact = (Contact_t*)malloc(sizeof(Contact_t));
    if (!testContact || !testWork || !testNumbers)
        TEST_FAIL_MESSAGE("Can't create test contact");

    memset(testWork, 0, sizeof(Work_t));
    memset(testNumbers, 0, sizeof(PhoneNumbers_t));
    memset(testContact, 0, sizeof(Contact_t));

    testContact->numbers = testNumbers;
    testContact->workInfo = testWork;

    testContact->emails = (char**)malloc(sizeof(char*) * MAX_EMAILS_COUNT);
    testContact->socialsLink = (char**)malloc(sizeof(char*) * MAX_SOCIALS_COUNT);

    testContact->lastName = initStrField(testContact->lastName, LAST_NAME_LENGTH, "Test2LastName");
    testContact->firstName = initStrField(testContact->firstName, FIRST_NAME_LENGTH, "Test2FirstName");
    testContact->patronim = initStrField(testContact->patronim, PATRONIM_LENGTH, "Test2Patronim");

    testWork->workPlace = initStrField(testWork->workPlace, COMPANY_NAME_LENGTH, "Test2Company");
    testWork->position = initStrField(testWork->position, POSITION_LENGTH, "Test2Position");

    testNumbers->work = initStrField(testNumbers->work, PHONE_NUMBER_LENGTH, "20000000000");
    testNumbers->personal = initStrField(testNumbers->personal, PHONE_NUMBER_LENGTH, "21111111111");
    testNumbers->home = initStrField(testNumbers->home, PHONE_NUMBER_LENGTH, "422222");
    testNumbers->extra = initStrField(testNumbers->extra, PHONE_NUMBER_LENGTH, "23333333333");

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        contactAddEmail(testContact, "test2@test1.ru");
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        contactAddSocial(testContact, "test2.ru/test1");

    Contact_t* newC = newContact(
        testContact->lastName, testContact->firstName, testContact->patronim,
        testContact->workInfo->workPlace, testContact->workInfo->position,
        testContact->numbers->work, testContact->numbers->personal,
        testContact->numbers->home, testContact->numbers->extra,
        testContact->emails, testContact->socialsLink
    );

    TEST_ASSERT_NOT_NULL(newC);

    compareContactsFull(testContact, newC);
}

void test_editContact(void)
{
    TEST_ASSERT_NULL(editContact(NULL,
        "1", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));

    Work_t* testWork = (Work_t*)malloc(sizeof(Work_t));
    PhoneNumbers_t* testNumbers = (PhoneNumbers_t*)malloc(sizeof(PhoneNumbers_t));

    Contact_t* testContact = (Contact_t*)malloc(sizeof(Contact_t));
    if (!testContact || !testWork || !testNumbers)
        TEST_FAIL_MESSAGE("Can't create test contact");

    memset(testWork, 0, sizeof(Work_t));
    memset(testNumbers, 0, sizeof(PhoneNumbers_t));
    memset(testContact, 0, sizeof(Contact_t));

    testContact->numbers = testNumbers;
    testContact->workInfo = testWork;

    testContact->emails = (char**)malloc(sizeof(char*) * MAX_EMAILS_COUNT);
    testContact->socialsLink = (char**)malloc(sizeof(char*) * MAX_SOCIALS_COUNT);

    testContact->lastName = initStrField(testContact->lastName, LAST_NAME_LENGTH, "Test2LastName");
    testContact->firstName = initStrField(testContact->firstName, FIRST_NAME_LENGTH, "Test2FirstName");
    testContact->patronim = initStrField(testContact->patronim, PATRONIM_LENGTH, "Test2Patronim");

    testWork->workPlace = initStrField(testWork->workPlace, COMPANY_NAME_LENGTH, "Test2Company");
    testWork->position = initStrField(testWork->position, POSITION_LENGTH, "Test2Position");

    testNumbers->work = initStrField(testNumbers->work, PHONE_NUMBER_LENGTH, "20000000000");
    testNumbers->personal = initStrField(testNumbers->personal, PHONE_NUMBER_LENGTH, "21111111111");
    testNumbers->home = initStrField(testNumbers->home, PHONE_NUMBER_LENGTH, "422222");
    testNumbers->extra = initStrField(testNumbers->extra, PHONE_NUMBER_LENGTH, "23333333333");

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        contactAddEmail(testContact, "test2@test1.ru");
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        contactAddSocial(testContact, "test2.ru/test1");

    Contact_t* newContact = editContact(
        contact1,
        testContact->lastName, testContact->firstName, testContact->patronim,
        testContact->workInfo->workPlace, testContact->workInfo->position,
        testContact->numbers->work, testContact->numbers->personal,
        testContact->numbers->home, testContact->numbers->extra,
        testContact->emails, testContact->socialsLink
    );

    TEST_ASSERT_NOT_NULL(newContact);
    // TEST_ASSERT_EQUAL_size_t(oldCount, contactsCount);
    TEST_ASSERT_EQUAL_PTR(contact1, newContact);
    compareContactsFull(testContact, newContact);
}

void test_compareContacts(void)
{
    TEST_ASSERT_LESS_THAN(0,
        compareContacts(*contact1, *contact2));
    TEST_ASSERT_EQUAL(0,
        compareContacts(*contact1, *contact1));
    TEST_ASSERT_GREATER_THAN(0,
        compareContacts(*contact3, *contact2));
}

void test_contactAddEmail(void)
{
    Contact_t* contact = contact1;
    size_t count = contact->emailCount;

    TEST_ASSERT_NULL(contactAddEmail(NULL, "1"));
    TEST_ASSERT_NULL(contactAddEmail(contact, NULL));
    TEST_ASSERT_NULL(contactAddEmail(contact, "more_than_max"));

    TEST_ASSERT_EQUAL(count, contact->emailCount);

    free(contact->emails[contact->emailCount - 1]);
    contact->emails[contact->emailCount - 1] = NULL;
    contact->emailCount--;
    count = contact->emailCount;

    TEST_ASSERT_NOT_NULL(contactAddEmail(contact, "last_email"));

    TEST_ASSERT_EQUAL_STRING("last_email",
        contact->emails[contact->emailCount - 1]);

    TEST_ASSERT_EQUAL(count + 1, contact->emailCount);
}

void test_contactEditEmail(void)
{
    Contact_t* contact = contact1;
    size_t count = contact->emailCount;

    TEST_ASSERT_NULL(contactEditEmail(NULL, 0, "1"));
    TEST_ASSERT_NULL(contactEditEmail(contact, 0, NULL));
    TEST_ASSERT_NULL(contactEditEmail(contact, contact->emailCount, "2"));
    TEST_ASSERT_NOT_NULL(contactEditEmail(contact, 0, "new_email"));

    TEST_ASSERT_EQUAL_STRING(contact->emails[0], "new_email");
    TEST_ASSERT_EQUAL(count, contact->emailCount);
}

void test_contactDeleteEmail(void)
{
    Contact_t* contact = contact1;
    size_t count = contact->emailCount;

    TEST_ASSERT_NULL(contactDeleteEmail(NULL, 0));
    TEST_ASSERT_NULL(contactDeleteEmail(contact, contact->emailCount));
    char** emails = contact->emails;
    contact->emails = NULL;
    TEST_ASSERT_NULL(contactDeleteEmail(contact, 0));
    contact->emails = emails;

    TEST_ASSERT_NOT_NULL(contactDeleteEmail(contact, 0));
    TEST_ASSERT_EQUAL(count - 1, contact->emailCount);
}

void test_contactAddSocial(void)
{
    Contact_t* contact = contact1;
    size_t count = contact->socialsCount;

    TEST_ASSERT_NULL(contactAddSocial(NULL, "1"));
    TEST_ASSERT_NULL(contactAddSocial(contact, NULL));
    TEST_ASSERT_NULL(contactAddSocial(contact, "more_than_max"));

    TEST_ASSERT_EQUAL(count, contact->socialsCount);

    free(contact->socialsLink[contact->socialsCount - 1]);
    contact->socialsLink[contact->socialsCount - 1] = NULL;
    contact->socialsCount--;
    count = contact->socialsCount;

    TEST_ASSERT_NOT_NULL(contactAddSocial(contact, "last_social"));

    TEST_ASSERT_EQUAL_STRING("last_social",
        contact->socialsLink[contact->socialsCount - 1]);

    TEST_ASSERT_EQUAL(count + 1, contact->socialsCount);
}

void test_contactEditSocial(void)
{
    Contact_t* contact = contact1;
    size_t count = contact->socialsCount;

    TEST_ASSERT_NULL(contactEditSocial(NULL, 0, "1"));
    TEST_ASSERT_NULL(contactEditSocial(contact, 0, NULL));
    TEST_ASSERT_NULL(contactEditSocial(contact, contact->socialsCount, "2"));
    TEST_ASSERT_NOT_NULL(contactEditSocial(contact, 0, "new_socials"));

    TEST_ASSERT_EQUAL_STRING(contact->socialsLink[0], "new_socials");
    TEST_ASSERT_EQUAL(count, contact->socialsCount);
}

void test_contactDeleteSocial(void)
{
    Contact_t* contact = contact1;
    size_t count = contact->socialsCount;

    TEST_ASSERT_NULL(contactDeleteSocial(NULL, 0));
    TEST_ASSERT_NULL(contactDeleteSocial(contact, contact->socialsCount));
    char** socialsLink = contact->socialsLink;
    contact->socialsLink = NULL;
    TEST_ASSERT_NULL(contactDeleteSocial(contact, 0));
    contact->socialsLink = socialsLink;

    TEST_ASSERT_NOT_NULL(contactDeleteSocial(contact, 0));
    TEST_ASSERT_EQUAL(count - 1, contact->socialsCount);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_clearContact);
    RUN_TEST(test_copyToContact);
    RUN_TEST(test_newContact);
    RUN_TEST(test_editContact);
    RUN_TEST(test_compareContacts);
    RUN_TEST(test_contactAddEmail);
    RUN_TEST(test_contactEditEmail);
    RUN_TEST(test_contactDeleteEmail);
    RUN_TEST(test_contactAddSocial);
    RUN_TEST(test_contactEditSocial);
    RUN_TEST(test_contactDeleteSocial);
    return UNITY_END();
}