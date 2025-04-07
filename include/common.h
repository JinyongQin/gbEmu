
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBUG

#define reg8 uint8_t
#define reg16 uint16_t

#define NO_IMPL                                                 \
    {                                                           \
        printf("[file]: %s [function]: %s() not implemented\n", \
               __FILE__,                                        \
               __func__);                                       \
        exit(-5);                                               \
    }

#define BETWEEN(x, a, b) (((x) >= (a)) && ((x) <= (b)))

// 环形队列
typedef struct rQueue
{
    uint8_t *buf;
    uint16_t head;
    uint16_t tail;
    uint16_t size;
} rQueue;

bool rQueue_is_empty(rQueue *q);
bool rQueue_is_full(rQueue *q);
void rQueue_init(rQueue *q, uint16_t size);
void rQueue_push(rQueue *q, uint8_t data);
uint8_t rQueue_pop(rQueue *q);
void rQueue_clear(rQueue *q);
void rQueue_destroy(rQueue *q);
