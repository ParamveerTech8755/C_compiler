#include "expression.h"
#include <stdio.h>
#include <stdlib.h>


Expression* initialize_expression(){
    Expression* expression = (Expression*)malloc(sizeof(Expression));
	return expression;
}

void generate_expression_asm(Expression *expression, FILE *file){
    //assuming i only have INT_LITERALS till now//
    if(expression->type == NODE_NUMBER){
        //move the number in accumulator
        fprintf(file, "\tmovl $%d, %%eax\n", expression->value);
    }
    else if(expression->type == NODE_OPERATOR){
        generate_expression_asm(expression->node.left, file);
        //now the result of the left sub tree is in acc

        if((expression->node.right)->type == NODE_OPERATOR){
            //push the result of the left subtree to the stack
            fprintf(file, "\tpush %%rax\n");

            //now perform operation on the right subtree and store result in accumulator
            generate_expression_asm(expression->node.right, file);


            //send the right tree result to B register and bring back the result of left subtree from stack to acc
            //essential for non-commutative operations like - and /
            fprintf(file, "\tmovl %%eax, %%ebx\n\tpop %%rax\n");
        }
        else if((expression->node.right)->type == NODE_NUMBER){
            fprintf(file, "\tmovl $%d, %%ebx\n", (expression->node.right)->value);
        }

        //now we have both the results n order eax <op> ebx
        switch(expression->node.op){
            //implement overflow error handling
            case '+':
                fprintf(file, "\taddl %%ebx, %%eax\n");
                break;
            case '-':
                fprintf(file, "\tsubl %%ebx, %%eax\n");
                break;
            case '*':
                fprintf(file, "\timull %%ebx, %%eax\n");
                break;
            case '/':
                fprintf(file, "\tcdq\n");
                fprintf(file, "\tidivl %%ebx\n");
                break;
            default:
                return;//error generating assembly
        }
        //result is stored in accumulator
        //check for any overflows here and call proper subroutine

    }
}

Expression* create_op_node(char op, Expression* leftChild, Expression* rightChild){
    Expression* exp = initialize_expression();
    exp->type = NODE_OPERATOR;
    exp->node.op = op;
    exp->node.left = leftChild;
    exp->node.right = rightChild;
    return exp;
}

Expression* create_number_node(int number){
    Expression* exp = initialize_expression();
    exp->type = NODE_NUMBER;
    exp->value = number;

    return exp;
}

Expression* create_identifier_node(token* tk){
    Expression* exp = initialize_expression();
    //do something with the token
    exp->type = NODE_ID;
    return exp;
}

void destory_expression(Expression** expression_ptr){
    free(*expression_ptr);
	*expression_ptr = NULL;
}
