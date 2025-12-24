#include "snake.h"

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