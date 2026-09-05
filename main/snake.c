#include "snake.h"

// Functions to start the linked list representing the snake, add new nodes or remove existing ones
// (usually used to remove the tail of the snake when it moves)

void enq(snake *sn, dot d) {
    node *nh = (node *)malloc(sizeof(node));
    nh->pos = d;
    nh->head = NULL;
    nh->tail = sn->head;
    sn->head->head = nh;
    sn->head = nh;
}

void snakestart(snake *sn, dot d) {
    enq(sn, d);
    sn->tail = sn->head;
    sn->head->tail = NULL;
}

dot deq(snake *sn) {
    node *rmv = sn->tail;
    sn->tail->head->tail = NULL;
    sn->tail = sn->tail->head;
    dot d = rmv->pos;
    free(rmv);
    return d;
}