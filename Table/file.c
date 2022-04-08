//
// Created by Stephan on 05.04.2022.
//
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "all_structures.h"

static const char * TableName = "Table";
static const char * InfoName = "Info";

void write_item(char * name, Item * item, InfoType * info) {
    if (item == NULL)
        return;

    FILE * data_file = fopen(name, "ab");
    bool flag = true;
    item->infoPos = ftell(data_file) + sizeof(Item) + sizeof(bool);
    fwrite(&flag, sizeof(bool), 1, data_file);
    fwrite(item, sizeof(Item), 1, data_file);
    fwrite(info, sizeof(InfoType), 1, data_file);
    fwrite(info->data, item->infoSize, 1, data_file);
    fclose(data_file);
}

InfoType * read_info(char * name, Item * item) {
    if (item == NULL)
        return NULL;

    FILE * data_file = fopen(name, "rb");
    fseek(data_file, (long) (item->infoPos- sizeof(Item)- sizeof(bool)), SEEK_SET);

    bool flag = true;

    fread(&flag, sizeof(bool), 1, data_file);
    if (!flag)
        return NULL;
    fseek(data_file, (long)sizeof(Item), SEEK_CUR);

    InfoType * info = malloc(sizeof(InfoType));

    fread(info, sizeof(InfoType), 1, data_file);
    info->data = malloc(info->infoSize);
    fread(info->data, info->infoSize, 1, data_file);

    fclose(data_file);
    return info;
}

bool write_table(Table * table) {
    printf("Items: %d\n", table->numberOfItems);
    FILE * table_file = fopen(table->tableName, "wb");
    //rewind(table_file);
    fwrite(table, sizeof(Table), 1, table_file);
    fclose(table_file);
    return true;
}

void delete_item_from_file(char * name, Item * item) {
    if (item == NULL)
        return;

    FILE * data_file = fopen(name, "r+b");
    bool flag = false;

    long pos = (long) (item->infoPos- sizeof(Item)- sizeof(bool));
    printf("%ld: \n", ftell(data_file));
    rewind(data_file);
    fseek(data_file, pos, SEEK_SET);
    fwrite(&flag, sizeof(bool), 1, data_file);

    fclose(data_file);
}

Table * read_table(char * tableName, char * dataFileName, int def_size1, int def_size2) {
    FILE * table_file = fopen(tableName, "rb");
    if (table_file == NULL) {
        remove(tableName);
        Table * table = init_table(def_size1, def_size2);
        return table;
    }
    Table * table = malloc(sizeof(Table));
    fread(table, sizeof(Table), 1, table_file);
    fclose(table_file);

    table->table1 = init_table1(table->msize1);
    table->table2 = init_table2(table->msize2);

    int check_num = table->numberOfItems;
    table->numberOfItems = 0;

    //print_table(*table);
    //getchar();

    FILE * data_file = fopen(dataFileName, "rb");
    if (data_file == NULL)
        return table;

    fseek(data_file, 0, SEEK_END);
    long end = ftell(data_file);
    fseek(data_file, 0, SEEK_SET);
    long curr_pos = 0;
    while (curr_pos < end) {
        bool flag = false;
        fread(&flag, sizeof(bool), 1, data_file);
        Item * item = malloc(sizeof(Item));
        fread(item, sizeof(Item), 1, data_file);
        size_t size = item->infoSize;
        if (!flag) {
            fseek(data_file, (long) (size + sizeof(InfoType)), SEEK_CUR);
            curr_pos += (long)(sizeof(Item)+ sizeof(InfoType)+ sizeof(bool)+size);
            free_item(item);
            continue;
        }
        //item->info = malloc(sizeof(InfoType));
        //fread(item->info, sizeof(InfoType), 1, data_file);
        item->infoPos = ftell(data_file);
        fseek(data_file, sizeof(InfoType), SEEK_CUR);
        fseek(data_file, (long) item->infoSize, SEEK_CUR);
        //item->info->data = malloc(item->infoSize);
        //fread(item->info, sizeof(InfoType), 1, data_file);
        //fread(item->info->data, item->info->infoSize, 1, data_file);
        add_item(table, item);
        curr_pos += (long)(sizeof(Item)+ sizeof(InfoType)+ sizeof(bool)+item->infoSize);//ftell(data_file);
    }
    fclose(data_file);

    if (check_num != table->numberOfItems) {
        printf("NUMBERS: %d, %d", check_num, table->numberOfItems);
        remove(tableName);
        remove(dataFileName);
        free_table(table);
        return NULL;
    }

    return table;
}