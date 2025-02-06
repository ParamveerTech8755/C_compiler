#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define HASHMAP_SIZE 10 //for now

#define INT_SIZE 4
#define CHAR_SIZE 1

#include "token.h"

typedef struct Symbol_Struct{
    token* tk;
    int stack_offset;
    // int64_t memory_address; for absolute memory addresses
    char* type;
    int scope; //0 -> global scope, 1, 2, 3 -> deeply nested scope and so on
    struct Symbol_Struct* next;
} Symbol;

typedef struct Symbol_Table_Struct{
    Symbol* symbol_list[HASHMAP_SIZE];
    int scope;
}SymbolTable;

int insert_symbol(SymbolTable*, int, token*, char*);

Symbol* create_symbol(token*, int, char*, int);

Symbol* lookup_symbol_table(SymbolTable*, char*);

SymbolTable* create_symbol_table();

void destroy_symbol(Symbol*);

void clear_scope(SymbolTable*);

int findSize(char*);

void mount_scope(SymbolTable*);

void print_symbolTable(SymbolTable*);

void destroy_symbol_table(SymbolTable**);

#endif
