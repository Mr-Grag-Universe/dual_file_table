//
// Created by Stephan on 03.04.2022.
//
#include "string.h"
#include "all_structures.h"
#include "file.h"

//=================TABLE=================

Table * init_table(int size1, int size2) {
    Table * table = malloc(sizeof(Table));
    table->msize1 = size1;
    table->msize2 = size2;
    table->numberOfItems = 0;
    table->dataSize = 0;
    table->table1 = init_table1(size1);
    table->table2 = init_table2(size2);
    return table;
}

Item * get_item(Table * table, KeyType1 key1, KeyType2 key2) {
    KS2_coll * ksc = get_ksc(table->table2, get_hash(table->table2, key2));
    if (ksc == NULL)
        return NULL;

    KeySpace2 * ks2 = get_ks2(ksc, key2);
    if (ks2 == NULL)
        return NULL;

    KeySpace1 * ks1 = get_ks1(table->table1, key1);
    if (ks1 == NULL)
        return NULL;

    Node1 * node = get_node1(ks1, key2);
    if (node == NULL)
        return NULL;

    return node->info;
}

bool add_item(Table * table, Item * item) {
    if(get_item(table, item->key1, item->key2)) {
        printf("there is item with such keys in this table\n");
        return false;
    }

    if (!can_insert_in_table1(table->table1, item->key1)) {
        printf("this item cannot be inserted in table 1\n");
        return false;
    }

    int ind1 = binary_search(table->table1, item->key1);
    KeySpace1 * ks1 = table->table1.ks1 + ind1;
    if (table->table1.numberDiffKeysInT1 == 0 || key1cmp(ks1->key, item->key1)) {
        if (table->table1.numberDiffKeysInT1)
            memmove(ks1 + 1, ks1, sizeof(KeySpace1) * (table->table1.numberDiffKeysInT1 - ind1));
        *ks1 = init_ks1();
        copy_key1(&(ks1->key), &(item->key1));
        table->table1.numberDiffKeysInT1++;
    }

    int hash = get_hash(table->table2, item->key2);
    KS2_coll * ksc = get_ksc(table->table2, hash);
    KeySpace2 * ks2 = get_ks2(ksc, item->key2);
    if (ks2 == NULL) {
        ks2 = malloc(sizeof(KeySpace2));
        *ks2 = init_ks2(item->key2);
        push_ks2(ksc, ks2);
        if (!get_ks2(ksc, item->key2)) {
            fprintf(stderr, "ERROR IN ADDING IN TABLE2\n");
            exit(111);
        }
    }

    item->ind1 = ind1;
    item->ind2 = hash;

    push_node1(ks1, item);
    push_node2(ks2, item);

    Node1 * node1 = get_node1(ks1, item->key2);
    if (node1 == NULL) {
        fprintf(stderr, "ERROR IN ADDING IN NODE1");
        exit(112);
    }
    Node2 * node2 = get_node2(ks2, item->key1);
    if (node2 == NULL) {
        fprintf(stderr, "ERROR IN ADDING IN NODE2");
        exit(113);
    }

    item->p1 = node1;
    item->p2 = node2;

    table->numberOfItems++;
    table->dataSize += item->infoSize + sizeof(Item) + sizeof(bool);

    return true;
}

bool delete_item(Table * table, KeyType1 key1, KeyType2 key2, bool mode) {
    if(get_item(table, key1, key2) == NULL) {
        printf("there is no item with such keys in this table\n");
        return false;
    }

    int ind1 = binary_search(table->table1, key1);
    KeySpace1 * ks1 = table->table1.ks1 + ind1;
    if (key1cmp(ks1->key, key1)) {
        fprintf(stderr,"ERROR: there is no such key in the 1 table\n");
        return false;
    }

    int hash = get_hash(table->table2, key2);
    KS2_coll * ksc = get_ksc(table->table2, hash);
    KeySpace2 * ks2 = get_ks2(ksc, key2);
    if (ks2 == NULL) {
        fprintf(stderr,"ERROR: there is no such key in the 2 table\n");
        return false;
    }

    Node1 * node = get_node1(ks1, key2);
    if (node == NULL) {
        fprintf(stderr, "something came really wrong.\nCHECK YOR DELETE\n");
        return false;
    }

    Item * item = node->info;

    delete_node1(ks1, key2);
    delete_node2(ks2, key1);


    Node1 * node1 = get_node1(ks1, key2);
    if (node1) {
        fprintf(stderr, "ERROR IN DELETE IN NODE1");
        exit(112);
    }
    Node2 * node2 = get_node2(ks2, key1);
    if (node2) {
        fprintf(stderr, "ERROR IN DELETE IN NODE2");
        exit(113);
    }

    if (ks1->numberOfNodes == 0) {
        memmove(ks1, ks1+1, sizeof(KeySpace1) * (table->table1.numberDiffKeysInT1 - ind1 - 1));
        table->table1.numberDiffKeysInT1--;
    }
    if (ks2->numberOfNodes == 0) {
        delete_ks2(ksc, key2);
    }

    if (mode)
        delete_item_from_file(table->infoFileName, item);

    //table->dataSize -= (item->infoSize + sizeof(Item) + sizeof(InfoType) + sizeof(bool));
    free_item(item);

    table->numberOfItems--;

    return true;
}

void print_table(Table table) {
    printf("THE FIRST TABLE\n");
    print_table1(table.infoFileName, table.table1);

    printf("THE SECOND TABLE\n");
    print_table2(table.infoFileName, table.table2);
}

void free_all_items(Table * table) {
    if (table == NULL)
        return;
    for (int i = 0; i < table->table1.numberDiffKeysInT1; ++i) {
        Node1 * node = table->table1.ks1[i].node;
        while (node) {
            if (node->info) {
                //free(node->info->info->data);
                //free(node->info->info);
                free_item(node->info);
            }
            node = node->next;
        }
    }
}

void free_table(Table * table) {
    if (table == NULL)
        return;
    free_all_items(table);
    free_table1(table->table1);
    free_table2(table->table2);
    free(table->infoFileName);
    free(table->tableName);
    free(table);
}

//===================ITEM=================

Item * create_item(InfoType * info, KeyType1 key1, KeyType2 key2) {
    Item * item = malloc(sizeof(Item));
    //item->info = info;
    copy_key1(&(item->key1), &key1);
    copy_key2(&(item->key2), &key2);
    item->p1 = NULL;
    item->p2 = NULL;
    item->ind1 = -1;
    item->ind2 = -1;
    item->infoPos = -1;
    //item->info->infoSize = sizeof(char) * (strlen(info->data) + 1);
    item->infoSize = sizeof(char) * (strlen(info->data) + 1);
    return item;
}

void free_item(Item * item) {
    free(item);
}

void print_item(char * name, Item item) {
    printf("key1: %d, key2: %d\n", item.key1.intKey, item.key2.intKey);
    if (item.infoPos == -1) {
        printf("(null)\nthere is not any data here\n");
        return;
    }
    InfoType * info = read_info(name, &item);
    if (info) {
        printf("data: %s\n", info->data);
        free(info->data);
        free(info);
    }
}

void copy_item(Item * item, Item * src) {
    copy_key1(&(item->key1), &(src->key1));
    copy_key2(&(item->key2), &(src->key2));

    //item->info = malloc(sizeof(InfoType));
    //item->info->data = malloc(sizeof(char) * src->info->infoSize);
    //memmove(item->info->data, src->info->data, sizeof(char) * src->info->infoSize);
    //item->info->infoSize = src->info->infoSize;
    item->infoPos = src->infoPos;
    item->infoSize = src->infoSize;
    item->ind1 = src->ind1;
    item->ind2 = src->ind2;
    item->p1 = NULL;
    item->p2 = NULL;
}