//
// Created by Stephan on 03.04.2022.
//
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "all_structures.h"
#include "MyString.h"

//==================Key2==================

void copy_key2(KeyType2 * key, KeyType2 * src) {
    if (src == NULL || key == NULL)
        return;
    key->intKey = src->intKey;
}

char key2cmp(KeyType2 key1, KeyType2 key2) {
    if (key1.intKey < key2.intKey)
        return 1;
    if (key1.intKey > key2.intKey)
        return -1;
    return 0;
}

KeyType2 enter_key2() {
    KeyType2 key;
    printf("enter your intKey for the new element in the hash table.\n");
    key.intKey = get_int();

    return key;
}

//=================TABLE2=================

Table2 init_table2(int size) {
    Table2 table;
    table.table2size = size;
    table.KS2C = malloc(sizeof(KS2_coll) * size);
    for (int i = 0; i < size; ++i)
        table.KS2C[i] = init_KSC();
    return table;
}

int get_hash(Table2 table, KeyType2 key) {
    return abs(key.intKey)%table.table2size;
}

KS2_coll * get_ksc(Table2 table, int hash) {
    if (hash < 0 || hash >= table.table2size)
        return NULL;
    return table.KS2C+hash;
}

void print_table2(char * name, Table2 table) {
    printf("=========================\n");
    for (int i = 0; i < table.table2size; ++i) {
        KS2_coll * ksc = table.KS2C+i;
        printf("hash: %d\n", i);
        KeySpace2 * ks = ksc->head;
        while (ks) {
            Node2 * node = ks->node;
            printf("key: %d\n", ks->key.intKey);
            while (node) {
                print_item(name, *(node->info));
                node = node->next;
            }
            ks = ks->next;
            printf(">>>>>>>>>>>>>>>>>>>>>>>>>\n");
        }
        printf("=========================\n");
    }
}

//==============KEYSPACECOLL==============

KS2_coll init_KSC() {
    KS2_coll ksc;
    ksc.head = NULL;
    ksc.numberOfKS = 0;
    ksc.tail = NULL;
    return ksc;
}

void copy_KS2(KeySpace2 * ks, KeySpace2 * src) {
    if (src == NULL)
        return;
    memmove(ks, src, sizeof(KeySpace2));
    copy_key2(&(ks->key), &(src->key));
}

KeySpace2 * get_ks2(KS2_coll * KSC, KeyType2 key2) {
    KeySpace2 * ks = KSC->head;
    while (ks != NULL) {
        if (!key2cmp(ks->key, key2))
            return ks;
        ks = ks->next;
    }
    return NULL;
}

void push_ks2(KS2_coll * KSC, KeySpace2 * ks) {
    if (ks == NULL)
        return;
    KeySpace2 * KS = KSC->head;
    KSC->head = ks;
    if (KSC->tail == NULL)
        KSC->tail = KSC->head;
    //copy_KS2(KSC->head, ks);
    KSC->numberOfKS++;
    KSC->head->next = KS;
}

void delete_ks2(KS2_coll * KSC, KeyType2 key) {
    if (KSC == NULL)
        return;
    KeySpace2 * ks = KSC->head;
    KeySpace2 * pr_ks = ks;
    while (ks) {
        if (!key2cmp(ks->key, key)) {
            KSC->numberOfKS++;
            if (ks == pr_ks) {
                KeySpace2 * tmp = KSC->head->next;
                free_ks2(KSC->head);
                KSC->head = tmp;
            } else {
                KeySpace2 * tmp = ks->next;
                free_ks2(ks);
                pr_ks->next = tmp;
            }
            return;
        }
        pr_ks = ks;
        ks = ks->next;
    }
}

void free_ksc(KS2_coll ksc) {
    KeySpace2 * ks = ksc.head;

    while (ks) {
        KeySpace2 * tmp = ks->next;
        free_ks2(ks);
        ks = tmp;
    }
    ksc.head = NULL;
    ksc.tail = NULL;
    ksc.numberOfKS = 0;
}

void free_table2(Table2 table) {
    for (int i = 0; i < table.table2size; ++i) {
        free_ksc(table.KS2C[i]);
    }
    free(table.KS2C);
}

//===============KEYSPACE2================

KeySpace2 init_ks2(KeyType2 key) {
    KeySpace2 ks;
    ks.node = NULL;
    copy_key2(&(ks.key), &key);
    ks.numberOfNodes = 0;
    ks.nodePos = -1;
    ks.KSNextNumber = 0;
    return ks;
}

int renum_nodes2(Node2 * node) {
    if (node == NULL) {
        return -1;
    }
    node->release = renum_nodes2(node->next)+1;
    return node->release;
}

Node2 * get_node2(KeySpace2 * ks, KeyType1 key) {
    Node2 * node = ks->node;
    while (node) {
        if (!key1cmp(node->info->key1, key)) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

bool push_node2(KeySpace2 * ks, Item * item) {
    if (ks == NULL)
        return false;

    Node2 * tmp = ks->node;

    ks->node = malloc(sizeof(Node2));
    ks->node->info = item;
    ks->node->next = tmp;
    ks->node->release = 0;
    if (tmp)
        ks->node->release = tmp->release+1;
    copy_key2(&(ks->key), &(item->key2));

    ks->numberOfNodes++;

    return true;
}

bool delete_node2(KeySpace2 * ks, KeyType1 key) {
    if (ks == NULL)
        return false;

    Node2 * node = ks->node;
    Node2 * pr_node = node;
    while (node) {
        if (!key1cmp(node->info->key1, key)) {
            if (pr_node == node) {
                Node2 * tmp = ks->node->next;
                free(ks->node);
                ks->node = tmp;
            } else {
                Node2 * tmp = node->next;
                free(node);
                pr_node->next = tmp;
            }
            ks->numberOfNodes--;
            if (ks->numberOfNodes == 0)
                ks->node = NULL;
            renum_nodes2(ks->node);
            return true;
        }
        pr_node = node;
        node = node->next;
    }
    return false;
}

/// free all nodes in ks and ks with its own
void free_ks2(KeySpace2 * ks) {
    Node2 * node = ks->node;
    while (node) {
        Node2 * tmp = node->next;
        free(node);
        node = tmp;
    }
    ks->numberOfNodes = 0;
    free(ks);
}

//==================Item==================

Item * get_item2(Node2 * node) {
    if (node == NULL)
        return NULL;
    return node->info;
}