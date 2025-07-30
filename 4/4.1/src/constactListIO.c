#include "contactListIO.h"

#define CLEAR_BUFFER()                          \
    while ((c = getchar()) != '\n' && c != EOF);
#define PRINT_FIELD_IFNZERO(field, fieldName) \
    if (field && field[0] != 0)               \
    printf("%s: %s\n", fieldName, field)
char c;

char *enterStrField(char *field, size_t fieldLength)
{
    char *buffer = (char *)malloc(sizeof(char) * fieldLength);
    if (!buffer)
    {
        CLEAR_BUFFER();
        RETURN_NULL_WITH_MSG("Не удалось выделить память под строку!");
    }
    if (!fgets(buffer, fieldLength - 1, stdin))
    {
        free(buffer);
        buffer = NULL;
        CLEAR_BUFFER();
        RETURN_NULL_WITH_MSG("Не удалось прочитать строку из потока ввода!");
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    size_t size = strlen(buffer);
    if (field)
        field = (char *)realloc(field, sizeof(char) * (size + 1));
    else
        field = (char *)malloc(sizeof(char) * (size + 1));
    strncpy(field, buffer, size);
    field[size] = '\0';
    if (!strlen(field))
    {
        free(field);
        field = NULL;
    }
    if (strlen(buffer) == fieldLength - 1)
        CLEAR_BUFFER();
    free(buffer);
    buffer = NULL;
    return field;
}

void printContact(const Contact_t contact)
{
    printf("%s: %s\n", menuItems[LAST_NAME].name, contact.lastName);
    printf("%s: %s\n", menuItems[FIRST_NAME].name, contact.firstName);
    PRINT_FIELD_IFNZERO(contact.patronim, menuItems[PATRONIM].name);

    if (contact.workInfo)
    {
        PRINT_FIELD_IFNZERO(contact.workInfo->workPlace, menuItems[WORK_PLACE].name);
        PRINT_FIELD_IFNZERO(contact.workInfo->position, menuItems[POSITION].name);
    }

    if (contact.numbers)
    {
        PRINT_FIELD_IFNZERO(contact.numbers->work, menuItems[WORK_PHONE].name);
        PRINT_FIELD_IFNZERO(contact.numbers->personal, menuItems[PERSONAL_PHONE].name);
        PRINT_FIELD_IFNZERO(contact.numbers->home, menuItems[HOME_PHONE].name);
        PRINT_FIELD_IFNZERO(contact.numbers->extra, menuItems[EXTRA_PHONE].name);
    }

    if (contact.emails)
    {
        for (size_t i = 0; i < contact.emailCount; i++)
        {
            if (!contact.emails[i] || !(contact.emails[i][0]))
                break;
            printf("%s %lu: %s\n", menuItems[EMAIL].name, i + 1, contact.emails[i]);
        }
    }

    if (contact.socialsLink)
    {
        for (size_t i = 0; i < contact.socialsCount; i++)
        {
            if (!contact.socialsLink[i] || !(contact.socialsLink[i][0]))
                break;
            printf("%s %lu: %s\n", menuItems[SOCIAL_LINK].name, i + 1, contact.socialsLink[i]);
        }
    }
}

void printAllContacts(const List_t list)
{
    if (list.length == 0)
        puts("Нет контактов!");
    Item_t *item = list.head;
    size_t i = 0;
    while (item && item->contact && i < list.length)
    {
        printf("%lu. ", ++i);
        printContact(*item->contact);
        item = item->next;
    }
}

void printShortContacts(const List_t list)
{
    if (list.length == 0)
        puts("Нет контактов!");
    Item_t *item = list.head;
    size_t i = 0;
    while (item && item->contact && i < list.length)
    {
        if (item->contact->numbers)
            printf("%lu. %s %s %s\n", ++i,
                   item->contact->lastName, item->contact->firstName,
                   item->contact->numbers ? item->contact->numbers->personal : NULL);
        else
            printf("%lu. %s %s\n", ++i,
                   item->contact->lastName, item->contact->firstName);
        item = item->next;
    }
}

Item_t *enterContactInfo(List_t *list)
{
    if (list->length + 1 > MAX_CONTACTS_COUNT)
        RETURN_NULL_WITH_MSG("Нельзя добавить контакт!\n");

    char *lastName = NULL, *firstName = NULL, *patronim = NULL,
         *work = NULL, *personal = NULL, *home = NULL, *extra = NULL,
         *workPlace = NULL, *position = NULL, **emails = NULL, **socialsLink = NULL;

    printf("Введите фамилию: ");
    lastName = enterStrField(lastName, LAST_NAME_LENGTH);
    while (!lastName)
    {
        puts("Поле \"фамилия\" должно быть заполнено!");
        // CLEAR_BUFFER();
        printf("Введите фамилию: ");
        lastName = enterStrField(lastName, LAST_NAME_LENGTH);
    }
    printf("Введите имя: ");
    firstName = enterStrField(firstName, FIRST_NAME_LENGTH);
    while (!firstName)
    {
        puts("Поле \"имя\" должно быть заполнено!");
        // CLEAR_BUFFER();
        printf("Введите имя: ");
        firstName = enterStrField(firstName, LAST_NAME_LENGTH);
    }

    printf("Введите отчество (пустая строка для пропуска): ");
    patronim = enterStrField(patronim, PATRONIM_LENGTH);
    printf("Введите место работы (пустая строка для пропуска): ");
    workPlace = enterStrField(workPlace, COMPANY_NAME_LENGTH);
    printf("Введите должность (пустая строка для пропуска): ");
    position = enterStrField(position, POSITION_LENGTH);
    printf("Введите рабочий телефон (пустая строка для пропуска): ");
    work = enterStrField(work, PHONE_NUMBER_LENGTH);
    printf("Введите личный телефон (пустая строка для пропуска): ");
    personal = enterStrField(personal, PHONE_NUMBER_LENGTH);
    printf("Введите домашний телефон (пустая строка для пропуска): ");
    home = enterStrField(home, PHONE_NUMBER_LENGTH);
    printf("Введите дополнительный телефон (пустая строка для пропуска): ");
    extra = enterStrField(extra, PHONE_NUMBER_LENGTH);

    emails = (char **)malloc(sizeof(char *));
    socialsLink = (char **)malloc(sizeof(char *));
    if (!emails || !socialsLink)
        return NULL;

    size_t emailCount = 0;
    for (; emailCount < MAX_EMAILS_COUNT; emailCount++)
    {
        printf("Введите электронную почту %ld (пустая строка для пропуска): ", emailCount + 1);
        emails = (char**)realloc(emails, sizeof(char*) * (emailCount + 1));
        emails[emailCount] = NULL;
        if (!emails)
            break;
        emails[emailCount] = enterStrField(emails[emailCount], MAX_EMAIL_LENGTH);
        if (!emails[emailCount])
        {
            if (emailCount)
                emails = (char**)realloc(emails, sizeof(char*) * emailCount);
            else
            {
                free(emails);
                emails = NULL;
            }
            break;
        }
    }
    size_t socialsCount = 0;
    for (; socialsCount < MAX_SOCIALS_COUNT; socialsCount++)
    {
        printf("Введите ссылку на соцсети или профиль %ld (пустая строка для пропуска): ", socialsCount + 1);
        socialsLink = (char**)realloc(socialsLink, sizeof(char*) * (socialsCount + 1));
        socialsLink[socialsCount] = NULL;
        if (!socialsLink)
            break;
        socialsLink[socialsCount] = enterStrField(socialsLink[socialsCount], SOCIALS_LINK_LENGTH);
        if (!socialsLink[socialsCount])
        {
            if (socialsCount)
                socialsLink = (char**)realloc(socialsLink, sizeof(char*) * socialsCount);
            else
            {
                free(socialsLink);
                socialsLink = NULL;
            }
            break;
        }
    }

    Item_t *contact = listAddCreate(list,
                                    lastName, firstName, patronim,
                                    workPlace, position,
                                    work, personal, home, extra,
                                    emailCount, emails, socialsCount, socialsLink);
    printContact(*contact->contact);
    return contact;
}

Item_t *editContactInfo(List_t *list, Item_t *contact)
{
    Contact_t* newValues; // = *contact->contact;
    Contact_t *old = contact->contact;

    int exitFlag = 0;
    if (!contact || !contact->contact)
    {
        // puts("Не удалось редактировать контакт: контакт не найден!");
        // return 0;
        RETURN_NULL_WITH_MSG("Не удалось редактировать контакт: контакт не найден!\n");
    }
    printf("Редактирование контакта:\n\n");
    printContact(*old);

    newValues = createCopy(contact->contact);

    if (!newValues)
        RETURN_NULL_WITH_MSG("Не удалось редактировать контакт: не удалось создать резервную копию!\n");

    int choice;
    while (!exitFlag)
    {
        printf("Выберите поле для редактирования:\n");

        for (size_t i = 0; i < menuCount; i++)
        {
            printf("%ld. %s\n", i + 1, menuItems[i].name);
        }
        printf("%ld. Выход из режима редактирования.\n Ваш выбор: ", menuCount + 1);

        if (!scanf("%d", &choice) || choice < 1 || (size_t)choice > menuCount + 1)
        {
            printf("Некорректный выбор! Попробуйте еще раз.\n");
            CLEAR_BUFFER();
            continue;
        }
        CLEAR_BUFFER();

        choice--;

        switch (choice)
        {
        case LAST_NAME:
        {
            changeField(newValues->lastName, menuItems[LAST_NAME]);
        }
        break;
        case FIRST_NAME:
            changeField(newValues->firstName, menuItems[FIRST_NAME]);
            break;
        case PATRONIM:
            changeField(newValues->patronim, menuItems[PATRONIM]);
            break;
        case WORK_PLACE:
        {
            if (!newValues->workInfo)
            {
                newValues->workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues->workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues->workInfo->workPlace, menuItems[WORK_PLACE]);
        }
        break;
        case POSITION:
        {
            if (!newValues->workInfo)
            {
                newValues->workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues->workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues->workInfo->position, menuItems[POSITION]);
        }
        break;
        case WORK_PHONE:
        {
            if (!newValues->workInfo)
            {
                newValues->workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues->workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues->numbers->work, menuItems[WORK_PHONE]);
        }
        break;
        case PERSONAL_PHONE:
        {
            if (!newValues->workInfo)
            {
                newValues->workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues->workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues->numbers->personal, menuItems[PERSONAL_PHONE]);
        }
        break;
        case HOME_PHONE:
        {
            if (!newValues->workInfo)
            {
                newValues->workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues->workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues->numbers->home, menuItems[HOME_PHONE]);
        }
        break;
        case EXTRA_PHONE:
        {
            if (!newValues->workInfo)
            {
                newValues->workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues->workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues->numbers->extra, menuItems[WORK_PHONE]);
        }
        break;
        case EMAIL:
        {
            newValues = editListField(newValues, menuItems[EMAIL]);
            if (!newValues)
            {
                puts("Не удалось изменить поле!");
                continue;
            }
        }
        break;
        case SOCIAL_LINK:
        {
            newValues = editListField(newValues, menuItems[SOCIAL_LINK]);
            if (!newValues)
            {
                puts("Не удалось изменить поле!");
                continue;
            }
        }
        break;
        case SOCIAL_LINK + 1:
        {
            puts("Значения до изменения:");
            printContact(*contact->contact);
            puts("После изменения:");
            printContact(*newValues);
            printf("Закончить редактирование (0 - нет, 1 - да, 2 - отменить изменения)? Выбор: ");

            while (!scanf("%d", &choice) || choice < 0 || choice > 2)
            {
                printf("Некорректный выбор! Попробуйте еще раз.\n");
                CLEAR_BUFFER();
            }
            CLEAR_BUFFER();
            if (choice == 0)
                continue;
            if (choice == 1)
            {
                Item_t *buf = listEdit(
                    list,
                    contact, newValues->lastName,
                    newValues->firstName,
                    newValues->patronim,
                    newValues->workInfo ? newValues->workInfo->workPlace : NULL,
                    newValues->workInfo ? newValues->workInfo->position : NULL,
                    newValues->numbers ? newValues->numbers->work : NULL,
                    newValues->numbers ? newValues->numbers->personal : NULL,
                    newValues->numbers ? newValues->numbers->home : NULL,
                    newValues->numbers ? newValues->numbers->extra : NULL,
                    newValues->emailCount, newValues->emails,
                    newValues->socialsCount, newValues->socialsLink);
                // free(newValues);
                if (!buf)
                {
                    clearContact(newValues);
                    free(newValues);
                    newValues = NULL;
                    RETURN_NULL_WITH_MSG("Не удалось изменить контакт!\n");
                }
                contact = buf;
            }
            //*contact = newValues;
            // free(newValues);
            // newValues = NULL;
            exitFlag = 1;
        }
        break;
        default:
        {
            puts("Некорректный ввод! Попробуйте еще раз.");
            CLEAR_BUFFER();
            continue;
        }
        }
        puts("Новые значения:");
        if (exitFlag)
            printContact(*contact->contact);
        else
            printContact(*newValues);
    }
    clearContact(newValues);
    free(newValues);
    newValues = NULL;
    return contact;
}

char *changeField(char *oldField, const ContactField fieldAttr)
{
    printf("Редактирование поля \"%s\".\n", fieldAttr.name);
    printf("Старое значение: %s\n", oldField);
    printf("Введите новое значение: ");
    oldField = enterStrField(oldField, fieldAttr.fieldLength);
    if (!oldField && !fieldAttr.canBeNull)
    {
        printf("Поле \"%s\" должно быть заполнено!\n", fieldAttr.name);
        printf("Введите фамилию: ");
        oldField = enterStrField(oldField, fieldAttr.fieldLength);
    }
    return oldField;
}

Contact_t* editListField(
    Contact_t* contact,
    const ContactField fieldAttr
)
{
    if (!contact)
        return NULL;

    Contact_t* reserveCopy = createCopy(contact);

    char** listField;
    size_t subfieldCount;
    Contact_t* (*addToListField)(Contact_t* contact, const char* newValue);
    Contact_t* (*editAtListField)(Contact_t* contact, const size_t index, const char* newValue);
    Contact_t* (*deleteAtListField)(Contact_t* contact, const size_t index);
    if (fieldAttr.type == EMAIL)
    {
        listField = reserveCopy->emails;
        subfieldCount = reserveCopy->emailCount;
        addToListField = contactAddEmail;
        editAtListField = contactEditEmail;
        deleteAtListField = contactDeleteEmail;
    }
    else if (fieldAttr.type == SOCIAL_LINK)
    {
        listField = reserveCopy->socialsLink;
        subfieldCount = reserveCopy->socialsCount;
        addToListField = contactAddSocial;
        editAtListField = contactEditSocial;
        deleteAtListField = contactDeleteSocial;
    }
    else
    {
        clearContact(reserveCopy);
        free(reserveCopy);
        reserveCopy = NULL;
        RETURN_NULL_WITH_MSG("editListField: некорректный параметр fieldAttr!\n");
    }

    bool exitSubFlag = false;
    while (!exitSubFlag)
    {
        printf("%s (%lu):\n", fieldAttr.name, subfieldCount);

        puts("Here");
        fflush(stdout);
        printf("%lu %lu %s\n", reserveCopy->emailCount, subfieldCount, listField[subfieldCount - 1]);
        puts("Here1");
        fflush(stdout);

        if (listField)
            for (size_t i = 0; i < subfieldCount; i++)
            {
                printf("%lu. %s\n", i + 1, listField[i] ? listField[i] : "(нет)");
            }

        printf("\n%s:\n1. добавить;\n"
               "2. удалить;\n"
               "3. изменить;\n"
               "4. Выход обратно в меню редактирования.\n"
               "Выбор: ",
               fieldAttr.name);
        int choice;
        if (!scanf("%d", &choice) || choice < 1 || choice > 4)
        {
            printf("Некорректный выбор!\n");
            CLEAR_BUFFER();
            break;
        }
        CLEAR_BUFFER();

        switch (choice)
        {
            case 1:  // добавить
            {
                if (subfieldCount >= fieldAttr.itemCount)
                {
                    printf("Достигнуто максимальное количество!\n");
                    break;
                }
                printf("Введите новое значение (пустая строка для выхода): ");
                char *newValue = enterStrField(NULL, fieldAttr.fieldLength);
                if (!newValue)
                    break;
                Contact_t *buf = addToListField(reserveCopy, newValue);
                if (!buf)
                {
                    printf("Не удалось добавить!\n");
                    break;
                }
                reserveCopy = buf;
                // Из-за realloc внутри обоих contactAdd...() надо перезаписывать
                // указатель на поле, хотя указатель на контакт и тот же
                if (fieldAttr.type == EMAIL)
                    listField = reserveCopy->emails;
                else if (fieldAttr.type == SOCIAL_LINK)
                    listField = reserveCopy->socialsLink;
                subfieldCount++;
            }
            break;
            case 2:  // удалить
            {
                if (subfieldCount == 0)
                {
                    printf("Нечего удалять!\n");
                    break;
                }

                printf("Введите номер для удаления (1-%lu): ", subfieldCount);
                size_t toDelete;
                if (!scanf("%lu", &toDelete) || toDelete < 1 || toDelete > subfieldCount)
                {
                    printf("Некорректный ввод!\n");
                    CLEAR_BUFFER();
                    break;
                }
                CLEAR_BUFFER();

                Contact_t *buf = deleteAtListField(reserveCopy, toDelete - 1);
                if (!buf)
                {
                    printf("Не удалось удалить!\n");
                    break;
                }
                reserveCopy = buf;
                if (fieldAttr.type == EMAIL)
                    listField = reserveCopy->emails;
                else if (fieldAttr.type == SOCIAL_LINK)
                    listField = reserveCopy->socialsLink;
                subfieldCount--;
            }
            break;
            case 3:  // изменить
            {
                if (subfieldCount == 0)
                {
                    printf("Нечего изменять!\n");
                    break;
                }

                printf("Введите номер для изменения (1-%lu): ", subfieldCount);
                size_t toEdit;
                if (!scanf("%lu", &toEdit) || toEdit < 1 || toEdit > subfieldCount)
                {
                    printf("Некорректный ввод!\n");
                    CLEAR_BUFFER();
                    break;
                }
                CLEAR_BUFFER();
                printf("Введите новое значение (пустая строка для выхода): ");
                char *newValue = enterStrField(listField[toEdit - 1], fieldAttr.fieldLength);
                if (newValue)
                {
                    Contact_t *buf = editAtListField(reserveCopy, toEdit - 1, newValue);
                    if (!buf)
                    {
                        printf("Не удалось удалить!\n");
                        break;
                    }
                    reserveCopy = buf;
                }
            }
            break;
            case 4:  // выход
            {
                printf("%s (%lu):\n", fieldAttr.name, subfieldCount);

                if (listField)
                    for (size_t i = 0; i < subfieldCount; i++)
                    {
                        printf("%lu. %s\n", i + 1, listField[i] ? listField[i] : "(нет)");
                    }

                printf("Уверены, что хотите выйти? (0 - отмена, 1 - да, 2 - отменить изменения): ");

                int choice1;
                if (!scanf("%d", &choice1) || choice1 < 0 || choice1 > 2)
                {
                    printf("Некорректный выбор!\n");
                    CLEAR_BUFFER();
                    break;
                }

                if (!choice1)
                    continue;
                else if (choice1 == 1)
                {
                    if (fieldAttr.type == EMAIL)
                    {
                        while (deleteAtListField(contact, 0));
                        free(contact->emails);
                        contact->emailCount = subfieldCount;
                        contact->emails = listField;
                        reserveCopy->emails = NULL;
                    }
                    else if (fieldAttr.type == SOCIAL_LINK)
                    {
                        while (deleteAtListField(contact, 0));
                        free(contact->socialsLink);
                        contact->socialsCount = subfieldCount;
                        contact->socialsLink = listField;
                        reserveCopy->socialsLink = NULL;
                    }
                }
                exitSubFlag = true;
            }
            break;
            default:
            {
                puts("Некорректный выбор! Попробуйте еще раз.");
                CLEAR_BUFFER();
                continue;
            }
            break;
        }
    }
    clearContact(reserveCopy);
    free(reserveCopy);
    reserveCopy = NULL;
    return contact;
}

Item_t *chooseContact(const List_t list)
{
    puts("Все контакты:");
    printShortContacts(list);
    if (!list.length)
        return NULL;
    printf("Выберите индекс контакта: ");
    size_t choice;

    while (!scanf("%ld", &choice) || choice < 1 || choice > list.length)
    {
        printf("Некорректный выбор! Попробуйте еще раз.\n");
        CLEAR_BUFFER();
    }
    CLEAR_BUFFER();
    return listGetAt(list, choice - 1);
}