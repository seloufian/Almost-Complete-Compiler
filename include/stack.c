#include "stack.h"

Stack initStack(){
	Stack s = { 0, NULL };
	return(s);
}


bool emptyStack(Stack s){
	return(s.size == 0 ? true : false);
}


void pushStack(Stack *s, int infoPush){

	ElementStack *e = malloc(sizeof(ElementStack));

	e->info = infoPush;
	e->next = s->head;

	s->head = e;

	(s->size)++;
}


int popStack(Stack *s){
	/* Stack must contain AT LEAST one element */
	int returnValue = (s->head)->info;
	/* Save current "head" */
	ElementStack *e = (s->head);
	/* "head" will point on the next value */
	(s->head) = (s->head)->next;
	/* Free old head */
	free(e);
	/* An element was removed, decrement stack's size */
	(s->size)--;

	return(returnValue);
}


void destroyStack(Stack *s){
	ElementStack *e;

	while(s->size){
		e = s->head;

		s->head = (s->head)->next;
		(s->size)--;

		free(e);
	}
}


void displayStack(Stack s){
	printf("\n\nStack : (Size = %d)   ", s.size);
	while(s.head != NULL){
		printf("%d  ", (s.head)->info);
		s.head = (s.head)->next;
	}
	printf("\n\n");
}
