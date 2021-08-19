#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry **pHead)
{
    /* TODO: implement */
    int hash = 0;
    for(int i=0; i<strlen(lastname); i++) {
        hash += lastname[i] - 'a';
    }
    hash %= hashnum;

    while (pHead[hash] != NULL) {
        if (strcasecmp(lastname, pHead[hash]->lastName) == 0)
            return pHead[hash];
        pHead[hash] = pHead[hash]->pNext;
    }
    return NULL;
}

entry **append(char lastName[], entry **e)
{
    /* generate hash number */
    int hash = 0;
    for(int i=0; i<strlen(lastName); i++) {
        hash += lastName[i] - 'a';
    }
    hash %= hashnum;

    /* allocate memory for the new entry and put lastName */
    e[hash]->pNext = (entry *) malloc(sizeof(entry));
    e[hash] = e[hash]->pNext;
    strcpy(e[hash]->lastName, lastName);
    e[hash]->pNext = NULL;
    return e;
}
