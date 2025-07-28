#include "contactListIO.h"

List_t contactList;
size_t contactsCount = 0;

int main(void)
{
    if (!listInit(&contactList))
    {
        fprintf(stderr, "Не удалось создать список!\n");
        return 1;
    }

    char* emails[] = {"test@example.com", "work@company.org", NULL};
    char* socials[] = {"vk.com/test", NULL};

    if (!listAddCreate(
        &contactList,
        "Fамилия", "Имя", "Отчество",
        "Company", "Позиция",
        "1234567890123", "0987654321123", "123456", "123456789012",
        emails, socials
    ))
    {
        fprintf(stderr, "Не удалось добавить новый контакт!\n");
        return 1;
    }

    printContact(*contactList.head->contact);

    char* emails2[] = {"test2@example.com", "work2@company.org", NULL};
    char* socials2[] = {"vk.com/test2", "auberge.com", NULL};


    if (!listAddCreate(
        &contactList,
        "Dамилия2", "Имя2", "Отчество2",
        "Company2", "Позиция2",
        "12345678901232", "0987654321123", "123456", "123456789012",
        emails2, socials2
    ))
    {
        fprintf(stderr, "Не удалось добавить новый 2 контакт!\n");
        return 1;
    }

    printf("%s %s | %s %s\n",
        contactList.head->contact->lastName,
        contactList.head->contact->firstName,
        contactList.tail->contact->lastName,
        contactList.tail->contact->firstName
    );

    Item_t* toEdit = listGetAt(&contactList, 1);

    if (!toEdit || !toEdit->contact)
    {
        fprintf(stderr, "Не удалось получить контакт 2!\n");
        return 1;
    }

    if (!editContact(
        toEdit->contact,
        "NewLastName", NULL, NULL,
        NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL
    ))
    {
        fprintf(stderr, "Не удалось отредактировать контакт 2!\n");
        return 1;
    }

    if (!listAddCreate(
        &contactList,
        "Zамилия2", "Имя2", "Отчество2",
        "Company2", "Позиция2",
        "12345678901232", "0987654321123", "123456", "123456789012",
        emails2, socials2
    ))
    {
        fprintf(stderr, "Не удалось добавить новый 3 контакт!\n");
        return 1;
    }

    printf("%s %s | %s %s | %s %s\n",
        contactList.head->contact->lastName,
        contactList.head->contact->firstName,
        contactList.tail->prev->contact->lastName,
        contactList.tail->prev->contact->firstName,
        contactList.tail->contact->lastName,
        contactList.tail->contact->firstName
    );

    if (!listClear(&contactList))
    {
        fprintf(stderr, "Не удалось очистить список!\n");
        return 1;
    }

    puts("Cleared");

    return 0;
}