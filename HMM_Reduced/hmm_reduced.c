/**************** Author  : Mohamed Ayman    *****************/
/**************** Name    : hmm_reduced.c    *****************/
/**************** Version : 0.0.1            *****************/
/**************** Date    : 12-8-2024        *****************/


#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#define HEAP_SIZE  1024 * 1024  // 1 MB simulated heap size

// Statically allocated array simulating the heap area
static uint8_t heap[HEAP_SIZE];

// Simulated program break, initially pointing to the beginning of the heap
static void *program_break = heap;

// Structure representing a block of memory in the heap
typedef struct BlockHeader {
    size_t size;                // Size of the allocated block (excluding the header)
    struct BlockHeader *next;   // Pointer to the next block in the free list
    int free;                   // Flag indicating whether the block is free (1) or allocated (0)
} BlockHeader;

#define HEADER_SIZE sizeof(BlockHeader)  // Size of the block header
#define ALIGN(x) (((x) + sizeof(size_t) - 1) & ~(sizeof(size_t) - 1))  // Align to the system's word size
#define MIN_BLOCK_SIZE (HEADER_SIZE + ALIGN(sizeof(size_t)))  // Minimum block size after splitting

// Pointer to the start of the free list
static BlockHeader *free_list = NULL;

// Function prototypes
void *HmmAlloc(size_t size);
void HmmFree(void *ptr);
BlockHeader *find_free_block(size_t size);
void split_block(BlockHeader *block, size_t size);
void add_to_free_list(BlockHeader *block);
void merge_free_blocks();

// Function to allocate memory of the specified size
void *HmmAlloc(size_t size) 
{
    if (size == 0) 
    {
        return NULL;  // If size is 0, return NULL as there's nothing to allocate
    }

    size = ALIGN(size);  // Align the requested size to the system's word size

    // Attempt to find a suitable free block in the free list
    BlockHeader *block = find_free_block(size);
    if (block == NULL) 
    {
        return NULL;  // If no suitable block is found, return NULL
    }

    block->free = 0;  // Mark the block as allocated
    return (void *)(block + 1);  // Return a pointer to the memory just after the block header
}

// Function to free a previously allocated block of memory
void HmmFree(void *ptr) 
{
    if (ptr == NULL) 
    {
        return;  // If the pointer is NULL, there is nothing to free
    }

    BlockHeader *block = (BlockHeader *)ptr - 1;  // Get the block header associated with the pointer
    block->free = 1;  // Mark the block as free

    // Add the block back to the free list
    add_to_free_list(block);

    // Merge consecutive free blocks to reduce fragmentation
    merge_free_blocks();
}

// Function to find a free block that can accommodate the requested size
BlockHeader *find_free_block(size_t size) 
{
    BlockHeader *current = free_list;  // Start from the beginning of the free list
    BlockHeader *prev = NULL;  // To keep track of the previous block

    while (current) 
    {
        if (current->free && current->size >= size) 
        {
            if (prev) 
            {
                prev->next = current->next;  // Remove the block from the free list
            } 
            else 
            {
                free_list = current->next;  // Update the free list head
            }
            split_block(current, size);  // Split the block if necessary
            
            return current;
        }
        prev = current;
        current = current->next;  // Move to the next block in the list
    }

    // Extend the heap by a larger chunk size to minimize future increments
    size_t chunk_size = (size + HEADER_SIZE) < (1024 * 16) ? (1024 * 16) : (size + HEADER_SIZE); // Minimum 16KB chunks

    if ((uintptr_t)program_break + chunk_size > (uintptr_t)heap + HEAP_SIZE) 
    {
        return NULL;  // If there isn't enough space left, return NULL
    }

    // Create a new block at the current program break position
    BlockHeader *new_block = (BlockHeader *)program_break;
    new_block->size = chunk_size - HEADER_SIZE;
    new_block->free = 0;
    new_block->next = NULL;



    // Move the program break forward by the size of the new block and its header
    program_break = (void *)((uintptr_t)program_break + chunk_size);

    // Optionally add remaining space to free list if there's extra room
    if (new_block->size > size + MIN_BLOCK_SIZE) 
    {
        split_block(new_block, size);
    }

    return new_block;
}

// Function to split a block into two if the requested size is smaller than the block size
void split_block(BlockHeader *block, size_t size) 
{
    if (block->size >= size + HEADER_SIZE + sizeof(size_t)) 
    {
        // Calculate the address of the new block that will be created by the split
        BlockHeader *new_block = (BlockHeader *)((uintptr_t)block + HEADER_SIZE + size);
        
        
        new_block->size = block->size - size - HEADER_SIZE;  // Adjust the size of the new block
        new_block->free = 1;  // Mark the new block as free
        new_block->next = block->next;  // Link the new block to the next block in the list



        block->size = size;  // Adjust the size of the original block
        block->next = new_block;  // Link the original block to the new block

        // Add the new block to the free list immediately
        add_to_free_list(new_block);
    }
}

// Function to add a block to the free list
void add_to_free_list(BlockHeader *block) 
{
    block->next = free_list;  // Add the block to the beginning of the free list
    free_list = block;
}

// Function to merge consecutive free blocks into a single larger block
void merge_free_blocks() 
{
    BlockHeader *current = free_list;  // Start from the beginning of the free list

    while (current && current->next) 
    {
        if (current->free && current->next->free) 
        {
            // If two consecutive blocks are both free, merge them
            current->size += current->next->size + HEADER_SIZE;  // Increase the size of the current block
            current->next = current->next->next;  // Remove the next block from the list
        } 
        else 
        {
            current = current->next;  // Move to the next block in the list
        }
    }
}


// Main function to demonstrate the Heap Memory Manager
int main() {
    // Allocate 256k bytes of memory
    void *ptr1 = HmmAlloc(256 * 1024);
    // Allocate 128k bytes of memory
    void *ptr2 = HmmAlloc(128 * 1024);
    // Free the first allocated block (256 bytes)
    HmmFree(ptr1);
    // Allocate 64k bytes of memory
    void *ptr3 = HmmAlloc(64 * 1024);
    
    // Print the addresses of the allocated blocks
    printf("ptr1: %p\n", ptr1);
    printf("ptr2: %p\n", ptr2);
    printf("ptr3: %p\n", ptr3);

    return 0;
} 

