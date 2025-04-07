#include "common.h"
// 判断队列是否为空
bool rQueue_is_empty(rQueue *q)
{
    return q->head == q->tail;
}
// 判断队列是否满
bool rQueue_is_full(rQueue *q)
{
    return ((q->head + 1) % q->size) == q->tail;
}

// 初始化环形队列
void rQueue_init(rQueue *q, uint16_t size)
{
    q->buf = (uint8_t *)malloc(size);
    if (q->buf == NULL)
    {
        printf("Failed to allocate memory for rQueue\n");
        exit(-1);
    }
    q->head = 0;
    q->tail = 0;
    q->size = size;
}
// 入队
void rQueue_push(rQueue *q, uint8_t data)
{
    if (rQueue_is_full(q))
    {
        printf("Queue is full, cannot push data\n");
        return;
    }
    q->buf[q->head] = data;
    q->head = (q->head + 1) % q->size;
}
// 出队
uint8_t rQueue_pop(rQueue *q)
{
    if (rQueue_is_empty(q))
    {
        printf("Queue is empty, cannot pop data\n");
        return 0; // or some error value
    }
    uint8_t data = q->buf[q->tail];
    q->tail = (q->tail + 1) % q->size;
    return data;
}
// 清空队列
void rQueue_clear(rQueue *q)
{
    q->head = 0;
    q->tail = 0;
}
// 销毁队列
void rQueue_destroy(rQueue *q)
{
    if (q->buf != NULL)
    {
        free(q->buf);
        q->buf = NULL;
    }
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    free(q);
    q = NULL;
    // printf("Queue destroyed\n");
}