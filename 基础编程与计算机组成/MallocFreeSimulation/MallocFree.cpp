#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 64      // 单个内存块大小
#define BLOCK_COUNT 1000   // 内存块总数

typedef struct Block {
    struct Block* next;    // 链表管理空闲块
} Block;

typedef struct {
    char* memory;          // 内存池起始地址
    Block* free_list;      // 空闲链表头
} MemoryPool;

MemoryPool pool;

// 初始化内存池
void init_pool() {
    pool.memory = (char*)malloc(BLOCK_SIZE * BLOCK_COUNT);
    pool.free_list = NULL;

    for (int i = 0; i < BLOCK_COUNT; ++i) {
        Block* block = (Block*)(pool.memory + i * BLOCK_SIZE);
        block->next = pool.free_list;
        pool.free_list = block;
    }
}

// 模拟 malloc
void* mymalloc() {
    if (pool.free_list == NULL) {
        printf("run out of menmory\n");
        return NULL;
    }
    Block* block = pool.free_list;
    pool.free_list = block->next;
    return (void*)block;
}

// 模拟 free
void myfree(void* ptr) {
    if (ptr == NULL) return;
    Block* block = (Block*)ptr;
    block->next = pool.free_list;
    pool.free_list = block;
}

// 销毁内存池
void destroy_pool() {
    free(pool.memory);
    pool.memory = NULL;
    pool.free_list = NULL;
}

int main() {
    init_pool();

    void* p = mymalloc();
    if (p == NULL) {
        printf("run out of menmory\n");
        return 1;
    }

    // 向内存写入字符串
    strcpy((char*)p, "Hello, Memory Pool");
    printf("string: %s\n", (char*)p);

    // 写入自定义结构体数据
    typedef struct {
        int id;
        float value;
    } MyData;

    MyData* data = (MyData*)p;
    data->id = 42;
    data->value = 3.14f;
    printf("structure: id = %d, value = %.2f\n", data->id, data->value);

    myfree(p);
    destroy_pool();

    return 0;
}