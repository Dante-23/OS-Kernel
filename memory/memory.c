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

unsigned int get_blocks(unsigned long long bytes){
    unsigned int blocks = bytes / PAGE_SIZE;
    if(bytes % PAGE_SIZE != 0) blocks++;
    return blocks;
}

void allocate_block(unsigned int block){
    unsigned int bitmap_block = block / BITMAP_BLOCK_SIZE;
    unsigned int bitmap_block_bit = block % BITMAP_BLOCK_SIZE;
    if((bitmap[bitmap_block] & (1 << bitmap_block_bit)) != 0)
        return;
    bitmap[bitmap_block] |= (1 << bitmap_block_bit);
}

void deallocate_block(unsigned int block){
    unsigned int bitmap_block = block / BITMAP_BLOCK_SIZE;
    unsigned int bitmap_block_bit = block % BITMAP_BLOCK_SIZE;
    if((bitmap[bitmap_block] & (1 << bitmap_block_bit)) == 0)
        return;
    bitmap[bitmap_block] &= ~(1 << bitmap_block_bit);
}

unsigned int get_number_of_free_blocks(){
    unsigned int free_blocks = 0;
    for(unsigned int bit = 0; bit < BITMAP_BLOCK_SIZE; bit++){
        unsigned int mask = (1 << bit);
        for(unsigned int i = 0; i < BITMAP_ACTUAL_SIZE; i++){
            if(i == BITMAP_ACTUAL_SIZE - 1 && bit >= last_block_bits)
                break;
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
        actual_memory_size += mp->region_length;
        if(mp->base_address + mp->region_length > memory_size)
            memory_size = mp->base_address + mp->region_length;
    }
    unsigned int total_blocks = get_blocks(memory_size);
    BITMAP_ACTUAL_SIZE = total_blocks / BITMAP_BLOCK_SIZE;
    if((total_blocks % BITMAP_BLOCK_SIZE) != 0){
        BITMAP_ACTUAL_SIZE++;
        last_block_bits = total_blocks % BITMAP_BLOCK_SIZE;
    }
    if(last_block_bits == 0) last_block_bits = BITMAP_BLOCK_SIZE;

    print("Bit map actual size: ");
    print(int_to_string(BITMAP_ACTUAL_SIZE));
    print("\n");

    // print(int_to_string(last_block_bits));
    // print("\n");

    // Initialization is not requied because if the data in bitmap array
    // is not 0 then probably we are overwriting some existing useful data
    // for(unsigned int i = 0; i < BITMAP_ACTUAL_SIZE; i++)
    //     bitmap[i] = 0;

    unsigned int cnt = 0;
    for(unsigned int block = 0; block < 17; block++){
        allocate_block(block);
        cnt++;
    }
    for(unsigned char i = 0; i < *memory_regions; i++){
        struct Memory_Map* mp = (struct Memory_Map*)0x5000;
        mp += i;
        if(mp->region_type != 1){
            // Allocate this complete region because memory regions
            // other than type 1 cannot be used by us as of now.
            unsigned int current_type_blocks = get_blocks(mp->region_length);
            unsigned int previous_blocks = get_blocks(mp->base_address);
            print(int_to_string(previous_blocks));
            print("-");
            print(int_to_string(current_type_blocks + previous_blocks - 1));
            print("\n");
            for(unsigned int block = previous_blocks; block < (current_type_blocks + previous_blocks); block++){
                allocate_block(block);
                cnt++;
            }
        }
    }
    print(int_to_string(cnt));
    print("\n");
}