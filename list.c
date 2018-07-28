#include "list.h"
#include <stdlib.h>

List *
new_list(void)
{
  List *list = malloc(sizeof *list);
  list->head = NULL;
  list->tail = NULL;
  return list;
}

void
list_append(List *list, const void *value)
{
  ListNode *node = malloc(sizeof *node);
  node->value = value;
  node->next = NULL;
  if (list->tail != NULL) {
    list->tail->next = node;
    list->tail = node;
  } else {
    list->head = node;
    list->tail = node;
  }
}

void
list_size(List *list)
{
  return (list->tail - list->head) / sizeof(ListNode);
}
