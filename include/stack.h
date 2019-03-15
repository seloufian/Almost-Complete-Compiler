#ifndef stack_h_
#define stack_h_

#include "general.h"


typedef struct ElementStack{
	int info;
	struct ElementStack *next;
}ElementStack;


typedef struct Stack{
	int size;
	ElementStack *head;
}Stack;


Stack initStack();

bool emptyStack(Stack s);

void pushStack(Stack *s, int infoPush);

int popStack(Stack *s);

void destroyStack(Stack *s);

void displayStack(Stack s);

#endif
