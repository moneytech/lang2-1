#ifndef LIST_H
#define LIST_H 1
typedef struct list_node {
	const void *value;
	struct list_node *next;
} ListNode;
typedef struct list {
	int len;
	ListNode *head;
	ListNode *tail;
} List;
List *new_list(void);
void list_append(List *list, const void *value);
#endif
