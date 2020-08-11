//NAME: Son Dang
//EMAIL: sonhdang@ucla.edu
//ID: 105215636

#include "SortedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>

int opt_yield = 0;

void SortedList_insert(SortedList_t *list, SortedListElement_t *element)
{
    SortedListElement_t *ptr = list->next;
    if (ptr == NULL)
    {
        if (opt_yield & INSERT_YIELD)
            sched_yield();

        list->next = element;
        list->prev = element;
        element->next = list;
        element->prev = list;
    }
    else
    {
        while(ptr->key != NULL && strcmp(ptr->key,element->key) < 0)
            ptr = ptr->next;

        if (opt_yield & INSERT_YIELD)
            sched_yield();

        element->next = ptr;
        element->prev = ptr->prev;
        (ptr->prev)->next = element;
        ptr->prev = element;
    }
}

int SortedList_delete( SortedListElement_t *element)
{
    if (&(element->next)->prev == &(element->prev)->next)
        return 1;
    else
    {
        if (opt_yield & DELETE_YIELD)
            sched_yield();

        (element->next)->prev = element->prev;
        (element->prev)->next = element->next;
        return 0;
    }
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key)
{
    SortedListElement_t *ptr = list;
    if (ptr->next == NULL)
        return NULL;
    else
    {
        ptr = ptr->next;
        while (ptr->key != NULL)
        {
            if (opt_yield & LOOKUP_YIELD)
                sched_yield();

            if (strcmp(ptr->key,key))
                return ptr;
            ptr = ptr->next;
        }
        return NULL;
    }
}

int SortedList_length(SortedList_t *list)
{
    SortedListElement_t *ptr = list;
    int counter = 0;
    if (ptr->next == NULL)
    {
        if (ptr->prev == NULL)
            return counter;
        else
            return -1;
    }
    else
    {
        do
        {
            if (&(ptr->next)->prev == &(ptr->prev)->next)
                return -1;
            ptr = ptr->next;
            counter++;
        } while(ptr->key != NULL);
        return --counter;
    }
}