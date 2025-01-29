#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define HASHMAP_SIZE 10 //for now

#include "token.h"

typedef struct Symbol_Struct{
    token* tk;
    int stack_offset;
    // int64_t memory_address; for absolute memory addresses
    char* type;
    struct Symbol_Struct* next;
} Symbol;

typedef struct Symbol_Table_Struct{
    Symbol* symbol_list[HASHMAP_SIZE];
}SymbolTable;

void insert_symbol(SymbolTable*, int, token*, char*);

Symbol* create_symbol(token*, int, char*);

Symbol* lookup_symbol_table(SymbolTable*, char*);

SymbolTable* create_symbol_table();

void destroy_symbol(Symbol*);

void destroy_symbol_table(SymbolTable**);

#endif
