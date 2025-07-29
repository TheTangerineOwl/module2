#include "contactList.h"

extern List_t contactList;
// extern size_t contactsCount;

List_t* listInit(List_t* list)
{
    List_t* bufList = (List_t*)malloc(sizeof(List_t));
    if (!bufList)
        return NULL;
    list = bufList;
    // list = (List_t*)memset(list, 0, sizeof(List_t));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

Item_t* listGetAt(List_t* list, const size_t index)
{
    if (!list || index >= list->length)
        return NULL;
    Item_t* item;
    if (index < list->length / 2)
    {
        item = list->head;
        for (size_t i = 0; i < index; i++)
            item = item->next;
    }
    else
    {
        item = list->tail;
        for (size_t i = list->length - 1; i > index; i--)
            item = item->prev;
    }
    return item;
}

Item_t* listAddCreate(
    List_t* list,
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
)
{
    if (!list)
        return NULL;
    Contact_t* contact = newContact(
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emails, socials
    );
    if (!contact)
        return NULL;
    Item_t* res = listAddSorted(list, contact);
    if (res)
        return res;
    clearContact(contact);
    free(contact);
    contact = NULL;
    res = NULL;
    return NULL;
}

static Item_t* listPlaceSorted(List_t* list, Item_t* newItem)
{
    Item_t* forward = list->head;
    if (!forward)
    {
        list->head = list->tail = newItem;
        list->length++;
        return newItem;
    }
    Item_t* backward = list->tail, *prev = list->tail;
    for (size_t i = 0; i < list->length / 2.0; i++)
    {
        if (!forward || !backward)
            break;
        if (compareContacts(*forward->contact, *newItem->contact) > 0)
        {
            prev = forward->prev;  // надо добавить между предыдущим и этим, поэтому берем предыдущий
            break;
        }
        if (compareContacts(*backward->contact, *newItem->contact) <= 0)
        {
            prev = backward;  // надо добавить между этим и следующим (новый prev для newItem)
            break;
        }
        forward = forward->next;
        backward = backward->prev;
    }
    if (!prev)  // forward->prev или backward->prev было NULL, то есть добавить надо в head
    {
        newItem->next = list->head;
        list->head = newItem;
        list->length++;
        return newItem;
    }
    if (!prev->next)
        list->tail = newItem;
    newItem->prev = prev;
    newItem->next = prev->next;
    prev->next = newItem;
    return newItem;
}

Item_t* listAddSorted(List_t* list, Contact_t* contact)
{
    if (!list || !contact || list->length >= MAX_CONTACTS_COUNT)
        return NULL;
    Item_t* newItem = (Item_t*)malloc(sizeof(Item_t));
    if (!newItem)
        return NULL;
    newItem->contact = contact;
    newItem->next = NULL;
    newItem->prev = NULL;
    // Item_t* forward = list->head;
    // if (!forward)
    // {
    //     list->head = list->tail = newItem;
    //     list->length++;
    //     return newItem->contact;
    // }
    // Item_t* backward = list->tail, *prev = list->tail;
    // for (size_t i = 0; i < list->length / 2.0; i++)
    // {
    //     if (!forward || !backward)
    //         break;
    //     if (compareContacts(*forward->contact, *contact) > 0)
    //     {
    //         prev = forward->prev;  // надо добавить между предыдущим и этим, поэтому берем предыдущий
    //         break;
    //     }
    //     if (compareContacts(*backward->contact, *contact) <= 0)
    //     {
    //         prev = backward;  // надо добавить между этим и следующим (новый prev для newItem)
    //         break;
    //     }
    //     forward = forward->next;
    //     backward = backward->prev;
    // }
    // if (!prev)  // forward->prev или backward->prev было NULL, то есть добавить надо в head
    // {
    //     newItem->next = list->head;
    //     list->head = newItem;
    //     list->length++;
    //     return newItem->contact;
    // }
    // if (!prev->next)
    //     list->tail = newItem;
    // newItem->prev = prev;
    // newItem->next = prev->next;
    // prev->next = newItem;
    newItem = listPlaceSorted(list, newItem);
    list->length++;
    return newItem;
}

Contact_t* listRemoveAt(List_t* list, const size_t index)
{
    if (!list || index >= list->length)
        return NULL;
    Item_t* buffer;
    Contact_t* res;
    buffer = listGetAt(list, index);
    if (!buffer)
        return NULL;
    Item_t* prev = buffer->prev, *next = buffer->next;
    if (!prev)
    {
        if (!next)
            list->head = list->tail = NULL;
        else
        {
            list->head = buffer->next;
            list->head->prev = NULL;
        }
    }
    else if (!next)
    {
        list->tail = buffer->prev;
        list->tail->next = NULL;
    }
    else
    {
        prev->next = next;
        next->prev = prev;
    }
    //
    list->length--;
    res = buffer->contact;
    if (!res)
        return NULL;
    free(buffer);
    buffer = NULL;
    return res;
}

List_t* listClear(List_t* list)
{
    if (!list)
        return NULL;
    if (list->length == 0)
        return list;
    Contact_t* pop = listRemoveAt(list, 0);
    if (!pop)
        return NULL;
    do
    {
        clearContact(pop);
        free(pop);
    } while ((pop = listRemoveAt(list, 0)) != NULL);
    return list;
}

Item_t* listEdit(
    List_t* list,
    Item_t* item,
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
)
{
    if (!list || !item)
        return NULL;
    Contact_t* edited = editContact(
        item->contact,
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emails, socials
    );
    if (!edited)
        return NULL;
    item->contact = edited;
    if ((item->prev && compareContacts(*item->prev->contact, *item->contact) > 0) ||
        (item->next && compareContacts(*item->next->contact, *item->contact) < 0)
    )
        item = listPlaceSorted(list, item);
    return item;
}
