#pragma once
#include "../drivers/print_string.c"
#include "buffers.c"

void print_memory_map(struct Memory_Map* memory_map, unsigned short position){

    print("Region Base:            ");
    char a[10];
    int_to_ascii(memory_map->base_address, a);
    print(a);

    print("\n");

    print("Region Length:          ");
    char b[10];
    int_to_ascii(memory_map->region_length, b);
    print(b);
    print("\n");

    print("Region Type:            ");
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

unsigned char is_allocated(unsigned int block){
    unsigned int bitmap_block = block / BITMAP_BLOCK_SIZE;
    unsigned int bitmap_block_bit = block % BITMAP_BLOCK_SIZE;
    if((bitmap[bitmap_block] & (1 << bitmap_block_bit)) == 0)
        return 0;
    else return 1;
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

void* allocate(unsigned short bytes){
    unsigned int blocks = get_blocks(bytes), region_index = 0;
    for(unsigned char reg = 0; reg < *memory_regions; reg++){
        struct Memory_Map* current_mp = (struct Memory_Map*)0x5000;
        current_mp += reg;
        if(current_mp->region_type != 1) continue;
        unsigned int current_type_blocks = get_blocks(current_mp->region_length);
        unsigned int previous_blocks = get_blocks(current_mp->base_address);
        for(unsigned int block = previous_blocks; block < (current_type_blocks + previous_blocks); block++){
            if(is_allocated(block)) continue;
            allocate_block(block);
            return (void*)(block * PAGE_SIZE);
        }
    }
    return (void*)0;
}

void deallocate(void* address){
    deallocate_block((unsigned int)(address) / PAGE_SIZE);
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

    // print("bit map actual size: ");
    // print(int_to_string(BITMAP_ACTUAL_SIZE));
    // print("\n");

    // print("last block bits: ");
    // print(int_to_string(last_block_bits));
    // print("\n");

    // print(int_to_string(last_block_bits));
    // print("\n");

    // Initialization is not requied because if the data in bitmap array
    // is not 0 then probably we are overwriting some existing useful data
    // for(unsigned int i = 0; i < BITMAP_ACTUAL_SIZE; i++)
    //     bitmap[i] = 0;

    unsigned int cnt = 0;
    for(unsigned int block = 0; block < 77; block++){
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
            // print("Memory regions currently being used: \n");
            // print(int_to_string(previous_blocks));
            // print(" - ");
            // print(int_to_string(current_type_blocks + previous_blocks - 1));
            // print("\n");
            for(unsigned int block = previous_blocks; block < (current_type_blocks + previous_blocks); block++){
                allocate_block(block);
                cnt++;
            }
        }
    }
    // print("Loop ran for ");
    // print(int_to_string(cnt));
    // print(" times.");
    // print("\n");
}