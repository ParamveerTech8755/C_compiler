#include "function.h"
#include "statement.h"

Function* initialize_function(char* return_type, char* name/*, PARAMETER** parameters*/){
	Function* function = (Function*)malloc(sizeof(Function));

	function->index = 0;
	function->capacity = 80;//initial capacity of statements.
	function->STATEMENT_LIST = (Statement**)calloc(function->capacity, sizeof(Statement*));
	function->name = name;
	function->return_type = return_type;

	return function;
}

void push_statement(Function* function, Statement* statement){
	if(function->index >= function->capacity){
		function->capacity *= 2;
		//assuming the capacity doesnt overflow int data type
		Statement** temp = (Statement**)calloc(function->capacity, sizeof(Statement*));
		for(int i = 0; i < function->index; i++)
			*(temp+i) = *(function->STATEMENT_LIST+i);

		free(function->STATEMENT_LIST);
		function->STATEMENT_LIST = temp;
	}

	*(function->STATEMENT_LIST + function->index++) = statement;
}

int generate_function_code(Function *function, FILE *file){
    fprintf(file, function->name);
    fprintf(file, ":\n");

    for(int i = 0; i < function->index; i++){
        //go thru all the statements.. fine
        if(function->STATEMENT_LIST[i]->type == RETURN){
            //let it be a char or int..both can be taken care of by int
            //no support for pointers yet so cannot return strings
            Expression* exp = function->STATEMENT_LIST[i]->expression;

            if(exp == NULL){
                fprintf(file, "\tret");
            }
            else if(exp->constant != NULL){//constant type expression
                fprintf(file, "\tmovl\t$");

                // string* constant = (string*)malloc(sizeof(string));
                if(exp->constant->type == INT){
                    string* constant = toString(exp->constant->value.number);
                    fprintf(file, constant->str);
                    fprintf(file, ",\t%%eax\n\tret");
                }
                else if(exp->constant->type == CHAR){
                    char* constant = convertCharToCString(exp->constant->value.ch);
                    fprintf(file, "\'");
                    fprintf(file, constant);
                    fprintf(file, "\',\t%%eax\n\tret");
                }
            }
            else{
                int eval = evaluate_expression(function->STATEMENT_LIST[i]->expression, file);
                //they might print something to the file too.. fine
                fprintf(file, "\tmovl\t[move from here], %%eax\n\tret");
            }
            return 0;
        }
    }
    return 0;
}


void destroy_function(Function** function_ptr){
	//no need to free return_type and name
	//as they are already freed by destory_lexer => destroy_token

	//destroy parameters

	Function* function = *function_ptr;
	//freeing the statement list
	for(int i = 0; i < function->index; i++)
		destroy_statement(function->STATEMENT_LIST+i);

	free(function->STATEMENT_LIST);
	free(function);

	*function_ptr = NULL;
}
