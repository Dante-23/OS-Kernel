#pragma once
#include "../drivers/print_string.c"
#include "buffers.c"

void print_memory_map(struct Memory_Map* memory_map, unsigned short position){

    print("Memory Base: ");
    char a[10];
    int_to_ascii(memory_map->base_address, a);
    print(a);

    print("\n");

    print("Region Length: ");
    char b[10];
    int_to_ascii(memory_map->region_length, b);
    print(b);
    print("\n");

    print("Region Type: ");
    char c[10];
    int_to_ascii(memory_map->region_type, c);
    print(c);

    print("\n\n");
}

struct Memory_Map** get_type_1_memory_map(){
    if(type_1_index) return type_1_memory_map;
    for(unsigned char i = 0; i < *memory_regions; i++){
        struct Memory_Map* mp = (struct Memory_Map*)0x5000;
        mp += i;
        if(mp->region_type == 1)
            type_1_memory_map[type_1_index++] = mp;
    }
    return type_1_memory_map;
}

void allocate_block(unsigned int block){
    unsigned int bitmap_block = block / BITMAP_BLOCK_SIZE;
    unsigned int bitmap_block_bit = block % BITMAP_BLOCK_SIZE;
    if((bitmap[bitmap_block] & (1 << bitmap_block_bit)) != 0)
        return;
    bitmap[bitmap_block] += (1 << bitmap_block_bit);
}

void deallocate_block(unsigned int block){
    unsigned int bitmap_block = block / BITMAP_BLOCK_SIZE;
    unsigned int bitmap_block_bit = block % BITMAP_BLOCK_SIZE;
    if((bitmap[bitmap_block] & (1 << bitmap_block_bit)) == 0)
        return;
    bitmap[bitmap_block] -= (1 << bitmap_block_bit);
}

unsigned int get_blocks(unsigned long long bytes){
    unsigned int blocks = bytes / PAGE_SIZE;
    if(bytes % PAGE_SIZE != 0) blocks++;
    return blocks;
}

unsigned int get_number_of_free_blocks(){
    unsigned int free_blocks = 0;
    for(unsigned int bit = 0; bit < BITMAP_BLOCK_SIZE; bit++){
        unsigned int mask = (1 << bit);
        for(unsigned int i = 0; i < BITMAP_SIZE; i++){
            if((bitmap[i] & mask) == 0) free_blocks++;
        }
    }
    return free_blocks;
}

unsigned int get_number_of_alloc_blocks(){
    unsigned int free_blocks = get_number_of_free_blocks();
    return (memory_size / PAGE_SIZE) - free_blocks;
}

void initialize_main_memory_map(){
    for(unsigned char i = 0; i < *memory_regions; i++){
        struct Memory_Map* mp = (struct Memory_Map*)0x5000;
        mp += i;
        memory_size += mp->region_length;
    }
    // unsigned int memory_blocks = memory_size / PAGE_SIZE;
    // if(memory_size % PAGE_SIZE != 0) memory_blocks++;

    // Assuming first 16 blocks (64 KB) is taken by kernel
    // Hence cannot be allocated
    // This value can change if the size of kernel increasing to more than 64 KB
    for(unsigned int block = 0; block < 16; block++){
        allocate_block(block);
    }
    for(unsigned char i = 0; i < *memory_regions; i++){
        struct Memory_Map* mp = (struct Memory_Map*)0x5000;
        mp += i;
        if(mp->region_type != 1){
            // Allocate this complete region because memory regions
            // other than type 1 cannot be used by us as of now.
            unsigned int current_type_blocks = get_blocks(mp->region_length);
            unsigned int previous_blocks = get_blocks(mp->base_address);
            for(unsigned int block = previous_blocks + 1; block <= (current_type_blocks + previous_blocks); block++){
                allocate_block(block);
            }
        }
    }
}