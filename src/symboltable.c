#include <stdlib.h>
#include "include/symboltable.h"
#include "include/customstring.h"

SymbolTable* create_symbol_table(){
    SymbolTable* smb_table = (SymbolTable*)malloc(sizeof(SymbolTable));
    for(int i = 0; i < HASHMAP_SIZE; i++)
        smb_table->symbol_list[i] = NULL;
    return smb_table;
}

int string_hash(char* name){
    unsigned int hash = 0;
    while (*name)
        hash = (hash * 31) + *name++;  // 31 is a common prime used in hashing
    return hash%HASHMAP_SIZE;
}

void insert_symbol(SymbolTable *symbolTable, int stack_offset, token * tk, char* type){
    int index = string_hash(tk->value);

    Symbol *newSymbol = create_symbol(tk, stack_offset, type);
    newSymbol->next = symbolTable->symbol_list[index];
    symbolTable->symbol_list[index] = newSymbol;
}

Symbol* lookup_symbol_table(SymbolTable *symbolTable, char *identifier){
    int index = string_hash(identifier);

    Symbol* cur = symbolTable->symbol_list[index];
    while(cur){
        if(stringcmp(cur->tk->value, identifier))
            return cur;
        cur = cur->next;
    }

    return NULL;
}

Symbol* create_symbol(token* tk, int stack_offset, char* type){
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->tk = tk;
    newSymbol->stack_offset = stack_offset;
    newSymbol->type = type;
    newSymbol->next = NULL;

    return newSymbol;
}

void destroy_symbol(Symbol* symbol){
    if(symbol->next != NULL)
        destroy_symbol(symbol->next);

    free(symbol);
}

int findSize(char* data_type){
    int size = INT_SIZE;
    if(stringcmp(data_type, "char"))
        size = CHAR_SIZE;
    //else if// other types
    return size;
}

void destroy_symbol_table(SymbolTable **symbolTable_ptr){

    for(int i = 0; i < HASHMAP_SIZE; i++){
        if((*symbolTable_ptr)->symbol_list[i])
            destroy_symbol((*symbolTable_ptr)->symbol_list[i]);
    }

    free(*symbolTable_ptr);
    *symbolTable_ptr = NULL;
}
