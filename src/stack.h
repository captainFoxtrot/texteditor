#ifndef EDITSTACK_H
#define EDITSTACK_H

#include "defs.h"

void stack_init(editstack_t* stack);

void stack_push(editstack_t* stack, void* item);

edit_t stack_pop(editstack_t* stack);

#endif