#include "./../header/queue.h"

struct keyData* createKeyQueue()
{
    struct keyData* pKeyQueue = malloc(sizeof(struct keyData) * QUEUE_SIZE);
    return pKeyQueue;
}
/**
 * return: if queue is full, the first key will be removeKeyd and returned
 */
struct keyData* addKey(struct keyData* pKeyQueue, struct keyData* pKey)
{
    if (!&pKeyQueue[0])
    {
        pKeyQueue[0] = *pKey;
        return NULL;
    }
    for (int i = 1; i < QUEUE_SIZE; i++)
    {
        if (!&pKeyQueue[i])
        {
            pKeyQueue[i] = *pKey;
            return NULL;
        }
        
    }
    return removeKey(pKeyQueue);
}

struct keyData* removeKey(struct keyData* pKeyQueue)
{
    if (!pKeyQueue) { 
        printf("queue empty\n"); 
        return NULL; 
    }
    struct keyData* pKey = &pKeyQueue[0];
    for (int i = 1; &pKeyQueue[i] != NULL; i++)
    {
        pKeyQueue[i-1] = pKeyQueue[i];
    }
    return pKey;
}