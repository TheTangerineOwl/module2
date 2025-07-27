#include "contactListIO.h"

// Contact_t* contacts;
size_t contactsCount;

int main(void)
{
    Contact_t* contact;

    char* emails[] = {"test@example.com", "work@company.org", NULL};
    char* socials[] = {"vk.com/test", NULL};

    // Надо доработать ограничения на длину строки
    // при передаче кириллицы (кодировка файла UTF-8).
    // Пока каждая буква киррилицы - два байта.

    contact = newContact(
        "Фамилия", "Имя", "Отчество",
        "Company", "Позиция",
        "1234567890123", "0987654321123", "123456", "123456789012",
        emails, socials
    );

    printContact(*contact);

    char* emails2[] = {"test2@example.com", "work2@company.org", NULL};
    char* socials2[] = {"vk.com/test2", "auberge.com", NULL};

    
    // На данный момент функция editContact, если ей
    // передать emails или socials, сначала стирает
    // предыдущие почты/соцсети, а после добавляет новые,
    // что, собственно, оставляет сохранение предыдущих
    // значений на плечах пользователя и не есть хорошо.

    if (!editContact(
        contact,
        NULL, "ФерстНейм", "Садововна",
        "Eltex", "Техник",
        "123", NULL, "123", NULL,
        socials2, emails2
    ))
        puts("Что-то пошло не так!");

    printContact(*contact);
    
    clearContact(contact);
    free(contact);

    return 0;
}