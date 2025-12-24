#include "move.h"

int min(int a, int b) {
    if (a > b) {
        return b;
    }
    else {
        return a;
    }
}

int have_fun(node *n, dot d) {
    if((n->pos.x == d.x)&&(n->pos.y == d.y)) {
        return 1;
    }
    return 2;
}

int check_collision(snake *sn, dot d) {
    int c = 2;
    int c1 = 2;
    node *n;
    if(sn->head->tail != NULL) {
        n = sn->head->tail;
    }
    else {
        n = sn->tail;
    }
    node *n2 = sn->tail;
    while(c+c1==4) {
        c = have_fun(n, d);
        c1 = have_fun(n2, d);
        if((c+c1 == 4) && ((n == n2)||(n->tail==n2))) {
            c = 0;
        }
        n = n->tail;
        n2 = n2->head;
    }
    return min(c, c1);
}

void start_apples(dot *app, snake *sn) {
    uint16_t data[2];
    uint8_t data1;
    uint8_t data2;
    uint16_t seed = read_seed(2, data, &data1, &data2);
    srand(seed);

    dot d;
    d.x = (rand() % 32)*4;
    d.y = (rand() % 32)*4;
    int i = check_collision(sn, d);
    while(i) {
        d.x = (rand() % 32)*4;
        d.y = (rand() % 32)*4;
        i = check_collision(sn, d);
    }
    *app = d;
}

void regen_apples(dot *app, snake *sn) {
    dot d;
    d.x = (rand() % 32)*4;
    d.y = (rand() % 32)*4;
    int i = check_collision(sn, d);
    while(i) {
        d.x = (rand() % 32)*4;
        d.y = (rand() % 32)*4;
        i = check_collision(sn, d);
    }
    *app = d;
}

int check_sn_collision(snake *sn) {
    if((sn->head->pos.x > 124)||(sn->head->pos.x < 0)||(sn->head->pos.y > 124)||(sn->head->pos.y < 0)) {
        return 1; //panic
    }
    else{
        // cursed recursion
        return check_collision(sn, sn->head->pos);
    }
}

int add_snake(wrapper *w, snake *sn, double speed, uint8_t xy) {
    //probably smarter to do it pixel-line-wise?
    dot *d = (dot *)malloc(sizeof(dot));
    d->x = sn->head->pos.x;
    d->y = sn->head->pos.y;
    if(xy == 0) {
        if((speed < 0)) {
            if((sn->head->tail == NULL)||(sn->head->tail->pos.y<=d->y)) {
                d->y += 4;
            }
            else {
                d->y -= 4;
            }
        }
        else if(speed >= 0){
            if((sn->head->tail == NULL)||(sn->head->tail->pos.y>=d->y)) {
                d->y -= 4;
            }
            else {
                d->y += 4;
            }
            }
        
    }
    else {
        if(speed < 0) {
            if((sn->head->tail == NULL)||(sn->head->tail->pos.x<=d->x)) {
                d->x += 4;
            }
            else {
                d->x -= 4;
            }

        }
        else if(speed >= 0){
            if((sn->head->tail == NULL)||(sn->head->tail->pos.x>=d->x)) {
                d->x -= 4;
            }
            else {
                d->x += 4;
            }

        }
        
    }
    enq(sn, *d);
    draw_dot(w, d, 1);
    free(d);
    if(check_sn_collision(sn)) {
        return 1;
    }
    return 0;
}

int move_snake(wrapper *w, snake *sn, double speed, uint8_t xy, dot *app, int *score) {
    //probably smarter to do it pixel-line-wise?
    int i = add_snake(w, sn, speed, xy);
    if(!i && ((sn->head->pos.x == app->x)&&(sn->head->pos.y == app->y))) {
        regen_apples(app, sn);
        draw_dot(w, app, 2);
        (*score)++;
    }
    else if(!i) {
        dot d2 = deq(sn); 
        if((d2.x != sn->tail->pos.x) || (d2.y != sn->tail->pos.y)) {
            draw_dot(w, &d2, 0);
        }
    }
    return i;
}

