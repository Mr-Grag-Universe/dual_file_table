//
// Created by Stephan on 04.04.2022.
//
#include "all_structures.h"
#include "MyString.h"

void delete_t1_k_v(Table * table, KeyType1 key, int version) {
    KeySpace1 * ks = get_ks1(table->table1, key);
    if (ks == NULL)
        return;

    if (ks->node == NULL || ks->node->release < version) {
        return;
    }

    Node1 * node = ks->node;
    // version = node->release.numberOfRelease - version;
    while (node && node->release != version) {
        node = node->next;
    }

    if (node == NULL)
        return;

    delete_item(table, key, node->info->key2, true);
}

void delete_all_k1(Table * table, KeyType1 key1) {
    KeySpace1 * ks = get_ks1(table->table1, key1);
    if (ks == NULL) {
        printf("there is no such intKey\n");
        return;
    }
    int number = (ks->node)?ks->node->release+1: 0;
    int i = 0;
    KeyType2 * keys = malloc(sizeof(KeyType2) * number);
    Node1 * node = ks->node;
    while (node) {
        keys[i].intKey = node->info->key2.intKey;
        i++;
        node = node->next;
    }
    for (int j = 0; j < number; ++j)
        delete_item(table, key1, keys[j], true);
    free(keys);
}

///return table with new table
///with el with key2 and it's version
void delete_all_k2(Table * table, KeyType2 key) {
    KeySpace2 * ks = get_ks2(get_ksc(table->table2, get_hash(table->table2, key)), key);
    if (ks == NULL) {
        printf("there is no such intKey\n");
        return;
    }
    //int number = number_of_nodes2(ks->node);
    int i = 0;
    if (ks->node == NULL)
        return;
    if (ks->node == NULL)
        return;
    int number = ks->node->release+1;
    KeyType1 * keys = malloc(sizeof(KeyType1) * number);
    Node2 * node = ks->node;
    while (node) {
        keys[i].intKey = node->info->key1.intKey;
        i++;
        node = node->next;
    }
    for (int j = 0; j < number; ++j)
        delete_item(table, keys[j], key, true);
    free(keys);
}

void delete_t2_k_v(Table * table, KeyType2 key, int version) {
    KeySpace2 * ks = get_ks2(get_ksc(table->table2, get_hash(table->table2, key)), key);
    if (ks == NULL) {
        printf("there is no such intKey\n");
        return;
    }
    Node2 * node = ks->node;
    Node2 * pr_n = ks->node;
    if (node == NULL)
        return;
    if (node->release < version) {
        printf("too big version\n");
        return;
    }
    // version = node->release.numberOfRelease - version;
    KeyType1 key1 = (KeyType1) {0};
    while (node) {
        if (node->release == version) {
            key1.intKey = node->info->key1.intKey;
            break;
        }
        pr_n = node;
        node = node->next;
    }
    if (node == NULL) {
        printf("not found\n");
        return;
    }
    delete_item(table, key1, key, true);
}

void delete_dialog(Table * table) {
    /*
     * KeyType1 key1 = enter_key1();
    KeyType2 key2 = enter_key2();

    if (!delete_item(table, key1, key2)) {
        printf("sorry, we can't delete that\n");
    } else {
        printf("deleted successfully\n");
    }*/
    printf("What do you wanna to delete?\n1) El with 2 keys\n2) Els with 1 intKey");
    int chose_2_or_1_key = -1;
    while (chose_2_or_1_key < 1 || chose_2_or_1_key > 2) {
        printf("\n");
        chose_2_or_1_key = get_int();
    }

    switch (chose_2_or_1_key) {
        case 1: {
            printf("enter keys of this element:\n");
            printf("key1:\n");
            int k1 = get_int();
            printf("key2:\n");
            int k2 = get_int();

            KeyType1 key1 = (KeyType1){k1};
            KeyType2 key2 = (KeyType2){k2};

            if (get_item(table, key1, key2) == NULL) {
                printf("there is not such element in table\n");
                return;
            }

            delete_item(table, key1, key2, true);
            break;
        }
        case 2: {
            printf("Which intKey do you have?\n1) key1\n2) key2\n");
            int chose_key = 0;
            while (chose_key < 1 || chose_key > 2) {
                chose_key = get_int();
            }

            printf("enter your intKey:\n");
            int k = get_int();
            printf("Would you like to delete every el with intKey or only one version?\n1) all\n2) version\n");
            int chose_all_or_version = 0;
            while (chose_all_or_version < 1 || chose_all_or_version > 2) {
                chose_all_or_version = get_int();
            }

            if (chose_key == 1) {
                KeyType1 key = (KeyType1){k};
                if (chose_all_or_version == 1) {
                    delete_all_k1(table, key);
                } else if (chose_all_or_version == 2) {
                    int version = -1;
                    printf("Enter your version");
                    while (version < 0) {
                        printf("\n");
                        version = get_int();
                    }
                    delete_t1_k_v(table, key, version);
                }
            } else if (chose_key == 2) {
                KeyType2 key = (KeyType2){k};
                if (chose_all_or_version == 1) {
                    delete_all_k2(table, key);
                }
                else {
                    int version = -1;
                    printf("Enter your version\n");
                    while (version < 0) {
                        version = get_int();
                    }
                    delete_t2_k_v(table, key, version);
                }
            } else
                return;
            break;
        }
        default:
            break;
    }
}