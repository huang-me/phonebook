#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry *pHead)
{
    /* TODO: implement */
    for(int i=levelNum-2; i>=0; i--) {
        while(pHead->pnNext[i] && strcasecmp(pHead->pnNext[i]->lastName, lastname) <= 0)
            pHead = pHead->pnNext[i];
    }
    while (pHead != NULL) {
        if (strcasecmp(lastname, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e, entry *tail[])
{
    /* get random number for level */
    int level = rand() % levelNum;

    /* initialize tail for each level */
    if(tail[0] == NULL) {
        for(int i=0; i<levelNum; i++)
            tail[i] = e;
    }

    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;

    /* setup skiplist pointer */
    for(int i=0; i<level; i++) {
        tail[i]->pnNext[i] = e;
        tail[i] = e;
    }
    for(int i=0; i<levelNum; i++)
        e->pnNext[i] = NULL;

    return e;
}
