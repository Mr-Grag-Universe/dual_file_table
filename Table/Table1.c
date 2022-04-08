//
// Created by Stephan on 03.04.2022.
//
#include "stdbool.h"
#include "stdlib.h"
#include "all_structures.h"
#include "MyString.h"

//==================Key1==================

void copy_key1(KeyType1 * key, KeyType1 * src) {
    if (src == NULL || key == NULL)
        return;
    key->intKey = src->intKey;
}

char key1cmp(KeyType1 key1, KeyType1 key2) {
    if (key1.intKey < key2.intKey)
        return 1;
    if (key1.intKey > key2.intKey)
        return -1;
    return 0;
}

KeyType1 enter_key1() {
    KeyType1 key;
    printf("enter your intKey for the new element in the lookthrough table.\n");
    key.intKey = get_int();
    return key;
}

//=================TABLE1=================

int binary_search(Table1 table, KeyType1 key) {
    int left = 0, right = table.numberDiffKeysInT1;
    while (left != right) {
        int m = (left+right)/2;
        if (key1cmp(table.ks1[m].key, key) > 0) {
            left = m + 1;
        }
        else if (key1cmp(table.ks1[m].key, key) == 0)
            return m;
        else
            right = m;
    }
    return left;
}

struct KeySpace1 * get_ks1(Table1 table, KeyType1 key1) {
    int ind = binary_search(table , key1);
    if (ind >= table.table1size || ind < 0)
        return NULL;
    if (!key1cmp(table.ks1[ind].key, key1))
        return table.ks1 + ind;
    return NULL;
}

Table1 init_table1(int size) {
    Table1 table;
    table.table1size = size;
    table.ks1 = malloc(sizeof(KeySpace1) * size);
    for (int i = 0; i < table.table1size; ++i) {
        table.ks1[i] = init_ks1();
        /*table.ks1[i].node = NULL;
        table.ks1[i].numberOfNodes = 0;
        table.ks1[i].nodePos = -1;
        table.ks1[i].key.intKey = 0;*/
    }
    table.numberDiffKeysInT1 = 0;
    return table;
}

bool can_insert_in_table1(Table1 table, KeyType1 key) {
    if (table.numberDiffKeysInT1 < table.table1size)
        return true;
    int ind = binary_search(table, key);
    if (ind < 0 || ind >= table.table1size)
        return false;
    if (!key1cmp(table.ks1[ind].key, key))
        return true;

    return false;
}

void print_table1(char * name, Table1 table) {
    printf("=========================\n");
    for (int i = 0; i < table.numberDiffKeysInT1; ++i) {
        printf("key: %d\n", table.ks1[i].key.intKey);
        Node1 * node = table.ks1[i].node;
        while (node) {
            print_item(name, *(node->info));
            node = node->next;
        }
        printf("=========================\n");
    }
}

void free_table1(Table1 table) {
    for (int i = 0; i < table.numberDiffKeysInT1; ++i) {
        Node1 * node = table.ks1[i].node;
        while (node) {
            Node1 * tmp = node->next;
            free(node);
            node = tmp;
        }
    }
    table.table1size = 0;
    table.numberDiffKeysInT1 = 0;
    free(table.ks1);
}

//===============KEYSPACE1================

KeySpace1 init_ks1() {
    KeySpace1 ks;
    ks.node = NULL;
    ks.key.intKey = 0;
    //copy_key1(&(ks.key), &key);
    ks.numberOfNodes = 0;
    ks.nodePos = -1;
    return ks;
}

int renum_nodes1(Node1 * node) {
    if (node == NULL) {
        return -1;
    }
    node->release = renum_nodes1(node->next)+1;
    return node->release;
}

Node1 * get_node1(KeySpace1 * ks1, KeyType2 key2) {
    Node1 * node = ks1->node;
    while (node) {
        if (!key2cmp(node->info->key2, key2)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

bool push_node1(KeySpace1 * ks1, Item * item) {
    if (ks1 == NULL)
        return false;

    Node1 * tmp = ks1->node;

    ks1->node = malloc(sizeof(Node1));
    ks1->node->info = item;
    ks1->node->next = tmp;
    ks1->node->release = 0;
    if (tmp)
        ks1->node->release = tmp->release+1;
    copy_key1(&(ks1->key), &(item->key1));

    ks1->numberOfNodes++;

    return true;
}

bool delete_node1(KeySpace1 * ks1, KeyType2 key2) {
    if (ks1 == NULL)
        return false;

    Node1 * node = ks1->node;
    Node1 * pr_node = node;

    while (node) {
        if (!key2cmp(node->info->key2, key2)) {
            if (pr_node == node) {
                Node1 * tmp = ks1->node->next;
                free(ks1->node);
                ks1->node = tmp;
            } else {
                Node1 * tmp = node->next;
                free(node);
                pr_node->next = tmp;
            }
            ks1->numberOfNodes--;
            if (ks1->numberOfNodes == 0)
                ks1->node = NULL;
            renum_nodes1(ks1->node);
            return true;
        }
        pr_node = node;
        node = node->next;
    }
    return false;
}

//==================Item==================

Item * get_item1(Node1 * node) {
    if (node == NULL)
        return NULL;
    return node->info;
}