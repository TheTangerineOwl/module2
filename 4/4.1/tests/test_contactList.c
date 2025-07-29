#include "../unity/unity.h"
#include "../src/contactList.h"

List_t contactList;

void setUp(void)
{
    listInit(&contactList);
}

void tearDown(void)
{
    listClear(&contactList);
}

void test_listInit(void)
{
    List_t* testList = NULL;
    testList = listInit(testList);
    TEST_ASSERT_NOT_NULL(testList);
    TEST_ASSERT_NULL(testList->head);
    TEST_ASSERT_NULL(testList->tail);
    TEST_ASSERT_EQUAL(0, testList->length);
    free(testList);
}

void test_listClear(void)
{
    TEST_ASSERT_NULL(listClear(NULL));
    List_t* cleared = listClear(&contactList);
    TEST_ASSERT_NOT_NULL(cleared);
    TEST_ASSERT_EQUAL_PTR(&contactList, cleared);

    Item_t* item = (Item_t*)malloc(sizeof(Item_t));
    contactList.head = contactList.tail = item;
    contactList.length = 1;

    TEST_ASSERT_NULL(listClear(&contactList));

    item->contact = (Contact_t*)malloc(sizeof(Contact_t));
    contactList.head = contactList.tail = item;
    contactList.length = 1;

    TEST_ASSERT_NOT_NULL(listClear(&contactList));

    TEST_ASSERT_EQUAL(0, contactList.length);
}

void test_listGetAt(void)
{
    TEST_ASSERT_NULL(listGetAt(NULL, 0));
    TEST_ASSERT_NULL(listGetAt(&contactList, 0));

    Item_t* item1 = (Item_t*)malloc(sizeof(Item_t));
    Item_t* item2 = (Item_t*)malloc(sizeof(Item_t));

    contactList.head = item1;
    item1->next = item2;
    item2->prev = NULL;
    contactList.tail = item2;
    item2->next = NULL;
    item1->prev = item1;
    contactList.length = 2;

    Item_t* got = listGetAt(&contactList, 0);
    TEST_ASSERT_NOT_NULL(got);
    TEST_ASSERT_EQUAL_PTR(item1, got);
    got = listGetAt(&contactList, 1);
    TEST_ASSERT_NOT_NULL(got);
    TEST_ASSERT_EQUAL_PTR(item2, got);

    Item_t* item3 = (Item_t*)malloc(sizeof(Item_t));
    item1->next = item3;
    item2->prev = item3;
    item3->prev = item1;
    item3->next = item2;
    contactList.length = 3;

    got = listGetAt(&contactList, 1);
    TEST_ASSERT_NOT_NULL(got);
    TEST_ASSERT_EQUAL_PTR(item3, got);
}

void test_listAddSorted(void)
{
    TEST_MESSAGE("Here0");
    Contact_t* contact = newContact(
        "1LastName", "1FirstName", "1Patronim",
        "1Company", "1Position",
        "1234567890", "1098765432", "123456", "124680",
        NULL, NULL
    );
    TEST_MESSAGE("Here1");
    while (contactAddEmail(contact, "1@1.ru"));
    while (contactAddSocial(contact, "1.ru/id1"));
    TEST_MESSAGE("Here2");
    TEST_ASSERT_NULL(listAddSorted(NULL, contact));
    TEST_ASSERT_NULL(listAddSorted(&contactList, NULL));

    contactList.length = MAX_CONTACTS_COUNT;
    TEST_ASSERT_NULL(listAddSorted(&contactList, contact));
    contactList.length = 0;

    Item_t* added = listAddSorted(&contactList, contact);
    TEST_ASSERT_NOT_NULL(added);
    TEST_ASSERT_EQUAL_PTR(contact, added->contact);
    TEST_ASSERT_EQUAL_PTR(contact, contactList.head->contact);
    TEST_ASSERT_EQUAL_PTR(contact, contactList.tail->contact);
    TEST_ASSERT_EQUAL(1, contactList.length);

    contact = newContact(
        "3LastName", "3FirstName", "3Patronim",
        "3Company", "3Position",
        "3234567890", "3098765432", "323456", "324680",
        NULL, NULL
    );
    while (contactAddEmail(contact, "3@3.ru"));
    while (contactAddSocial(contact, "3.ru/id3"));

    added = listAddSorted(&contactList, contact);
    TEST_ASSERT_NOT_NULL(added);
    TEST_ASSERT_EQUAL_PTR(contact, added->contact);
    TEST_ASSERT_EQUAL_PTR(contact, contactList.tail->contact);
    TEST_ASSERT_EQUAL(2, contactList.length);

    contact = newContact(
        "2LastName", "2FirstName", "2Patronim",
        "2Company", "2Position",
        "2234567890", "2098765432", "223456", "224680",
        NULL, NULL
    );
    while (contactAddEmail(contact, "2@2.ru"));
    while (contactAddSocial(contact, "2.ru/id2"));

    added = listAddSorted(&contactList, contact);
    TEST_ASSERT_NOT_NULL(added);
    TEST_ASSERT_EQUAL_PTR(contact, added->contact);
    TEST_ASSERT_EQUAL_PTR(contact, contactList.head->next);
    TEST_ASSERT_EQUAL(3, contactList.length);
}

void test_listAddCreate(void)
{
    TEST_ASSERT_NULL(listAddCreate(NULL, "", "", "", "", "", "", "", "", "", NULL, NULL));

    TEST_ASSERT_NULL(listAddCreate(&contactList, NULL, "1", "", "", "", "", "", "", "", NULL, NULL));
    TEST_ASSERT_NULL(listAddCreate(&contactList, "1", NULL, "", "", "", "", "", "", "", NULL, NULL));
    TEST_ASSERT_NULL(listAddCreate(&contactList, "1", "", "", "", "", "", "", "", "", NULL, NULL));
    TEST_ASSERT_NULL(listAddCreate(&contactList, "", "1", "", "", "", "", "", "", "", NULL, NULL));

    contactList.length = MAX_CONTACTS_COUNT;
    TEST_ASSERT_NULL(listAddCreate(&contactList, "1", "1", "", "", "", "", "", "", "", NULL, NULL));
    contactList.length = 0;

    Item_t* created = listAddCreate(&contactList, "1", "1", "", "", "", "", "", "", "", NULL, NULL);
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_NULL(created->contact);
    TEST_ASSERT_EQUAL_PTR(contactList.head, created);
    TEST_ASSERT_EQUAL(1, contactList.length);

    created = listAddCreate(&contactList, "2", "2", "", "", "", "", "", "", "", NULL, NULL);
    TEST_ASSERT_NOT_NULL(created);
    TEST_ASSERT_NOT_NULL(created->contact);
    TEST_ASSERT_EQUAL_PTR(contactList.tail, created);
    TEST_ASSERT_EQUAL(2, contactList.length);
}

void test_listEdit(void)
{
    Contact_t* contact1 = newContact(
        "1LastName", "1FirstName", "1Patronim",
        "1Company", "1Position",
        "1234567890", "1098765432", "123456", "124680",
        NULL, NULL
    );
    while (contactAddEmail(contact1, "1@1.ru"));
    while (contactAddSocial(contact1, "1.ru/id1"));
    Item_t* item1 = listAddSorted(&contactList, contact1);

    Contact_t* contact3 = newContact(
        "3LastName", "3FirstName", "3Patronim",
        "3Company", "3Position",
        "3234567890", "3098765432", "323456", "324680",
        NULL, NULL
    );
    while (contactAddEmail(contact1, "3@3.ru"));
    while (contactAddSocial(contact1, "3.ru/id3"));
    Item_t* item3 = listAddSorted(&contactList, contact3);

    Contact_t* contact2 = newContact(
        "4LastName", "4FirstName", "4Patronim",
        "4Company", "4Position",
        "4234567890", "4098765432", "423456", "424680",
        NULL, NULL
    );
    while (contactAddEmail(contact1, "4@4.ru"));
    while (contactAddSocial(contact1, "4.ru/id4"));
    Item_t* item2 = listAddSorted(&contactList, contact2);

    TEST_ASSERT_NULL(listEdit(NULL, item2,
        "2LastName", "2FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    ));

    TEST_ASSERT_NULL(listEdit(&contactList, NULL,
        "2LastName", "2FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    ));

    TEST_ASSERT_NULL(listEdit(&contactList, item2,
        "2LastName", "2FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    ));

    Item_t* edited = listEdit(&contactList, item2,
        "2LastName", "2FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );
    TEST_ASSERT_NOT_NULL(edited);
    TEST_ASSERT_EQUAL_PTR(item2, edited);

    TEST_ASSERT_EQUAL_STRING("2LastName", item2->contact->lastName);
    TEST_ASSERT_EQUAL_STRING("2FirstName", item2->contact->firstName);

    // Проверка на пересортировку
    TEST_ASSERT_NOT_NULL(item1->next);
    TEST_ASSERT_EQUAL_PTR(item1->next, item2);
    TEST_ASSERT_NOT_NULL(item3->prev);
    TEST_ASSERT_EQUAL_PTR(item3->prev, item2);
}

void test_listRemoveAt(void)
{
    TEST_ASSERT_NULL(listRemoveAt(NULL, 0));
    TEST_ASSERT_NULL(listRemoveAt(&contactList, 0));

    Item_t* contact1 = listAddCreate(&contactList,
        "1LastName", "1FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );

    Contact_t* removed = listRemoveAt(&contactList, 0);
    TEST_ASSERT_NOT_NULL(removed);
    TEST_ASSERT_EQUAL_PTR(contact1->contact, removed);
    TEST_ASSERT_NULL(contactList.head);
    TEST_ASSERT_NULL(contactList.tail);
    TEST_ASSERT_EQUAL(0, contactList.length);

    contact1 = listAddSorted(&contactList, contact1->contact);
    Item_t* contact2 = listAddCreate(&contactList,
        "2LastName", "2FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );
    Item_t* contact3 = listAddCreate(&contactList,
        "3LastName", "3FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );
    Item_t* contact4 = listAddCreate(&contactList,
        "4LastName", "4FirstName", NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    );

    // Порядок: 0:contact1, 1:contact2, 2:contact3, 3:contact4
    // Удаление из середины (останутся contact1, contact3, contact4)
    removed = listRemoveAt(&contactList, 1);
    TEST_ASSERT_NOT_NULL(removed);
    TEST_ASSERT_EQUAL_PTR(contact2->contact, removed);
    TEST_ASSERT_EQUAL_PTR(contactList.head->next, contact3);
    TEST_ASSERT_EQUAL_PTR(contactList.tail->prev, contact2);
    TEST_ASSERT_EQUAL(3, contactList.length);

    // Удаление из хвоста (останутся contact1, contact3)
    removed = listRemoveAt(&contactList, 2);
    TEST_ASSERT_NOT_NULL(removed);
    TEST_ASSERT_EQUAL_PTR(contact4->contact, removed);
    TEST_ASSERT_EQUAL_PTR(contactList.head->next, contact3);
    TEST_ASSERT_EQUAL_PTR(contactList.tail, contact3);
    TEST_ASSERT_EQUAL_PTR(contactList.tail->prev, contact1);
    TEST_ASSERT_EQUAL(2, contactList.length);

    // Удаление из головы (останется contact3)
    removed = listRemoveAt(&contactList, 0);
    TEST_ASSERT_NOT_NULL(removed);
    TEST_ASSERT_EQUAL_PTR(contact1->contact, removed);
    TEST_ASSERT_EQUAL_PTR(contactList.head, contact3);
    TEST_ASSERT_EQUAL_PTR(contactList.tail, contact3);
    TEST_ASSERT_NULL(contactList.head->next);
    TEST_ASSERT_NULL(contactList.head->prev);
    TEST_ASSERT_EQUAL(1, contactList.length);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_listInit);
    RUN_TEST(test_listClear);

    RUN_TEST(test_listGetAt);
    RUN_TEST(test_listAddSorted);
    RUN_TEST(test_listEdit);
    RUN_TEST(test_listAddCreate);

    RUN_TEST(test_listRemoveAt);
    return UNITY_END();
}