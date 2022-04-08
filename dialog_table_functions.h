//
// Created by Stephan on 03.04.2022.
//

#ifndef DUAL_FILE_TABLE_DIALOG_TABLE_FUNCTIONS_H
#define DUAL_FILE_TABLE_DIALOG_TABLE_FUNCTIONS_H
#include "all_structures.h"
void add_dialog(Table * table);
void find_dialog(Table * table);

Table * find_t1_k_v(Table * table, KeyType1 key, int version);
Table * find_t1_k(Table * table, KeyType1 key);
Table * find_t2_k_v(Table * table, KeyType2 key, int version);
Table * find_t2_k(Table * table, KeyType2 key);

void delete_dialog(Table * table);

void delete_all_k1(Table * table, KeyType1 key1);
void delete_t1_k_v(Table * table, KeyType1 key, int version);
void delete_all_k2(Table * table, KeyType2 key);
void delete_t2_k_v(Table * table, KeyType2 key, int version);

void clear_table2(Table * table, bool mode);
#endif //DUAL_FILE_TABLE_DIALOG_TABLE_FUNCTIONS_H
