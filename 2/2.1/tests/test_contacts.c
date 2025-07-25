#include "../unity/unity.h"
#include "../src/contacts.h"

#include <string.h>

#define TEST_OBJ_CREATE 3

Contact_t contacts[MAX_CONTACTS_COUNT];
size_t contactsCount;

static char emailsDummy[MAX_EMAILS_COUNT][MAX_EMAIL_LENGTH] = {""};
static char socialsDummy[MAX_SOCIALS_COUNT][SOCIALS_LINK_LENGTH] = {""};

static const Work_t emptyWork = {
    workPlace : "",
    position : ""
};
static const PhoneNumbers_t emptyNumbers = {
    work : "",
    personal : "",
    home : "",
    extra : ""
};

static Contact_t emptyContact = {
    lastName : "",
    firstName : "",
    patronim : "",
    numbers : emptyNumbers,
    workInfo : emptyWork,
    emails : {""},
    socialsLink : {""}
};

void setUp(void)
{
    size_t d;
    for (size_t k = 0; k < TEST_OBJ_CREATE; k++)
    {
        d = k % 10;
        for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
            snprintf(contacts[k].emails[i], MAX_EMAIL_LENGTH, "%ld@%ld.email.ru", i, k);
        for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
            snprintf(contacts[k].socialsLink[j], SOCIALS_LINK_LENGTH, "site%ld.ru/%ld", j, k);
        snprintf(contacts[k].lastName, LAST_NAME_LENGTH, "LastName%ld", k);
        snprintf(contacts[k].firstName, FIRST_NAME_LENGTH, "FirstName%ld", k);
        snprintf(contacts[k].patronim, PATRONIM_LENGTH, "Patronim%ld", k);

        snprintf(contacts[k].workInfo.workPlace, COMPANY_NAME_LENGTH, "CompanyName%ld", k);
        snprintf(contacts[k].workInfo.position, POSITION_LENGTH, "Position%ld", k);

        snprintf(contacts[k].numbers.work, PHONE_NUMBER_LENGTH, "8999111%ld%ld%ld%ld", d, d, d, d);
        snprintf(contacts[k].numbers.personal, PHONE_NUMBER_LENGTH, "8888222%ld%ld%ld%ld", d, d, d, d);
        snprintf(contacts[k].numbers.home, PHONE_NUMBER_LENGTH, "%ld%ld%ld%ld", d, d, d, d);
        snprintf(contacts[k].numbers.extra, PHONE_NUMBER_LENGTH, "8777333%ld%ld%ld%ld", d, d, d, d);
        contactsCount++;
    }
}

void tearDown(void)
{
    contactsCount = 0;
    for (size_t i = 0; i < MAX_CONTACTS_COUNT; i++)
        clearContact(&contacts[i]);
}

void compareContacts(const Contact_t* expected, const Contact_t* actual)
{
    TEST_ASSERT_EQUAL_STRING(expected->lastName, actual->lastName);
    TEST_ASSERT_EQUAL_STRING(expected->firstName, actual->firstName);
    TEST_ASSERT_EQUAL_STRING(expected->patronim, actual->patronim);

    TEST_ASSERT_EQUAL_STRING(expected->workInfo.workPlace, actual->workInfo.workPlace);
    TEST_ASSERT_EQUAL_STRING(expected->workInfo.position, actual->workInfo.position);

    TEST_ASSERT_EQUAL_STRING(expected->numbers.work, actual->numbers.work);
    TEST_ASSERT_EQUAL_STRING(expected->numbers.personal, actual->numbers.personal);
    TEST_ASSERT_EQUAL_STRING(expected->numbers.home, actual->numbers.home);
    TEST_ASSERT_EQUAL_STRING(expected->numbers.extra, actual->numbers.extra);

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        TEST_ASSERT_EQUAL_STRING(expected->emails[i], actual->emails[i]);

    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        TEST_ASSERT_EQUAL_STRING(expected->socialsLink[j], actual->socialsLink[j]);
}

void test_clearContact(void)
{
    TEST_ASSERT_NULL(clearContact(NULL));
    TEST_ASSERT_NOT_NULL(clearContact(&contacts[0]));
    compareContacts(&emptyContact, &contacts[0]);
}

void test_copyContactInfo(void)
{
    TEST_ASSERT_NULL(copyContactInfo(
        &contacts[0],
        NULL, "1", "", "", "", "", "", "", "",
        emailsDummy, socialsDummy));
    TEST_ASSERT_NULL(copyContactInfo(
        &contacts[0],
        "1", NULL, "", "", "", "", "", "", "",
        emailsDummy, socialsDummy));
    TEST_ASSERT_NULL(copyContactInfo(
        &contacts[0],
        "", "1", "", "", "", "", "", "", "",
        emailsDummy, socialsDummy));
    TEST_ASSERT_NULL(copyContactInfo(
        &contacts[0],
        "1", "", "", "", "", "", "", "", "",
        emailsDummy, socialsDummy));

    Work_t testWork = {
        workPlace: "TestCompany",
        position: "TestPosition"
    };
    PhoneNumbers_t testNumbers = {
        work: "00000000000",
        personal: "11111111111",
        home: "222222",
        extra: "33333333333"
    };

    Contact_t testContact = {
        lastName: "TestLastName",
        firstName: "TestFirstName",
        patronim: "TestPatronim",
        numbers: testNumbers,
        workInfo: testWork
    };

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        strncpy(testContact.emails[i], "test1@test1.ru", MAX_EMAIL_LENGTH);
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        strncpy(testContact.socialsLink[j], "test1.ru/test1", SOCIALS_LINK_LENGTH);

    Contact_t* copiedContact = copyContactInfo(
        &contacts[0],
        testContact.lastName, testContact.firstName, testContact.patronim,
        testContact.workInfo.workPlace, testContact.workInfo.position,
        testContact.numbers.work, testContact.numbers.personal,
        testContact.numbers.home, testContact.numbers.extra,
        testContact.emails, testContact.socialsLink
    );
    TEST_ASSERT_NOT_NULL(copiedContact);
    TEST_ASSERT_EQUAL_PTR(&contacts[0], copiedContact);
    compareContacts(&testContact, &contacts[0]);
    
    strncpy(testContact.lastName, "TestNewLastName", LAST_NAME_LENGTH);
    strncpy(testContact.firstName, "TestNewFirstName", FIRST_NAME_LENGTH);

    copiedContact = copyContactInfo(
        &contacts[0],
        "TestNewLastName", "TestNewFirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );
    
    TEST_ASSERT_NOT_NULL(copiedContact);
    TEST_ASSERT_EQUAL_PTR(&contacts[0], copiedContact);
    compareContacts(&testContact, &contacts[0]);

    for (size_t i = MAX_EMAILS_COUNT - 1; i > MAX_EMAILS_COUNT / 2; i--)
        strncpy(testContact.emails[i], "", MAX_EMAIL_LENGTH);
    for (size_t j = MAX_SOCIALS_COUNT - 1; j > MAX_SOCIALS_COUNT / 2; j--)
        strncpy(testContact.socialsLink[j], "", SOCIALS_LINK_LENGTH);

    copiedContact = copyContactInfo(
        &contacts[0],
        testContact.lastName, testContact.firstName, NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        testContact.emails, testContact.socialsLink
    );
    
    TEST_ASSERT_NOT_NULL(copiedContact);
    TEST_ASSERT_EQUAL_PTR(&contacts[0], copiedContact);
    compareContacts(&testContact, &contacts[0]);
}

void test_addContact(void)
{
    TEST_ASSERT_NULL(addContact(NULL, "1", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
    size_t oldCount = contactsCount;
    contactsCount = MAX_CONTACTS_COUNT;
    TEST_ASSERT_NULL(addContact(contacts, "1", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
    contactsCount = oldCount;
    // Случай, когда copyContactInfo внутри вернет NULL;
    TEST_ASSERT_NULL(addContact(contacts, "", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));

    Work_t testWork = {
        workPlace: "Test2Company",
        position: "Test2Position"
    };
    PhoneNumbers_t testNumbers = {
        work: "20000000000",
        personal: "21111111111",
        home: "422222",
        extra: "23333333333"
    };

    Contact_t testContact = {
        lastName: "Test2LastName",
        firstName: "Test2FirstName",
        patronim: "Test2Patronim",
        numbers: testNumbers,
        workInfo: testWork
    };

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        strncpy(testContact.emails[i], "test2@test2.ru", MAX_EMAIL_LENGTH);
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        strncpy(testContact.socialsLink[j], "test2.ru/test2", SOCIALS_LINK_LENGTH);

    oldCount = contactsCount;

    Contact_t* newContact = addContact(
        contacts,
        testContact.lastName, testContact.firstName, testContact.patronim,
        testContact.workInfo.workPlace, testContact.workInfo.position,
        testContact.numbers.work, testContact.numbers.personal,
        testContact.numbers.home, testContact.numbers.extra,
        testContact.emails, testContact.socialsLink
    );

    TEST_ASSERT_NOT_NULL(newContact);
    TEST_ASSERT_EQUAL_size_t(oldCount + 1, contactsCount);
    TEST_ASSERT_EQUAL_PTR(&contacts[contactsCount - 1], newContact);
    compareContacts(&testContact, newContact);
}

void test_deleteContact(void)
{
    TEST_ASSERT_EQUAL(0, deleteContact(NULL, 0));
    TEST_ASSERT_EQUAL(0, deleteContact(contacts, MAX_CONTACTS_COUNT + 1));
    TEST_ASSERT_EQUAL(0, deleteContact(contacts, contactsCount));
    Contact_t oldValues[MAX_CONTACTS_COUNT];
    for (size_t i = 0; i < MAX_CONTACTS_COUNT; i++)
        oldValues[i] = contacts[i];
    size_t oldCount = contactsCount;
    TEST_ASSERT_EQUAL(1, deleteContact(contacts, 0));
    TEST_ASSERT_EQUAL(oldCount - 1, contactsCount);

    // Проверка того контакта, который был последним до удаления.
    compareContacts(&emptyContact, &contacts[oldCount - 1]);

    for (size_t i = 0; i < MAX_CONTACTS_COUNT - 1; i++)
        compareContacts(&oldValues[i + 1], &contacts[i]);
    compareContacts(&emptyContact, &contacts[MAX_CONTACTS_COUNT - 1]);
}

void test_editContact(void)
{
    TEST_ASSERT_NULL(editContact(NULL, "1", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
    size_t oldCount = contactsCount;
    // Случай, когда copyContactInfo внутри вернет NULL;
    TEST_ASSERT_NULL(editContact(&contacts[0], "", "1", NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL));

    Work_t testWork = {
        workPlace: "Test3Company",
        position: "Test3Position"
    };
    PhoneNumbers_t testNumbers = {
        work: "30000000000",
        personal: "31111111111",
        home: "322222",
        extra: "33333333333"
    };

    Contact_t testContact = {
        lastName: "Test3LastName",
        firstName: "Test3FirstName",
        patronim: "Test3Patronim",
        numbers: testNumbers,
        workInfo: testWork
    };

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
        strncpy(testContact.emails[i], "test3@test3.ru", MAX_EMAIL_LENGTH);
    for (size_t j = 0; j < MAX_SOCIALS_COUNT; j++)
        strncpy(testContact.socialsLink[j], "test3.ru/test3", SOCIALS_LINK_LENGTH);

    Contact_t* newContact = editContact(
        contacts,
        testContact.lastName, testContact.firstName, testContact.patronim,
        testContact.workInfo.workPlace, testContact.workInfo.position,
        testContact.numbers.work, testContact.numbers.personal,
        testContact.numbers.home, testContact.numbers.extra,
        testContact.emails, testContact.socialsLink
    );

    TEST_ASSERT_NOT_NULL(newContact);
    TEST_ASSERT_EQUAL_size_t(oldCount, contactsCount);
    TEST_ASSERT_EQUAL_PTR(&contacts[0], newContact);
    compareContacts(&testContact, newContact);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_clearContact);
    RUN_TEST(test_copyContactInfo);
    RUN_TEST(test_addContact);
    RUN_TEST(test_deleteContact);
    RUN_TEST(test_editContact);
    return UNITY_END();
}