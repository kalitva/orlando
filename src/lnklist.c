#include "defines.h"


bool is_empty(t_list *list)
{
	return list->last == NULL || list->first == NULL;
}

t_list* new_list()
{
	t_list *list = malloc(sizeof(t_list));
	list->first = NULL;
	list->last = NULL;
	list->head = NULL;

	return list;
}

t_node* new_node(void *value)
{
	t_node *node = malloc(sizeof(t_node));
	node->value = value;
	node->next = NULL;
	node->previous = NULL;

	return node;
}

void append_node(t_list *list, void *value)
{
	t_node *node = new_node(value);

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

void free_node(t_node *node)
{
	free(node->value);
	node->next = NULL;
	node->previous = NULL;
	node = NULL;
}

void prepend_node(t_list *list, void *value)
{
	t_node *node = malloc(sizeof(t_node));
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

void insert_node(t_list *list, void *value)
{
	if (is_empty(list))
		return;

	if (list->head == list->last) {
		append_node(list, value);
		return;
	}

	t_node *node = new_node(value);
	t_node *head = list->head;
	t_node *head_next = head->next;

	node->next = head->next;
	node->previous = head;
	head->next = node;
	head_next->previous = node;

	list->size++;
}

void remove_last(t_list *list)
{
	if (is_empty(list))
		return;

	t_node *tmp = list->last;

	list->last = list->last->previous;
	list->last->next = NULL;

	list->size--;
	free_node(tmp);
}

void remove_first(t_list *list)
{
	if (is_empty(list))
		return;

	t_node *tmp = list->first;

	list->first = list->first->next;
	list->first->previous = NULL;

	list->size--;
	free_node(tmp);
}

void remove_head(t_list *list)
{
	if (is_empty(list))
		return;

	if (list->head == list->first) {
		remove_first(list);
		return;

	} else	if (list->head == list->last) {
		remove_last(list);
		return;
	}

	t_node *head_previous = list->head->previous;
	t_node *head_next = list->head->next;
	t_node *tmp = list->head;

	head_previous->next = list->head->next;
	head_next->previous = list->head->previous;

	list->size--;
	free_node(tmp);
}

void init_head(t_list *list)
{
	if (list)
		list->head = list->last;
	else
		return;
}

void head_to_next(t_list *list)
{
	list->head = list->head->next ? list->head->next : list->head;
}

void head_to_previous(t_list *list)
{
	list->head = list->head->previous ? list->head->previous : list->head;
}