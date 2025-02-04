#include "statement.h"
#include "expression.h"
#include "../symboltable.h"
#include "../utils.h"
#include <stdlib.h>
#include <stdio.h>

Statement* initialize_statement(){
	Statement* statement = (Statement*)malloc(sizeof(Statement));
	statement->expression = NULL;

	return statement;
}

void generate_statement_asm(Statement* statement, unsigned int x, FILE* file){
    switch(statement->type){
        case RETURN:
            generate_return_statement_asm(statement, x, file);
            break;
            //break; once conditional statements are implemented
        case DECLARATION:
            generate_declaration_statement_asm(statement, file);
            break;
        case ASSIGNMENT:
            generate_assignment_statement_asm(statement, file);
            break;
        case IF:
            generate_if_statement_asm(statement, x, file);
            break;
        case EXPRESSION:
            generate_expression_asm(statement->expression, file);
            break;
        default:
            break;
    }
}

//utility function
void generate_assignment_asm(Expression* id, Expression* exp, FILE* file){
    char ch;
    if(exp->type == NODE_NUMBER){
        ch = 'l';
        if(id->identifier.sizeInBytes == CHAR_SIZE)
            ch = 'b';
        fprintf(file, "\tmov%c $%d, %d(%%rbp)\n", ch, exp->value, id->identifier.stack_offset);
    }
    else if(exp->type == NODE_ID){
        int var_offset = id->identifier.stack_offset;
        int asign_offset = exp->identifier.stack_offset;
        char* regs[] = {"%eax", "%al"};
        int reg = 1;
        ch = 'b';

        if(id->identifier.sizeInBytes != exp->identifier.sizeInBytes){
            //only the least significant byte of the number has to be moved
            if(exp->identifier.sizeInBytes == CHAR_SIZE){
                fprintf(file, "\tmovl $0, %d(%%rbp)\n", var_offset);
                var_offset += INT_SIZE - CHAR_SIZE;
            }
            else if(exp->identifier.sizeInBytes == INT_SIZE)
                asign_offset += INT_SIZE - CHAR_SIZE;
        }
        else if(id->identifier.sizeInBytes == INT_SIZE){
            ch = 'l';
            reg = 0;
        }
        fprintf(file, "\tmov%c %d(%%rbp), %s\n\tmov%c %s, %d(%%rbp)\n", ch, asign_offset, regs[reg], ch, regs[reg], var_offset);
    }
    else if(exp->type == NODE_CHAR){
        ch = 'b';
        if(id->identifier.sizeInBytes == INT_SIZE)
            ch = 'l';
        fprintf(file, "\tmov%c $'%s', %d(%%rbp)\n", ch, exp->ch, id->identifier.stack_offset);
    }
    else{
        generate_expression_asm(exp, file);
        fprintf(file, "\tjo _overflow\n\tmovl %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
    }

}

void generate_if_statement_asm(Statement* statement, unsigned int y, FILE* file){
    unsigned int x = getUniqueInt();
    generate_expression_asm(statement->conditional.expression, file);
    fprintf(file, "\tcmpl $0, %%eax\n\tjz false%u\n", x);
    generate_statement_asm(statement->conditional.when_true, y, file);
    int has_else = statement->conditional.when_false != NULL;
    if(has_else)
        fprintf(file, "\tjmp end%u\n", x);
    fprintf(file, "false%u:\n", x);
    if(has_else){
        generate_statement_asm(statement->conditional.when_false, y, file);
        fprintf(file, "end%u:\n", x);
    }
    return;
}

void generate_declaration_statement_asm(Statement* statement, FILE* file){
    if(statement->expression->type == NODE_ASGN)
        generate_assignment_asm(statement->expression->node.var, statement->expression->node.asign, file);

    //shift the stackOffset
    //else expression->type is NODE_ID
    //space has already been made in the stack, nothing to do
}

void generate_assignment_statement_asm(Statement *statement, FILE *file){
    Expression* id = statement->expression->node.var;//this is an identifier node
    Expression* exp = statement->expression->node.asign;

    //destination is the same.. only the source is affected and the command

    if(statement->expression->node.tk->type != TOKEN_OP_ASGN){
        generate_expression_asm(exp, file);
        //output in %eax
        switch(statement->expression->node.tk->type){
            case TOKEN_OP_MUL_ASGN:
                fprintf(file, "\timull %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
                break;
            case TOKEN_OP_DIV_ASGN:
                fprintf(
                    file,
                    "\tmovl %%eax, %%ebx\n\tmovl %d(%%rbp), %%eax\n\tcdq\n\tidivl %%ebx\n\tmovl %%eax, %d(%%rbp)\n",
                    id->identifier.stack_offset,
                    id->identifier.stack_offset
                );
                break;
            case TOKEN_OP_SUB_ASGN:
                fprintf(file, "\tsubl %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
                break;
            case TOKEN_OP_MOD_ASGN:
                fprintf(
                    file,
                    "\tmovl %%eax, %%ebx\n\tmovl %d(%%rbp), %%eax\n\tcdq\n\tidivl %%ebx\n\tmovl %%edx, %d(%%rbp)\n",
                    id->identifier.stack_offset,
                    id->identifier.stack_offset
                );
                break;
            case TOKEN_OP_ADD_ASGN:
                fprintf(file, "\taddl %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
                break;
            case TOKEN_OP_BIT_XOR_ASGN:
                fprintf(file, "\txorl %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
                break;
            case TOKEN_OP_BIT_AND_ASGN:
                fprintf(file, "\tandl %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
                break;
            case TOKEN_OP_BIT_OR_ASGN:
                fprintf(file, "\torl %%eax, %d(%%rbp)\n", id->identifier.stack_offset);
                break;
            default:
                return;
        }
        fprintf(file, "\tjo _overflow\n");
    }
    else{
        generate_assignment_asm(id, exp, file);
    }
}

void generate_return_statement_asm(Statement *statement, unsigned int x, FILE *file){
    if(statement->expression != NULL){
        if(statement->expression->type == NODE_CHAR)//constant type expression
            fprintf(file, "\tmovl $'%s',\t%%eax\n", statement->expression->ch);
        else
            generate_expression_asm(statement->expression, file);
    }
    if(x > 0)
        fprintf(file, "\tjmp return%u\n", x);
}

void destroy_statement(Statement** statement_ptr){
	//maybe will have to use destroy_expression later
	free((*statement_ptr)->expression);
	free(*statement_ptr);
	*statement_ptr = NULL;
}
