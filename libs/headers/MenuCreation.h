#pragma once

#ifndef MENU
#define MENU

typedef enum FNames {
	EXIT, // = 0
	F1, // = 3
	F2 // = 4
} FName;

typedef struct Functions{
	char * name;
	FName enum_name;
} Function;

typedef struct Folders{
	char * name;
	struct Folders ** folders;
	struct Folders * previous;
	// char ** functions;
	Function * functions;
} Folder;

typedef struct Menus{
	Folder * root;
	Folder ** all_folders;
	int nof;
} Menu;

Menu init_menu();
int number_of_folders(Folder folder);
int number_of_functions(Folder folder);
void add_folder(char * name, char * host_name, Menu * menu);
Menu create_menu(char * file_name);
void add_function(char * name, char * host_name, Menu menu);
void delete_all_folders(Menu * menu);
void clear_root(Menu * menu);
void print_menu(Folder * f);
#endif