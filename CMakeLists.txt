cmake_minimum_required(VERSION 3.16)
project(dual_file_table C)

set(CMAKE_C_STANDARD 99)

include_directories(Table Table/tables_structures libs libs/headers)

add_executable(dual_file_table main.c Table/Table.c Table/Table1.c Table/tables_structures/all_structures.h Table/Table2.c dialog_table_functions.h add.c libs/MyString.c libs/KGetLine.c commands.h tests.h tests.c find.c delete.c clear.c Table/file.c Table/tables_structures/file.h)
