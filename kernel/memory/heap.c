#include "heap.h"

#include <stdint.h>

#define HEAP_SIZE (1024 * 1024)

static uint8_t kernel_heap[HEAP_SIZE];
static size_t heap_offset = 0;

typedef struct
{
    size_t size;
    int free;
} heap_block_t;

void heap_init(void)
{
    heap_offset = 0;
}

void *kmalloc(size_t size)
{
    if (size == 0)
    {
        return 0;
    }

    size = (size + 7) & ~7;

    heap_block_t *block = (heap_block_t *)kernel_heap;

    while ((uint8_t *)block < kernel_heap + heap_offset)
    {
        if (block->free && block->size >= size)
        {
            block->free = 0;
            return (void *)(block + 1);
        }

        block = (heap_block_t *)((uint8_t *)(block + 1) + block->size);
    }

    if (heap_offset + sizeof(heap_block_t) + size > HEAP_SIZE)
    {
        return 0;
    }

    heap_block_t *new_block = (heap_block_t *)&kernel_heap[heap_offset];

    new_block->size = size;
    new_block->free = 0;

    heap_offset += sizeof(heap_block_t) + size;

    return (void *)(new_block + 1);
}

void kfree(void *ptr)
{
    if (ptr == 0)
    {
        return;
    }

    heap_block_t *block = ((heap_block_t *)ptr) - 1;

    block->free = 1;
}
