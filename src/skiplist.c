// src/skiplist.c
#include "skiplist.h"
#include <stdlib.h>
#include <stdio.h>

/*original
SkipList *create_skiplist()
{
    SkipList *list = malloc(sizeof(SkipList));
    list->level = 0;
    list->header = malloc(sizeof(SkipListNode) + MAX_LEVEL * sizeof(SkipListNode *));
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        list->header->forward[i] = NULL;
    }
    return list;
}
*/

SkipList *create_skiplist()
{
    SkipList *list = malloc(sizeof(SkipList));
    if (!list)
    {
        printf("Error: No se pudo asignar memoria para SkipList.\n");
        exit(EXIT_FAILURE);
    }
    list->level = 0;
    list->header = malloc(sizeof(SkipListNode) + MAX_LEVEL * sizeof(SkipListNode *));
    if (!list->header)
    {
        printf("Error: No se pudo asignar memoria para el nodo header.\n");
        free(list);
        exit(EXIT_FAILURE);
    }

    list->header->key = 0;
    list->header->row_index = 0;
    for (int i = 0; i < MAX_LEVEL; i++)
    {
        list->header->forward[i] = NULL;
    }

    return list;
}

void skiplist_insert(SkipList *list, int key, uint32_t row_index)
{
    SkipListNode *update[MAX_LEVEL];
    SkipListNode *x = list->header;

    for (int i = list->level; i >= 0; i--)
    {
        while (x->forward[i] && x->forward[i]->key < key)
        {
            x = x->forward[i];
        }
        update[i] = x;
    }

    int new_level = (rand() % MAX_LEVEL); // Randomizar el nivel
    if (new_level > list->level)
    {
        for (int i = list->level + 1; i <= new_level; i++)
        {
            update[i] = list->header;
        }
        list->level = new_level;
    }
    x = malloc(sizeof(SkipListNode) + (new_level + 1) * sizeof(SkipListNode *));
    // x = malloc(sizeof(SkipListNode) + new_level * sizeof(SkipListNode*));
    x->key = key;
    x->row_index = row_index;

    for (int i = 0; i <= new_level; i++)
    {
        x->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = x;
    }
}

uint32_t skiplist_search(SkipList *list, int key)
{
    if (!list || !list->header)
    {
        printf("Error: índice no inicializado.\n");
        return UINT32_MAX;
    }

    SkipListNode *x = list->header;
    for (int i = list->level; i >= 0; i--)
    {
        while (x->forward[i] && x->forward[i]->key < key)
        {
            x = x->forward[i];
        }
    }

    if (x->forward[0] && x->forward[0]->key == key)
    {
        return x->forward[0]->row_index;
    }

    printf("Clave %d no encontrada en el índice.\n", key);
    return UINT32_MAX;
}

/*original
uint32_t skiplist_search(SkipList* list, int key) {
    SkipListNode* x = list->header;

    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->key < key) {
            x = x->forward[i];
        }
    }

    x = x->forward[0];
    if (x && x->key == key) {
        return x->row_index;
    }
    return UINT32_MAX;  // No encontrado
}
*/
/*segunda modificacion
uint32_t skiplist_search(SkipList* list, int key) {
    if (!list || !list->header) {
        printf("Error: indice no inicializado.\n");
        return UINT32_MAX;
    }

    SkipListNode* x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->key < key) {
            x = x->forward[i];
        }
    }

    x = x->forward[0];
    if (x && x->key == key) {
        return x->row_index;
    }
    printf("Clave %d no encontrada en el indice.\n", key);
    return UINT32_MAX;
}
*/

/*original
void skiplist_free(SkipList* list) {
    SkipListNode* node = list->header->forward[0];
    while (node) {
        SkipListNode* temp = node->forward[0];
        free(node);
        node = temp;
    }
    free(list->header);
    free(list);
}*/

void skiplist_free(SkipList *list)
{
    if (!list)
        return; // Verificar si la lista es nula
    SkipListNode *node = list->header->forward[0];
    while (node)
    {
        SkipListNode *temp = node->forward[0];
        free(node);
        node = temp;
    }
    if (list->header)
    {
        free(list->header);
    }
    free(list);
}
