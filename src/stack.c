#include "defs.h"

bool stack_init(editstack_t* stack){
    if((stack->basePointer = malloc(CHUNKSIZE)))
        return true;
    else return false;
}

void stack_push(editstack_t* stack, edit_t* item){
    *(EDITSTACK_SP(stack)) = *item;
    ++(stack->numItems);
}

edit_t stack_pop(editstack_t* stack){
    --(stack->numItems);
    return *(EDITSTACK_SP(stack));
}