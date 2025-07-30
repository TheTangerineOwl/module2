#include "contactList.h"

extern List_t contactList;

List_t* listInit(List_t* list)
{
    List_t* bufList = (List_t*)malloc(sizeof(List_t));
    if (!bufList)
        RETURN_NULL_WITH_MSG("listInit: can't allocate memory to list\n");
    list = bufList;
    // list = (List_t*)memset(list, 0, sizeof(List_t));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    return list;
}

Item_t* listGetAt(const List_t list, const size_t index)
{
    if (index >= list.length)
        RETURN_NULL_WITH_MSG("listGetAt: index out of range\n");
    Item_t* item = NULL;
    if (index < list.length / 2.0)
    {
        item = list.head;
        for (size_t i = 0; i < index; i++)
            item = item->next;
    }
    else
    {
        item = list.tail;
        for (size_t i = list.length - 1; i > index; i--)
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
        RETURN_NULL_WITH_MSG("listAddCreate: 'list' was NULL\n");
    Contact_t* contact = newContact(
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emails, socials
    );
    if (!contact)
    {
        RETURN_NULL_WITH_MSG("listAddCreate: 'newContact' returned NULL\n");
    }
    Item_t* res = listAddSorted(list, contact);
    if (res)
        return res;
    clearContact(contact);
    free(contact);
    contact = NULL;
    res = NULL;
    RETURN_NULL_WITH_MSG("listAddCreate: 'listAddSorted' returned NULL\n");
}

Item_t* listPlaceSorted(List_t* list, Item_t* newItem)
{
    Item_t* forward = list->head;

    if (!newItem)
        RETURN_NULL_WITH_MSG("listPlaceSorted: 'newItem' was NULL\n");
    if (!newItem->contact)
        RETURN_NULL_WITH_MSG("listPlaceSorted: 'newItem->contact' was NULL\n");
    if (!forward)
    {
        list->head = list->tail = newItem;
        return newItem;
    }
    Item_t* backward = list->tail, *prev = list->tail;
    for (size_t i = 0; i <= list->length / 2.0; i++)
    {
        if (!forward || !forward->contact || !backward || !backward->contact)
            break;
        if (compareContacts(*forward->contact, *newItem->contact) > 0)
        {
            prev = forward->prev;  // надо добавить между предыдущим и этим, поэтому берем предыдущий
            break;
        }
        if (compareContacts(*backward->contact, *newItem->contact) <= 0)
        {
            prev = backward;  // надо добавить между этим и следующим (newItem будет после prev)
            break;
        }
        forward = forward->next;
        backward = backward->prev;
    }
    if (!prev)  // forward->prev или backward->prev было NULL, то есть добавить надо в head
    {
        newItem->next = list->head;
        newItem->prev = NULL;
        list->head = newItem;
        return newItem;
    }

    Item_t* buf = prev->next;  // будущий next
    if (!buf)  // добавление в tail
        list->tail = newItem;
    else
        buf->prev = newItem;
    newItem->prev = prev;
    newItem->next = buf;
    prev->next = newItem;

    return newItem;
}

Item_t* listAddSorted(List_t* list, Contact_t* contact)
{
    if (!list)
        RETURN_NULL_WITH_MSG("listAddSorted: 'list' was NULL\n");
    if (!contact)
        RETURN_NULL_WITH_MSG("listAddSorted: 'contact' was NULL\n");
    if (list->length >= MAX_CONTACTS_COUNT)
        RETURN_NULL_WITH_MSG("listAddSorted: maximum contacts reached\n");
    Item_t* newItem = (Item_t*)malloc(sizeof(Item_t));
    if (!newItem)
        RETURN_NULL_WITH_MSG("listAddSorted: 'newItem' was NULL after malloc\n");
    newItem->contact = contact;
    newItem->next = NULL;
    newItem->prev = NULL;

    newItem = listPlaceSorted(list, newItem);

    if (!newItem)
        RETURN_NULL_WITH_MSG("listAddSorted: 'newItem' was NULL\n");
    list->length++;
    return newItem;
}

Contact_t* listRemoveAt(List_t* list, const size_t index)
{
    if (!list)
        RETURN_NULL_WITH_MSG("listRemoveAt: 'list' was NULL\n");
    if (index >= list->length)
        RETURN_NULL_WITH_MSG("listRemoveAt: index out of range\n");
    Item_t* buffer;
    Contact_t* res;

    buffer = listGetAt(*list, index);
    if (!buffer)
        RETURN_NULL_WITH_MSG("listRemoveAt: 'listGetAt' returned NULL\n");
    Item_t* prev = buffer->prev, *next = buffer->next;

    if (!prev && !next)  // prev == NULL, next == NULL (only element)
        list->head = list->tail = NULL;
    else if (!prev && next)  // prev == NULL, next != NULL (head)
    {
        list->head = buffer->next;
        list->head->prev = NULL;
    }
    else if (!next)  // prev != NULL, next == NULL (tail)
    {
        list->tail = buffer->prev;
        list->tail->next = NULL;
    }
    else  // prev != NULL, next != NULL
    {
        prev->next = next;
        next->prev = prev;
    }
    list->length--;
    res = buffer->contact;
    if (!res)
        RETURN_NULL_WITH_MSG("listRemoveAt: 'contact' in list item was NULL\n");
    buffer->contact = NULL;
    free(buffer);
    buffer = NULL;
    return res;
}

List_t* listClear(List_t* list)
{
    if (!list)
        RETURN_NULL_WITH_MSG("listClear: 'list' was NULL\n");
    if (list->length == 0)
    {
        list->head = NULL;
        list->tail = NULL;
        return list;
    }
    Contact_t* pop = listRemoveAt(list, 0);
    if (!pop)
    {
        list->head = NULL;
        list->tail = NULL;
        RETURN_NULL_WITH_MSG("listClear: 'listRemoveAt' returned NULL\n");
    }
    do
    {
        clearContact(pop);
        free(pop);
        pop = NULL;
    } while ((pop = listRemoveAt(list, 0)) != NULL);
    list->head = NULL;
    list->tail = NULL;
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
    if (!list)
        RETURN_NULL_WITH_MSG("listEdit: 'list' returned NULL\n");
    if (!item)
        RETURN_NULL_WITH_MSG("listEdit: 'item' returned NULL\n");
    Contact_t* edited = editContact(
        item->contact,
        lastName, firstName, patronim,
        workPlace, position,
        workPhone, personalPhone, homePhone, extraPhone,
        emails, socials
    );
    if (!edited)
        RETURN_NULL_WITH_MSG("listEdit: 'editContact' returned NULL\n");
    item->contact = edited;
    if ((item->prev && compareContacts(*item->prev->contact, *item->contact) > 0) ||
        (item->next && compareContacts(*item->next->contact, *item->contact) < 0)
    )
    {
        Item_t* prev = item->prev;
        // Удаление из списка, чтобы потом вставить сортированно
        if (prev)
        {
            if (!item->next)  // prev != NULL && next == NULL (tail)
            {
                list->tail = prev;
                prev->next = NULL;
            }
            else
            {
                prev->next = item->next;
                item->next->prev = prev;
            }
        }
        else 
        {
            // prev == NULL, next != NULL (head)
            list->head = item->next;
            item->next->prev = NULL;
        }
        item = listPlaceSorted(list, item);
    }
    return item;
}
