#pragma once

#define MEM_CHK_ENABLED

#ifdef MEM_CHK_ENABLED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct mem_chk_block_t {
    const char* file;
    int line;
    size_t size;
    struct mem_chk_block_t *next, *prev;
} mem_chk_block;

static mem_chk_block* mem_blocks = NULL;

void* mem_chk_malloc(size_t sz, const char* file, int line) {
    mem_chk_block* blk = (mem_chk_block*)malloc(sz * sizeof(mem_chk_block));
    if(blk == NULL) return blk;
    blk->file = file;
    blk->line = line;
    blk->next = mem_blocks;
    if(mem_blocks) {
        blk->next->prev = mem_blocks;
    }
    blk->prev = NULL;
    blk->size = sz;
    mem_blocks = blk;
    return blk + 1;
}

void mem_chk_free(void* ptr) {
    if(ptr == NULL) return;
    mem_chk_block* blk = (mem_chk_block*)ptr - 1;
    mem_chk_block* b = mem_blocks;
    while(b != NULL) {
        if(strcmp(b->file, blk->file) == 0 && b->line == blk->line && b->size == blk->size) {
            if(blk->prev != NULL) {
                blk->prev->next = blk->next;
                if(blk->next != NULL) {
                    blk->next->prev = blk->prev;
                }
            } else {
                mem_blocks = blk->next;
            }
        }
        b = b->next;
    }
    free(blk);
}

#define malloc(sz)       mem_chk_malloc(sz, __FILE__, __LINE__)
#define free(ptr)        mem_chk_free(ptr)
#define realloc(ptr, sz) mem_chk_realloc(ptr, sz, __FILE__, __LINE__)

#endif
