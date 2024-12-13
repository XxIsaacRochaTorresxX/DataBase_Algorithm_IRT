// include/skiplist.h
#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_LEVEL 6 // Máximo nivel para los nodos

typedef struct SkipListNode {
    int key;                         // Clave de indexado (puede ser `id`)
    uint32_t row_index;              // Índice de fila en la tabla
    struct SkipListNode* forward[];  // Apuntadores hacia adelante
} SkipListNode;

typedef struct {
    int level;                       // Nivel máximo actual
    SkipListNode* header;            // Nodo cabecera
} SkipList;

// Funciones para la Skip List
SkipList* create_skiplist();
void skiplist_insert(SkipList* list, int key, uint32_t row_index);
uint32_t skiplist_search(SkipList* list, int key);
void skiplist_free(SkipList* list);

#endif // SKIPLIST_H
