#include <stdio.h>
#include <stdlib.h>
#define NUM_STUDENTS 4

typedef struct student{
    char * forename;
    char * surname;
    float average_module_mark;
} student;

typedef struct llitem{
	struct llitem *previous;
	struct llitem *next;
	void* record;
} llitem;

void print_student(const student * s);

void (*print_callback)(void* r) = NULL;

void print_items(llitem *ll_start, void print_student(const student *)){
	llitem *ll = ll_start;
	while (ll != NULL){
		//printf
		if (print_callback != NULL)
			print_callback(ll->record);
		print_student((const student *) ll->record);
		//next
		ll = ll->next;
	}
}

llitem* create_linked_list(void * r){
	llitem* ll_start;

	ll_start = (llitem*)malloc(sizeof(llitem));
	ll_start->next = NULL;
	ll_start->previous = NULL;
	ll_start->record = r;
	return ll_start;
}

llitem* add_to_linked_list(llitem* ll_end, void * r){
	// check end
	if (ll_end->next != NULL) return NULL;

	// add to the end of list
	ll_end->next = create_linked_list(r);
    return ll_end->next;
}

void free_linked_list(llitem *ll_start){
	llitem *ll = ll_start;
	while (ll != NULL){
		llitem *temp = ll->next;
		free(ll);
		ll = temp;
	}
}

