//
// Created by Stephan on 04.04.2022.
//

#include "all_structures.h"
#include "file.h"
#include "MyString.h"
#include "string.h"

Table * find_t2_k_v(Table * table, KeyType2 key, int version) {
    Table * table1 = init_table(1, 1);
    KeySpace2 * ks = get_ks2(get_ksc(table->table2, get_hash(table->table2, key)), key);

    if (ks == NULL || ks->node == NULL)
        return table1;

    Node2 * node = ks->node;
    if (ks->node == NULL)
        return table1;
    while (node && node->release != version) {
        node = node->next;
    }

    if (node == NULL)
        return table1;

    InfoType * info;
    info = read_info(table->infoFileName, node->info);
    Item * item = create_item(info, node->info->key1, node->info->key2);
    item->infoPos = node->info->infoPos;
    add_item(table1, item);
    free(info->data);
    free(info);

    return table1;
}

Table * find_t2_k(Table * table, KeyType2 key) {
    KeySpace2 * ks = get_ks2(get_ksc(table->table2, get_hash(table->table2, key)), key);
    if (ks == NULL || ks->node == NULL) {
        return NULL;
    }
    Table * table1 = init_table((ks->node->release+1), 1);
    Node2 * node = ks->node;
    if (ks->node == NULL)
        return table1;
    while (node) {
        //Item * item = malloc(sizeof(Item));
        InfoType * info = read_info(table->infoFileName, node->info);
        Item * item = create_item(info, node->info->key1, node->info->key2);
        item->infoPos = node->info->infoPos;
        add_item(table1, item);
        free(info->data);
        free(info);
        node = node->next;
    }
    return table1;
}

Table * find_t1_k_v(Table * table, KeyType1 key, int version) {
    Table * table1 = init_table(1, 1);
    KeySpace1 * ks = get_ks1(table->table1, key);

    if (ks == NULL || ks->node == NULL)
        return table1;

    Node1 * node = ks->node;
    while (node && node->release != version) {
        node = node->next;
    }

    if (node == NULL)
        return table1;

    InfoType * info = read_info(table->infoFileName, node->info);
    Item * item = create_item(info, node->info->key1, node->info->key2);
    //copy_item(item, node->info);
    //create_item()
    item->infoPos = node->info->infoPos;
    add_item(table1, item);
    free(info->data);
    free(info);

    return table1;
}

Table * find_t1_k(Table * table, KeyType1 key) {
    KeySpace1 * ks = get_ks1(table->table1, key);
    if (ks == NULL || ks->node == NULL) {
        return NULL;
    }
    Table * table1 = init_table(1, (ks->node->release+1));

    Node1 * node = ks->node;
    while (node) {
        InfoType * info = read_info(table->infoFileName, node->info);
        Item * item = create_item(info, node->info->key1, node->info->key2);
        item->infoPos = node->info->infoPos;
        add_item(table1, item);
        free(info->data);
        free(info);
        node = node->next;
    }
    return table1;
}

void find_dialog(Table * table) {
    printf("What are you searching for?\n1) El with 2 keys\n2) Els with 1 intKey\n");
    int chose_2_or_1_key = -1;
    while (chose_2_or_1_key < 1 || chose_2_or_1_key > 2) {
        chose_2_or_1_key = get_int();
    }

    switch (chose_2_or_1_key) {
        ///normal 2 keys searching
        case 1: {
            printf("enter your keys:\n");
            printf("key1:\n");
            int k1 = get_int();
            printf("key2:\n");
            int k2 = get_int();

            KeyType1 key1 = (KeyType1){k1};
            KeyType2 key2 = (KeyType2){k2};

            Item * item = get_item(table, key1, key2);
            if (item == NULL) {
                return;
            }
            print_item(table->infoFileName, *item);
            break;
        }
            ///special search with 1 Key
        case 2: {
            printf("Which intKey do you have?\n1) key1\n2) key2\n");
            int chose_key = 0;
            while (chose_key < 1 || chose_key > 2) {
                chose_key = get_int();
            }
            printf("enter your intKey:\n");
            int k = get_int();

            ///with key1
            if (chose_key == 1) {
                KeyType1 key = (KeyType1){k};

                printf("Would you like to find every el with intKey or only one version?\n1) all\n2) version\n");
                int chose_all_or_version = 0;
                while (chose_all_or_version < 1 || chose_all_or_version > 2) {
                    chose_all_or_version = get_int();
                }

                ///find every element with key1
                if (chose_all_or_version == 1) {
                    Table * t = find_t1_k(table, key);
                    if (t) {
                        print_table(*t);
                        free_table(t);
                    }
                }
                    ///find 1 el with key1 and version
                else if (chose_all_or_version == 2) {
                    int version = -1;
                    printf("Enter your version\n");
                    while (version < 0)
                        version = get_int();

                    Table * t = find_t1_k_v(table, key, version);
                    if (t) {
                        print_table(*t);
                        free_table(t);
                    }
                }
            }
                ///with key2
            else if (chose_key == 2) {
                KeyType2 key = (KeyType2){k};

                printf("Would you like to find every el with intKey or only one version?\n1) all\n2) version\n");
                int chose_all_or_version = 0;
                while (chose_all_or_version < 1 || chose_all_or_version > 2) {
                    chose_all_or_version = get_int();
                }

                ///find every element with key2
                if (chose_all_or_version == 1) {
                    Table * t = find_t2_k(table, key);
                    if (t) {
                        print_table(*t);
                        free_table(t);
                    }
                }
                    ///find 1 el with key1 and version
                else if (chose_all_or_version == 2) {
                    int version = -1;
                    printf("Enter your version\n");
                    while (version < 0)
                        version = get_int();

                    Table * t = find_t2_k_v(table, key, version);
                    if (t) {
                        print_table(*t);
                        free_table(t);
                    }
                }
            } else
                return;
            break;
        }
        default:
            break;
    }
}