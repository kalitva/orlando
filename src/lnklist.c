#include "defines.h"


bool is_empty(List *list)
{
	return list->last == NULL || list->first == NULL;
}

List* new_list()
{
	return malloc(sizeof(List));
}

void append_node(List *list, void *value)
{
	Node *node = malloc(sizeof(Node));
	node->value = value;

	if (is_empty(list)) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->previous = list->last;
		list->last = node;
	}

	list->size++;
}

void prepend_node(List *list, void *value)
{
	Node *node = malloc(sizeof(Node));
	node->value = value;

	if (is_empty(list)) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->previous = node;
		list->first = node;
	}

	list->size++;
}

void remove_last(List *list)
{
	if (is_empty(list))
		return;

	Node *tmp = list->last;
	list->last = list->last->previous;
	list->last->next = NULL;
	free(tmp);

	list->size--;
}

void remove_first(List *list)
{
	if (is_empty(list))
		return;

	Node *tmp = list->first;
	list->first = list->first->next;
	list->first->previous = NULL;
	free(tmp);

	list->size--;
}

void remove_node(List *list, Node *node)
{
	if (is_empty(list))
		return;

	if (node == list->first) {
		remove_first(list);
		return;
	}

	if (node == list->last) {
		remove_last(list);
		return;
	}

	Node *node_previous = node->previous;
	Node *node_next = node->next;

	node_previous->next = node->next;
	node_next->previous = node->previous;

	free(node);
	list->size--;
}