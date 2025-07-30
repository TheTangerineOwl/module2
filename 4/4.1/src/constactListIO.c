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
            if (!contact.emails[i] || !(contact.emails[i][0]))
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

    for (size_t i = 0; i < MAX_EMAILS_COUNT; i++)
    {
        printf("Введите электронную почту %ld (пустая строка для пропуска): ", i + 1);
        emails = (char**)realloc(emails, sizeof(char*) * (i + 1));
        emails[i] = NULL;
        if (!emails)
            break;
        emails[i] = enterStrField(emails[i], MAX_EMAIL_LENGTH);
        if (!emails[i])
        {
            if (i)
                emails = (char**)realloc(emails, sizeof(char*) * i);
            else
            {
                free(emails);
                emails = NULL;
            }
            break;
        }
    }
    for (size_t i = 0; i < MAX_SOCIALS_COUNT; i++)
    {
        printf("Введите ссылку на соцсети или профиль %ld (пустая строка для пропуска): ", i + 1);
        socialsLink = (char**)realloc(socialsLink, sizeof(char*) * (i + 1));
        socialsLink[i] = NULL;
        if (!socialsLink)
            break;
        socialsLink[i] = enterStrField(socialsLink[i], SOCIALS_LINK_LENGTH);
        if (!socialsLink[i])
        {
            if (i)
                socialsLink = (char**)realloc(socialsLink, sizeof(char*) * i);
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
                                    emails, socialsLink);
    printContact(*contact->contact);
    return contact;
}

Item_t *editContactInfo(List_t *list, Item_t *contact)
{
    Contact_t newValues; // = *contact->contact;
    Contact_t *old = contact->contact;

    int exitFlag = 0;
    if (!contact || !contact->contact)
    {
        // puts("Не удалось редактировать контакт: контакт не найден!");
        // return 0;
        RETURN_NULL_WITH_MSG("Не удалось редактировать контакт: контакт не найден!");
    }
    printf("Редактирование контакта:\n\n");
    printContact(*old);

    copyToContact(
        &newValues,
        old->lastName, old->firstName, old->patronim,
        old->workInfo ? old->workInfo->workPlace : NULL,
        old->workInfo ? old->workInfo->position : NULL,
        old->numbers ? old->numbers->work : NULL,
        old->numbers ? old->numbers->personal : NULL,
        old->numbers ? old->numbers->home : NULL,
        old->numbers ? old->numbers->extra : NULL,
        old->emails, old->socialsLink);

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
            changeField(newValues.lastName, menuItems[LAST_NAME]);
        }
        break;
        case FIRST_NAME:
            changeField(newValues.firstName, menuItems[FIRST_NAME]);
            break;
        case PATRONIM:
            changeField(newValues.patronim, menuItems[PATRONIM]);
            break;
        case WORK_PLACE:
        {
            if (!newValues.workInfo)
            {
                newValues.workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues.workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues.workInfo->workPlace, menuItems[WORK_PLACE]);
        }
        break;
        case POSITION:
        {
            if (!newValues.workInfo)
            {
                newValues.workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues.workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues.workInfo->position, menuItems[POSITION]);
        }
        break;
        case WORK_PHONE:
        {
            if (!newValues.workInfo)
            {
                newValues.workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues.workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues.numbers->work, menuItems[WORK_PHONE]);
        }
        break;
        case PERSONAL_PHONE:
        {
            if (!newValues.workInfo)
            {
                newValues.workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues.workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues.numbers->personal, menuItems[PERSONAL_PHONE]);
        }
        break;
        case HOME_PHONE:
        {
            if (!newValues.workInfo)
            {
                newValues.workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues.workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues.numbers->home, menuItems[HOME_PHONE]);
        }
        break;
        case EXTRA_PHONE:
        {
            if (!newValues.workInfo)
            {
                newValues.workInfo = (Work_t *)malloc(sizeof(Work_t));
                if (!newValues.workInfo)
                {
                    puts("Не удалось выделить память!");
                    continue;
                }
            }
            changeField(newValues.numbers->extra, menuItems[WORK_PHONE]);
        }
        break;
        case EMAIL:
        {
            printf("%s (%lu):\n", menuItems[EMAIL].name, newValues.emailCount);
            for (size_t i = 0; i < newValues.emailCount; i++)
            {
                printf("%lu. %s\n", i + 1, newValues.emails[i] ? newValues.emails[i] : "(нет)");
            }

            printf("\n1. Добавить email;\n2. Удалить email;\n3. Изменить email.\nВыбор: ");
            int emailChoice;
            if (!scanf("%d", &emailChoice) || emailChoice < 1 || emailChoice > 3)
            {
                printf("Некорректный выбор!\n");
                CLEAR_BUFFER();
                break;
            }
            CLEAR_BUFFER();

            if (emailChoice == 1)
            {
                if (newValues.emailCount >= MAX_EMAILS_COUNT)
                {
                    printf("Достигнуто максимальное количество emails!\n");
                    break;
                }

                char *newEmail = enterStrField(NULL, MAX_EMAIL_LENGTH);
                if (!newEmail)
                    break;

                Contact_t *buf = contactAddEmail(&newValues, newEmail);
                if (!buf)
                {
                    printf("Не удалось добавить email!\n");
                    break;
                }
                newValues = *buf;
            }
            else if (emailChoice == 2)
            {
                if (newValues.emailCount == 0)
                {
                    printf("Нечего удалять!\n");
                    break;
                }

                printf("Введите номер email для удаления (1-%lu): ", newValues.emailCount);
                size_t toDelete;
                if (!scanf("%lu", &toDelete) || toDelete < 1 || toDelete > newValues.emailCount)
                {
                    printf("Некорректный ввод!\n");
                    CLEAR_BUFFER();
                    break;
                }
                CLEAR_BUFFER();

                Contact_t *buf = contactDeleteEmail(&newValues, toDelete);
                if (!buf)
                {
                    printf("Не удалось удалить email!\n");
                    break;
                }
                newValues = *buf;
            }
            else if (emailChoice == 3)
            {
                if (newValues.emailCount == 0)
                {
                    printf("Нечего изменять!\n");
                    break;
                }

                printf("Введите номер email для изменения (1-%lu): ", newValues.emailCount);
                size_t toEdit;
                if (!scanf("%lu", &toEdit) || toEdit < 1 || toEdit > newValues.emailCount)
                {
                    printf("Некорректный ввод!\n");
                    CLEAR_BUFFER();
                    break;
                }
                CLEAR_BUFFER();

                char *newEmail = enterStrField(newValues.emails[toEdit - 1], MAX_EMAIL_LENGTH);
                if (newEmail)
                {
                    Contact_t *buf = contactEditEmail(&newValues, toEdit, newEmail);
                    if (!buf)
                    {
                        printf("Не удалось удалить email!\n");
                        break;
                    }
                    newValues = *buf;
                }
            }
            break;
        }

        case SOCIAL_LINK:
        {
            printf("%s (%lu):\n", menuItems[SOCIAL_LINK].name, newValues.socialsCount);
            for (size_t i = 0; i < newValues.socialsCount; i++)
            {
                printf("%lu. %s\n", i + 1, newValues.socialsLink[i] ? newValues.socialsLink[i] : "(нет)");
            }

            printf("\n1. Добавить socials;\n2. Удалить socials;\n3. Изменить socials.\nВыбор: ");
            int socialsChoice;
            if (!scanf("%d", &socialsChoice) || socialsChoice < 1 || socialsChoice > 3)
            {
                printf("Некорректный выбор!\n");
                CLEAR_BUFFER();
                break;
            }
            CLEAR_BUFFER();

            if (socialsChoice == 1)
            {
                if (newValues.socialsCount >= MAX_SOCIALS_COUNT)
                {
                    printf("Достигнуто максимальное количество socialsLink!\n");
                    break;
                }

                char *newSocials = enterStrField(NULL, SOCIALS_LINK_LENGTH);
                if (!newSocials)
                    break;

                Contact_t *buf = contactAddSocial(&newValues, newSocials);
                if (!buf)
                {
                    printf("Не удалось добавить socials!\n");
                    break;
                }
                newValues = *buf;
            }
            else if (socialsChoice == 2)
            {
                if (newValues.socialsCount == 0)
                {
                    printf("Нечего удалять!\n");
                    break;
                }

                printf("Введите номер socials для удаления (1-%lu): ", newValues.socialsCount);
                size_t toDelete;
                if (!scanf("%lu", &toDelete) || toDelete < 1 || toDelete > newValues.socialsCount)
                {
                    printf("Некорректный ввод!\n");
                    CLEAR_BUFFER();
                    break;
                }
                CLEAR_BUFFER();

                Contact_t *buf = contactDeleteSocial(&newValues, toDelete);
                if (!buf)
                {
                    printf("Не удалось удалить socials!\n");
                    break;
                }
                newValues = *buf;
            }
            else if (socialsChoice == 3)
            {
                if (newValues.socialsCount == 0)
                {
                    printf("Нечего изменять!\n");
                    break;
                }

                printf("Введите номер socials для изменения (1-%lu): ", newValues.socialsCount);
                size_t toEdit;
                if (!scanf("%lu", &toEdit) || toEdit < 1 || toEdit > newValues.socialsCount)
                {
                    printf("Некорректный ввод!\n");
                    CLEAR_BUFFER();
                    break;
                }
                CLEAR_BUFFER();

                char *newSocials = enterStrField(newValues.socialsLink[toEdit - 1], MAX_EMAIL_LENGTH);
                if (newSocials)
                {
                    Contact_t *buf = contactEditSocial(&newValues, toEdit, newSocials);
                    if (!buf)
                    {
                        printf("Не удалось удалить socials!\n");
                        break;
                    }
                    newValues = *buf;
                }
            }
            break;
        }
        case SOCIAL_LINK + 1:
        {
            puts("Значения до изменения:");
            printContact(*contact->contact);
            puts("После изменения:");
            printContact(newValues);
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
                    contact, newValues.lastName,
                    newValues.firstName,
                    newValues.patronim,
                    newValues.workInfo ? newValues.workInfo->workPlace : NULL,
                    newValues.workInfo ? newValues.workInfo->position : NULL,
                    newValues.numbers ? newValues.numbers->work : NULL,
                    newValues.numbers ? newValues.numbers->personal : NULL,
                    newValues.numbers ? newValues.numbers->home : NULL,
                    newValues.numbers ? newValues.numbers->extra : NULL,
                    newValues.emails, newValues.socialsLink);
                if (!buf)
                    RETURN_NULL_WITH_MSG("Не удалось изменить контакт!\n");
                contact = buf;
            }
            //*contact = newValues;
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
        printContact(newValues);
    }
    return contact;
}

char *changeField(char *oldField, const ContactField fieldAttr)
{
    printf("Редактирование поля \"%s\".\n", fieldAttr.name);
    printf("Старое значение: %s\n", oldField);
    printf("Введите новое значение: ");
    // if (!enterStrField(oldField, field.fieldLength))
    //     RETURN_NULL_WITH_MSG("Не удалось изменить поле!");
    oldField = enterStrField(oldField, fieldAttr.fieldLength);
    if (!oldField && !fieldAttr.canBeNull)
    {
        printf("Поле \"%s\" должно быть заполнено!\n", fieldAttr.name);
        // CLEAR_BUFFER();
        printf("Введите фамилию: ");
        oldField = enterStrField(oldField, fieldAttr.fieldLength);
    }
    return oldField;
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