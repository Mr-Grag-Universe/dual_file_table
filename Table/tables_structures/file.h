//
// Created by Stephan on 05.04.2022.
//

#ifndef DUAL_FILE_TABLE_FILE_H
#define DUAL_FILE_TABLE_FILE_H
bool write_table(Table * table);
Table * read_table(char * tableName, char * dataFileName, int def_size1, int def_size2);
void delete_item_from_file(char * name, Item * item);
void write_item(char * name, Item * item, InfoType * info);
InfoType * read_info(char * name, Item * item);
#endif //DUAL_FILE_TABLE_FILE_H
