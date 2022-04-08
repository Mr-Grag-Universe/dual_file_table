//
// Created by Stephan on 03.04.2022.
//
#include "all_structures.h"
#include "KGetLine.h"
#include "MyString.h"
#include "string.h"
#include "file.h"

InfoType * enter_info() {
    InfoType * info = malloc(sizeof(InfoType));
    printf("Enter your data here:\n-> ");
    info->data = get_line();
    info->infoSize = sizeof(char) * (strlen(info->data)+1);
    return info;
}

void add_dialog(Table * table) {
    KeyType1 key1 = enter_key1();
    KeyType2 key2 = enter_key2();

    printf("enter your info.\n");
    InfoType * info = enter_info();       // считываем введенную информацию

    Item * item = create_item(info, key1, key2);
    if (!add_item(table, item)) {
        free_item(item);
        printf("sorry, we can't add that\n");
    } else {
        write_item(table->infoFileName, item, info);
    }
    if (item) {
        if (info->data)
            free(info->data);
        free(info);
    }
}