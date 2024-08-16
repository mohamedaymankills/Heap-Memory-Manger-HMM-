/**************** Author  : Mohamed Ayman *****************/
/**************** Name    : hmm_rand.c    *****************/
/**************** Version : 0.0.1         *****************/
/**************** Date    : 12-8-2024     *****************/



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Define the number of memory allocations to perform
#define NUM_ALLOCATIONS 100

// Define the maximum size for each allocation (64KB)
#define MAX_ALLOCATION_SIZE 1024 * 64



// Declare external functions for the custom Heap Memory Manager
extern void *HmmAlloc(size_t size);  // Function to allocate memory
extern void HmmFree(void *ptr);      // Function to free allocated memory

int main() {
    // Seed the random number generator with the current time
    srand((unsigned int)time(NULL));

    // Array to hold the pointers to allocated memory blocks
    void *allocations[NUM_ALLOCATIONS] = {NULL};  // Initialize all pointers to NULL
    
    
    

    // Perform a series of random memory allocations and deallocations
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) 
    {
        // Randomly decide whether to allocate or free memory (50% chance for each)
        int action = rand() % 2;

        if (action == 0) 
        {  // Allocate memory
            // Generate a random allocation size between 1 and MAX_ALLOCATION_SIZE
            size_t size = rand() % MAX_ALLOCATION_SIZE + 1;
            
            // Attempt to allocate the memory and store the pointer
            allocations[i] = HmmAlloc(size);
            
            // Check if the allocation was successful
            if (allocations[i] == NULL) 
            {
            
                // If allocation failed, print an error message with the requested size
                printf("Allocation failed for size: %zu\n", size);
            } 
            else 
            {
                // If allocation succeeded, print the size and address of the allocated block
                printf("Allocated %zu bytes at %p\n", size, allocations[i]);
            }
        } 
        else 
        {  // Free memory
            // Randomly select an index in the allocations array to free
            int free_index = rand() % NUM_ALLOCATIONS;
            
            // Check if the selected index points to a valid allocation
            if (allocations[free_index] != NULL) 
            {
                // Free the memory block at the selected index
                HmmFree(allocations[free_index]);
                
                // Print the address of the freed memory block
                printf("Freed memory at %p\n", allocations[free_index]);
                
                // Set the pointer to NULL to mark it as freed
                allocations[free_index] = NULL;
            }
        }
    }

    // Free all remaining allocated blocks to avoid memory leaks
    for (int i = 0; i < NUM_ALLOCATIONS; ++i) 
    {
        if (allocations[i] != NULL) 
        {
            // Free the memory block if it's still allocated
            HmmFree(allocations[i]);
            
            // Print the address of the freed memory block
            printf("Freed remaining memory at %p\n", allocations[i]);
        }
    }

    return 0;  // Return success
}

