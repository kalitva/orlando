#include <stdlib.h>
#include <stdbool.h>

#include "types.h"

bool is_empty_list(list_t *list)
{
	return list->last == NULL || list->first == NULL;
}

list_t* new_list()
{
	list_t *list = malloc(sizeof(list_t));

	list->first = NULL;
	list->last = NULL;
	list->head = NULL;
	list->size = 0;

	return list;
}

node_t* new_node(void *value)
{
	node_t *node = malloc(sizeof(node_t));
	
	node->value = value;
	node->next = NULL;
	node->previous = NULL;

	return node;
}

void free_node(node_t *node)
{
	free(node->value);
	node->value = NULL;
	free(node);
	node = NULL;
}

void append_node(list_t *list, void *value)
{
	node_t *node = new_node(value);

	if (is_empty_list(list)) {
		list->first = node;
		list->last = node;
	} else {
		list->last->next = node;
		node->previous = list->last;
		list->last = node;
	}

	list->size++;
}

void prepend_node(list_t *list, void *value)
{
	node_t *node = new_node(value);

	if (is_empty_list(list)) {
		list->first = node;
		list->last = node;
	} else {
		node->next = list->first;
		list->first->previous = node;
		list->first = node;
	}

	list->size++;
}

void insert_node(list_t *list, void *value)
{
	if (!list->head) {
		return;
	}

	if (list->head == list->last) {
		append_node(list, value);
		return;
	}

	node_t *node = new_node(value);
	node_t *head = list->head;
	node_t *head_next = head->next;

	node->next = head->next;
	node->previous = head;
	head->next = node;
	head_next->previous = node;

	list->size++;
}

void remove_last(list_t *list)
{
	if (is_empty_list(list)) {
		return;
	}

	node_t *tmp = list->last;

	list->last = list->last->previous;

	if (list->last) {
		list->last->next = NULL;
	}

	list->size--;
	free_node(tmp);
}

void remove_first(list_t *list)
{
	if (is_empty_list(list)) {
		return;
	}

	node_t *tmp = list->first;

	list->first = list->first->next;

	if (list->first) {
		list->first->previous = NULL;
	}

	list->size--;
	free_node(tmp);
}

void remove_head(list_t *list)
{
	if (is_empty_list(list) || !list->head) {
		return;
	}

	if (list->head == list->first) {
		remove_first(list);
		return;
	} else	if (list->head == list->last) {
		remove_last(list);
		return;
	}

	node_t *head_previous = list->head->previous;
	node_t *head_next = list->head->next;
	node_t *tmp = list->head;

	head_previous->next = list->head->next;
	head_next->previous = list->head->previous;

	list->size--;
	free_node(tmp);
}

void init_head(list_t *list)
{
	if (list) {
		list->head = list->last;
	}	else {
		return;
	}
}

void head_to_next(list_t *list)
{
	if (list->head) {
		list->head = list->head->next ? list->head->next : list->head;
	}
}

void head_to_previous(list_t *list)
{
	if (list->head) {
		list->head = list->head->previous ? list->head->previous : list->head;
	}
}