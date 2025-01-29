#include "expression.h"
#include <stdio.h>
#include <stdlib.h>
#include "../utils.h"


Expression* initialize_expression(){
    Expression* expression = (Expression*)malloc(sizeof(Expression));
	return expression;
}

void generate_expression_asm(Expression *expression, FILE *file){
    //assuming i only have INT_LITERALS till now//
    if(expression->type == NODE_ID){
        //find the location of the variable
        fprintf(file, "\tmovl location, %%eax\n");
        return;
    }
    else if(expression->type == NODE_NUMBER){
        //move the number in accumulator
        fprintf(file, "\tmovl $%d, %%eax\n", expression->value);
        return;
    }
    else if(expression->type == NODE_ASGN){
        //evaluate the value that has to be assigned to the variable
        generate_expression_asm(expression->node.asign, file);

        //get the memory location of the variable

        token* asign_operator = expression->node.tk;
        if(asign_operator->type != TOKEN_OP_ASGN){
            switch(asign_operator->type){
                case TOKEN_OP_ADD_ASGN:
                    fprintf(file, "\taddl location, %%eax\n");
                    break;
                case TOKEN_OP_SUB_ASGN:
                    fprintf(file, "\tsubl location, %%eax\n");
                    break;
                case TOKEN_OP_MUL_ASGN:
                    fprintf(file, "\timull location, %%eax\n");
                    break;
                case TOKEN_OP_DIV_ASGN:
                    fprintf(file, "\tmovl %%eax, %%ebx\n\tmovl location, %%eax\n\tcdq\n\tidivl %%ebx\n");
                    break;
                case TOKEN_OP_MOD_ASGN:
                    fprintf(file, "\tmovl %%eax, %%ebx\n\tmovl location, %%eax\n\tcdq\n\tidivl %%ebx\n");
                    //result in %edx unlike the rest cases
                    fprintf(file, "\tmovl %%edx, location\n");
                    return;
                case TOKEN_OP_BIT_XOR_ASGN:
                    fprintf(file, "\txorl location, %%eax\n");
                    break;
                case TOKEN_OP_BIT_AND_ASGN:
                    fprintf(file, "\tandl location, %%eax\n");
                    break;
                case TOKEN_OP_BIT_OR_ASGN:
                    fprintf(file, "\torl location, %%eax\n");
                    break;
                default:
                    return;
            }
        }
        fprintf(file, "\tjo _overflow\n\tmovl %%eax, location\n");
    }
    else if(expression->type == NODE_BINARY_OPERATOR){
        generate_expression_asm(expression->node.left, file);
        //now the result of the left sub tree is in acc
        if((expression->node.tk)->type == TOKEN_OP_AND){
            // short circuit mechanism for &&...right expression must only be evaluated if there is ambiguity after the first one
            unsigned int x = getUniqueInt();
            fprintf(file, "\ttestl %%eax, %%eax\n\tje .end%u\n", x);

            //here comes code for expression 2
           generate_expression_asm(expression->node.right, file);
           fprintf(file, "\ttestl %%eax, %%eax\n\tje .end%u\n", x);
           fprintf(file, "\tmovl $1, %%eax\n.end%u:\n", x);

           return;
        }
        else if((expression->node.tk)->type == TOKEN_OP_OR){
            //short circuit for ||
            unsigned int x = getUniqueInt();
            fprintf(file, "\ttestl %%eax, %%eax\n\tje .right_exp%u\n", x);
            fprintf(file, "\tmovl $1, %%eax\n\tjmp .end%u\n", x);
            //left expression evaluates to zero
            //code for right expression
            fprintf(file, ".right_exp%u:\n", x);
            generate_expression_asm(expression->node.right, file);
            fprintf(file, "\ttestl %%eax, %%eax\n\tmovl $0, %%eax\n\tsetne %%al\n.end%u:\n", x);

            return;
        }
        else if((expression->node.right)->type == NODE_BINARY_OPERATOR || (expression->node.right)->type == NODE_UNARY_OPERATOR){
            //push the result of the left subtree to the stack
            fprintf(file, "\tpush %%rax\n");

            //now perform operation on the right subtree and store result in accumulator
            generate_expression_asm(expression->node.right, file);


            //send the right tree result to B register and bring back the result of left subtree from stack to acc
            //essential for non-commutative operations like - and /
            if(isNonCommutative(expression->node.tk->type))
                fprintf(file, "\tmovl %%eax, %%ebx\n\tpop %%rax\n");
            else{
                fprintf(file, "\tpop %%rbx\n");//commutative
            }
        }
        else if((expression->node.right)->type == NODE_NUMBER){
            fprintf(file, "\tmovl $%d, %%ebx\n", (expression->node.right)->value);
        }

        //now we have both the results n order eax <op> ebx
        switch((expression->node.tk)->type){
            //implement overflow error handling
            case TOKEN_OP_ADD:
                fprintf(file, "\taddl %%ebx, %%eax\n");
                break;
            case TOKEN_OP_SUB:
                fprintf(file, "\tsubl %%ebx, %%eax\n");
                break;
            case TOKEN_OP_MUL:
                fprintf(file, "\timull %%ebx, %%eax\n");
                break;
            case TOKEN_OP_DIV:
                fprintf(file, "\tcdq\n");
                fprintf(file, "\tidivl %%ebx\n");
                break;
            case TOKEN_OP_MOD:
                fprintf(file, "\tcdq\n");
                fprintf(file, "\tidivl %%ebx\n\tmovl %%edx, %%eax");//move the remainder to accumulator
                break;
            case TOKEN_OP_BIT_XOR:
                fprintf(file, "\txorl %%ebx, %%eax\n");
                break;
            case TOKEN_OP_EQUALS:
                fprintf(file, "\tcmpl %%ebx, %%eax\n");
                fprintf(file, "\tmovl $0, %%eax\n\tsete %%al\n");
                break;
            case TOKEN_OP_NOT_EQL:
                fprintf(file, "\tcmpl %%ebx, %%eax\n");
                fprintf(file, "\tmovl $0, %%eax\n\tsetne %%al\n");
                break;
            case TOKEN_OP_GRT:
                fprintf(file, "\tcmpl %%ebx, %%eax\n");
                fprintf(file, "\tmovl $0, %%eax\n\tsetg %%al\n");
                break;
            case TOKEN_OP_GRT_EQL:
                fprintf(file, "\tcmpl %%ebx, %%eax\n");
                fprintf(file, "\tmovl $0, %%eax\n\tsetge %%al\n");
                break;
            case TOKEN_OP_LSR:
                fprintf(file, "\tcmpl %%ebx, %%eax\n");
                fprintf(file, "\tmovl $0, %%eax\n\tsetl %%al\n");
                break;
            case TOKEN_OP_LSR_EQL:
                fprintf(file, "\tcmpl %%ebx, %%eax\n");
                fprintf(file, "\tmovl $0, %%eax\n\tsetle %%al\n");
                break;
            //&& and || have been take care of already
            default:
                return;//error generating assembly
        }

    }
    else if(expression->type == NODE_UNARY_OPERATOR){
        generate_expression_asm(expression->node.child, file);
        switch((expression->node.tk)->type){
            case TOKEN_OP_BIT_NOT:
                fprintf(file, "\tnotl %%eax\n");
                break;
            case TOKEN_OP_NOT:
                fprintf(file, "\ttest %%eax, %%eax\n");
                fprintf(file, "\tmovl %%eax, %%eax\n\tsete %%al\n");
                break;
            case TOKEN_OP_SUB:
                fprintf(file, "\tneg %%eax\n");
                break;
            default:
                return;//unknown// not reachable
        }
    }
    //result is stored in accumulator
    //check for overflow errors
    fprintf(file, "\tjo _overflow\n");

}

Expression* create_bop_node(token* tk, Expression* leftChild, Expression* rightChild){
    Expression* exp = initialize_expression();
    exp->type = NODE_BINARY_OPERATOR;
    exp->node.tk = tk;
    exp->node.left = leftChild;
    exp->node.right = rightChild;
    return exp;
}

Expression* create_uop_node(token* tk, Expression* child){
    Expression* exp = initialize_expression();
    exp->type = NODE_UNARY_OPERATOR;
    exp->node.tk = tk;
    exp->node.child = child;

    return exp;
}

Expression* create_asign_node(token* operator, token* variable, Expression* assignment){
    Expression* exp = initialize_expression();
    exp->type = NODE_ASGN;
    exp->node.tk = operator;
    exp->node.var = variable;
    exp->node.asign = assignment;

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
    exp->identifier = tk;
    exp->type = NODE_ID;
    return exp;
}

void destroy_expression(Expression** expression_ptr){
    Expression* exp = *expression_ptr;

    if(exp->type == NODE_BINARY_OPERATOR){
        destroy_expression(&exp->node.left);
        destroy_expression(&exp->node.right);
    }
    else if(exp->type == NODE_UNARY_OPERATOR){
        destroy_expression(&exp->node.child);
    }
    else if(exp->type == NODE_ASGN){
        destroy_expression(&exp->node.asign);
    }

    free(*expression_ptr);
	*expression_ptr = NULL;
}
