//
// Created by Stephan on 03.04.2022.
//

#ifndef DUAL_FILE_TABLE_ALL_STRUCTURES_H
#define DUAL_FILE_TABLE_ALL_STRUCTURES_H
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

typedef struct Table1 Table1;
typedef struct Table2 Table2;
typedef struct KS2_coll KS2_coll;
typedef struct KeySpace1 KeySpace1;
typedef struct KeySpace2 KeySpace2;
typedef struct Node1 Node1;
typedef struct Node2 Node2;
typedef struct KeyType1 KeyType1;
typedef struct KeyType2 KeyType2;
typedef struct Item Item;

struct Table1 {
    int table1size;
    int numberDiffKeysInT1;
    struct KeySpace1 * ks1;
};
struct KeySpace1 * get_ks1(Table1 table, KeyType1 key1);
int binary_search(Table1 table, KeyType1 key1);
Table1 init_table1(int size);
bool can_insert_in_table1(Table1 table, KeyType1 key);
void print_table1(char * name, Table1 table);
void free_table1(Table1 table);

struct Table2{
    int table2size;
    struct KS2_coll * KS2C;
};
int get_hash(Table2 table, KeyType2 key);
struct KS2_coll * get_ksc(Table2 table, int hash);
Table2 init_table2(int size);
void print_table2(char * name, Table2 table);
void free_table2(Table2 table);

typedef struct Table {
    char * tableName;
    char * infoFileName;

    int msize1;
    int msize2;

    Table1 table1;
    Table2 table2;

    // размер в байтах файла DataFile
    size_t dataSize;
    int numberOfItems;
    fpos_t ks1_offset;
    fpos_t ks2_offset;
} Table;
bool add_item(Table * table, Item * item);
bool delete_item(Table * table, KeyType1 key1, KeyType2 key2, bool mode);
Item * get_item(Table * table, KeyType1 key1, KeyType2 key2);
Table * init_table(int size1, int size2);
void print_item(char * name, Item item);
void print_table(Table table);
void free_table(Table * table);

typedef struct InfoType{
    size_t infoSize;
    char * data;
} InfoType;

typedef struct KeyType1 {
    int intKey;
} KeyType1;
char key1cmp(KeyType1 key1, KeyType1 key2);
KeyType1 enter_key1();
void copy_key1(KeyType1 * key, KeyType1 * src);

typedef struct KeyType2 {
    int intKey;
} KeyType2;
char key2cmp(KeyType2 key1, KeyType2 key2);/* {
    if (key1.intKey < key2.intKey)
        return 1;
    if (key1.intKey > key2.intKey)
        return -1;
    return 0;
}*/
void copy_key2(KeyType2 * key, KeyType2 * src);
KeyType2 enter_key2();

struct Item {
    //InfoType * info;
    size_t infoSize;
    size_t infoPos;

    KeyType1 key1;
    KeyType2 key2;

    int ind1;
    int ind2;

    struct Node1 *p1;
    struct Node2 *p2;
};
Item * create_item(InfoType * info, KeyType1 key1, KeyType2 key2);
void free_item(Item * item);
void copy_item(Item * item, Item * src);

typedef struct KeySpace1 {
    KeyType1 key;

    Node1 *node;

    //KeySpace1 *next;

    //int numberOfKS;
    int numberOfNodes;
    //int KSNextNumber;
    //long KSNextPos;
    size_t nodePos;
} KeySpace1;
KeySpace1 init_ks1();
Node1 * get_node1(KeySpace1 * ks1, KeyType2 key2);
bool push_node1(KeySpace1 * ks1, Item * item);
bool delete_node1(KeySpace1 * ks1, KeyType2 key2);

typedef struct Node1 {
    int release;
    Item *info;
    struct Node1 *next;

    size_t offset;
} Node1;
Item * get_item1(Node1 * node1);

typedef struct KS2_coll {
    struct KeySpace2 * head;
    struct KeySpace2 * tail;
    int numberOfKS;
} KS2_coll;
KeySpace2 * get_ks2(KS2_coll * KSC, KeyType2 key2);
void push_ks2(KS2_coll * KSC, KeySpace2 * ks);
void delete_ks2(KS2_coll * KSC, KeyType2 key2);
KS2_coll init_KSC();

typedef struct KeySpace2 {
    KeyType2 key;

    struct Node2 *node;

    struct KeySpace2 *next;

    int numberOfKS;
    int numberOfNodes;
    int KSNextNumber;
    long KSNextPos;
    size_t nodePos;
} KeySpace2;
KeySpace2 init_ks2(KeyType2 key);
struct Node2 * get_node2(KeySpace2 * ks1, KeyType1 key1);
bool push_node2(KeySpace2 * ks, Item * item);
bool delete_node2(KeySpace2 * ks2, KeyType1 key2);
void free_ks2(KeySpace2 * ks);

typedef struct Node2 {
    int release;
    Item * info;
    struct Node2 *next;

    size_t offset;
} Node2;
InfoType * get_info(Node2 * node2);

#endif //DUAL_FILE_TABLE_ALL_STRUCTURES_H
