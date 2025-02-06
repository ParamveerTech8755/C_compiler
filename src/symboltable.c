#include <stdlib.h>
#include <stdio.h>
#include "include/symboltable.h"
#include "include/customstring.h"

SymbolTable* create_symbol_table(){
    SymbolTable* smb_table = (SymbolTable*)malloc(sizeof(SymbolTable));
    for(int i = 0; i < HASHMAP_SIZE; i++)
        smb_table->symbol_list[i] = NULL;
    smb_table->scope = 0; //global
    return smb_table;
}

int string_hash(char* name){
    unsigned int hash = 0;
    while (*name)
        hash = (hash * 31) + *name++;  // 31 is a common prime used in hashing
    return hash%HASHMAP_SIZE;
}

int insert_symbol(SymbolTable *symbolTable, int stack_offset, token *tk, char* type){
    int index = string_hash(tk->value);
    // same identifiers are not allowed in the same scope
    Symbol* ptr = symbolTable->symbol_list[index];
    while(ptr && ptr->scope == symbolTable->scope){
        if(stringcmp(ptr->tk->value, tk->value))
            return EXIT_FAILURE;
        ptr = ptr->next;
    }
    Symbol *newSymbol = create_symbol(tk, stack_offset, type, symbolTable->scope);

    newSymbol->next = symbolTable->symbol_list[index];
    symbolTable->symbol_list[index] = newSymbol;

    return 0;
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
void mount_scope(SymbolTable *symbolTable){
    symbolTable->scope++;
}

void clear_scope(SymbolTable* symbolTable){
    //delete all the symbols with scope set as symbolTable->scope
    for(int i = 0; i < HASHMAP_SIZE; i++){
        Symbol *ptr = symbolTable->symbol_list[i], *q;
        while(ptr && ptr->scope == symbolTable->scope){
            q = ptr;
            ptr = ptr->next;
            free(q);
        }
        symbolTable->symbol_list[i] = ptr;
    }
    symbolTable->scope--;
}

Symbol* create_symbol(token* tk, int stack_offset, char* type, int scope){
    Symbol* newSymbol = (Symbol*)malloc(sizeof(Symbol));
    newSymbol->tk = tk;
    newSymbol->stack_offset = stack_offset;
    newSymbol->type = type;
    newSymbol->next = NULL;
    newSymbol->scope = scope;

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

void print_symbolTable(SymbolTable *symbolTable){
    for(int i = 0; i < HASHMAP_SIZE; i++){
        Symbol* cur = symbolTable->symbol_list[i];
        while(cur){
            printf("%s ", cur->tk->value);
            cur = cur->next;
        }
        if(cur)
            printf(":%d\n", i);
    }
}

void destroy_symbol_table(SymbolTable **symbolTable_ptr){
    for(int i = 0; i < HASHMAP_SIZE; i++){
        if((*symbolTable_ptr)->symbol_list[i])
            destroy_symbol((*symbolTable_ptr)->symbol_list[i]);
    }

    free(*symbolTable_ptr);
    *symbolTable_ptr = NULL;
}
