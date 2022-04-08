#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "KGetLine.h"
#include "time.h"
#include "MyString.h"
#include "all_structures.h"
#include "commands.h"
#include "dialog_table_functions.h"
#include "tests.h"
#include "file.h"

const char * TableName = "Table";
const char * InfoName = "Info";

void clrscr(){
    system("@cls||clear");
}

char ** init_menu_points() {
    char ** menu_points = malloc(sizeof(char*) * (UNKNOWN_COMMAND));
    menu_points[EXIT] = "exit";
    menu_points[ADD_NEW_EL] = "add a new element";
    menu_points[CLEAR_TABLE2] = "clear table 2";
    menu_points[DELETE_EL] = "delete an element";
    menu_points[FIND_EL] = "find an element";
    menu_points[PRINT_TABLE] = "print table";
    menu_points[DELETE_ALl] = "delete all";
    return menu_points;
}

void print_menu(char ** menu) {
    printf("This is menu:\n");
    for (int i = 0; i < UNKNOWN_COMMAND; ++i) {
        printf("%d) %s\n", i, menu[i]);
    }
}

bool check_command(char * command) {

    return true;
}

bool execute_command(Table ** table, Command command) {
    switch (command) {
        case EXIT: {
            printf("OK. Goodbye!\n");
            return true;
        }
        case ADD_NEW_EL: {
            add_dialog(*table);
            return false;
        }
        case FIND_EL: {
            find_dialog(*table);
            return false;
        }
        case DELETE_EL: {
            delete_dialog(*table);
            return false;
        }
        case PRINT_TABLE: {
            if (*table)
                print_table(**table);
            return false;
        }
        case CLEAR_TABLE2: {
            clear_table2(*table, true);
            return false;
        }
        case DELETE_ALl: {
            int s1 = (*table)->msize1;
            int s2 = (*table)->msize2;
            FILE * data_file = fopen((*table)->infoFileName, "wb");
            fclose(data_file);
            char * name1 = malloc(sizeof(char) * (1+strlen((*table)->infoFileName)));
            char * name2 = malloc(sizeof(char) * (1+strlen((*table)->tableName)));
            memcpy(name1, (*table)->infoFileName, sizeof(char) * (1+strlen((*table)->infoFileName)));
            memcpy(name2, (*table)->tableName, sizeof(char) * (1+strlen((*table)->tableName)));
            free_table(*table);
            *table = init_table(s1, s2);
            (*table)->infoFileName = name1;
            (*table)->tableName = name2;
            return false;
        }
        default: {
            printf("Your command is wrong!\n");
            return false;
        }
    }
    return false;
}

Command get_command_code(char * command) {
    if (is_digit(command)) {
        int x = atoi(command);
        if (x < 0 || x > UNKNOWN_COMMAND)
            return UNKNOWN_COMMAND;
        return x;
    }

    if (!strcmp(command, "exit"))
        return EXIT;
    else if (!strcmp(command, "find"))
        return FIND_EL;
    else if (!strcmp(command, "add"))
        return ADD_NEW_EL;
    else if (!strcmp(command, "delete"))
        return DELETE_EL;
    else if (!strcmp(command, "print"))
        return PRINT_TABLE;
    else if (!strcmp(command, "clear2"))
        return CLEAR_TABLE2;
    else if (!strcmp(command, "delete all"))
        return DELETE_ALl;
    else return UNKNOWN_COMMAND;
}

Table * start_session(int m1, int m2) {
    char * tableName = NULL;
    while (tableName == NULL || tableName[0] == '\0') {
        printf("enter TableFileName here: ");
        tableName = get_line();
    }
    char * dataFileName = NULL;
    while (dataFileName == NULL || dataFileName[0] == '\0') {
        printf("enter DataFileName here: ");
        dataFileName = get_line();
    }
    Table * table = read_table(tableName, dataFileName, 10, 10); //init_table(m1, m2);
    if (table == NULL) {
        fprintf(stderr, "READING ERROR\n");
        exit(1111);
    }
    table->tableName = tableName;
    table->infoFileName = dataFileName;
    return table;
}

int main() {
    printf("Info: %ld, Item: %ld, Node1: %ld, Node2: %ld\n", sizeof(InfoType), sizeof(Item), sizeof(Node1), sizeof(Node2));
    printf("KeySp1: %ld, KeySp2: %ld, KSC: %ld, Table: %ld\n", sizeof(KeySpace1), sizeof(KeySpace2), sizeof(KS2_coll), sizeof(Table));
    srand(time(NULL));
    bool finish = false;
    char ** menu = init_menu_points();
    int msize1 = 10;
    int msize2 = 10;
    Table * table = start_session(msize1, msize2);
    if (table == NULL)
        return 1;
    //TEST_ALL(table);

    while (!finish) {
        /// clrscr();
        print_menu(menu);

        char * command = get_line();
        if (command[0] == '\0') {
            free(command);
            continue;
        }
        //delete_double_spaces(&command);

        if (check_command(command) == false) {
            printf("Your command is wrong.");
            free(command);
            continue;
        }

        Command command_code = get_command_code(command);


        finish = execute_command(&table, command_code);

        free(command);
    }

    //remove(InfoName);
    //remove(TableName);
    write_table(table);
    free_table(table);
    free(menu);

    return 0;
}
