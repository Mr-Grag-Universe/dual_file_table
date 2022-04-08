//
// Created by Stephan on 04.04.2022.
//
#include "all_structures.h"

///remove all els with not 0 release
void clear_table2(Table * table, bool mode) {
    for (int i = 0; i < table->table2.table2size; ++i) {
        KeySpace2 * ks = table->table2.KS2C[i].head;
        if (ks == NULL || ks->node == NULL)
            continue;
        while (ks) {
            if (ks->node == NULL || ks->numberOfNodes == 1) {
                ks = ks->next;
                continue;
            }

            int numberOfKeys = ks->node->release;
            KeyType1 * keys = malloc(sizeof(KeyType1) * numberOfKeys);
            Node2 * node = ks->node->next;
            while (node && numberOfKeys) {
                keys[numberOfKeys - 1].intKey = node->info->key1.intKey;
                numberOfKeys--;
                node = node->next;
            }
            numberOfKeys = ks->numberOfNodes-1;//->node->release;
            KeySpace2 * tmp = ks->next;
            for (int j = 0; j < numberOfKeys; ++j) {
                printf("key1: %d, key2: %d\ntable items: %d\n", keys[j].intKey, ks->key.intKey, table->numberOfItems);
                delete_item(table, keys[j], ks->key, mode);
                printf("table items: %d\n", table->numberOfItems);
            }
            ks->node->release = 0;
            ks->node->next = NULL;
            ks = tmp;

            free(keys);
        }
    }
}