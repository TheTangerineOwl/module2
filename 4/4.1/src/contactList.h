#ifndef CONTACT_LIST_H
#define CONTACT_LIST_H

#include "contacts.h"

typedef struct Item_t Item_t;
typedef struct List_t List_t;

struct Item_t
{
    Contact_t* contact;
    Item_t* next;
    Item_t* prev;
};

struct List_t
{
    Item_t* head;
    Item_t* tail;
    size_t length;
};

extern List_t contactList;
// extern size_t contactsCount;

List_t* listInit(List_t* list);
Item_t* listGetAt(const List_t list, const size_t index);
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
    const size_t emailCount,
    char** emails,
    const size_t socialsCount,
    char** socials
);
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
    const size_t emailCount,
    char** emails,
    const size_t socialsCount,
    char** socials
);
Item_t* listAddSorted(List_t* list, Contact_t* contact);
Contact_t* listRemoveAt(List_t* list, const size_t index);
List_t* listClear(List_t* list);

#endif