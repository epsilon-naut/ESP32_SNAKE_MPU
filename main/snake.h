#ifndef SNAKE_H
#define SNAKE_H

#include "dot.h"

typedef struct node node;

struct node {
    dot pos;
    node *head;
    node *tail;
};

typedef struct snake {
    node *head;
    node *tail;
} snake;

void enq(snake *sn, dot d);

void snakestart(snake *sn, dot d);

dot deq(snake *sn);

#endif