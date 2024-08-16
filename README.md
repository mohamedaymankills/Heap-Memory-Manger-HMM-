
# Heap Memory Manager

## Overview

This project implements a custom Heap Memory Manager (HMM) in C, designed to provide dynamic memory allocation services to user-space programs without relying on kernel-level functions like `sbrk()`. Instead, the HMM operates entirely in user space, simulating the heap using a statically allocated array. This approach allows for easy debugging and flexibility during development.

## Features

- **Custom Memory Allocation:** The function `void *HmmAlloc(size_t size)` is provided to allocate memory blocks of the specified size.
- **Custom Memory Deallocation:** The function `void HmmFree(void *ptr)` is provided to free the allocated memory using the pointer returned by `HmmAlloc()`.
- **Simulated Heap Management:** The heap is simulated using a large, statically allocated array, with a variable simulating the program break pointer.
- **Dynamic Heap Adjustment:** The simulated heap size can be increased or decreased by adjusting the program break variable, mimicking the behavior of `sbrk()`.

## How It Works

1. **Static Array Simulation:** A large statically allocated array is used to simulate the heap. This array represents the memory available for allocation.
2. **Program Break Simulation:** A variable is used to simulate the program break. Initially, it points to the beginning of the static array, and it is adjusted as memory is allocated or freed.
   - To increase the heap size, the program break is incremented.
   - To decrease the heap size, the program break is decremented.
3. **Memory Allocation and Deallocation:** 
   - `HmmAlloc(size_t size)` allocates a block of memory by advancing the program break.
   - `HmmFree(void *ptr)` deallocates a block by marking it as free in the simulated heap.

## Getting Started

### Prerequisites

- C Compiler (e.g., `gcc`)

### Compilation

To compile the project, run:

```bash
gcc -o heap_memory_manager heap_memory_manager.c
```

### Usage

You can use the following functions in your user-space programs:

```c
void *HmmAlloc(size_t size); // Allocates a memory block
void HmmFree(void *ptr);     // Frees a memory block
```

### Example

```c
#include "heap_memory_manager.h"

int main() {
    int *array = (int *)HmmAlloc(10 * sizeof(int)); // Allocate memory for 10 integers
    HmmFree(array); // Free the allocated memory
    return 0;
}
```

## Future Enhancements

- Add error handling for out-of-memory conditions.
- Optimize memory management strategies for fragmentation and coalescing free blocks.
- Implement a more sophisticated memory allocation algorithm (e.g., best fit, first fit).

## HMM Fixed Flowchart

![1](https://github.com/mohamedaymankills/Heap-Memory-Manger-HMM-/blob/main/Readme_Screenshots/Fixed.png?raw=true)

```sql
+-------------------+
|       Start       |
+-------------------+
         |
         v
+-------------------+
|   Define HEAP     |
+-------------------+
         |
         v
+-------------------+
|  Initialize heap  |
+-------------------+
         |
         v
+-------------------+
| Initialize program|
|      break        |
+-------------------+
         |
         v
+-------------------+
|  Define Struct    |
+-------------------+
         |
         v
+-------------------+
| Initialize free_list |
+-------------------+
         |
         v
+-------------------+
| Define HmmAlloc   |
+-------------------+
         |
         v
+---------------------------+
|  size == 0 ? Return NULL  |
+---------------------------+
         |
         v
+-----------------------------+
| Align size & Find free block|
+-----------------------------+
         |
         +---------+---------+
         |                   |
         v                   v
+-------------------+  +--------------------+
| Block Found?      |  | No Block Found?    |
| Set as Allocated  |  | Return NULL        |
+-------------------+  +--------------------+
         |
         v
+-------------------+
| Return Pointer    |
+-------------------+
```


## HMM Random Flowchart 

![2](https://github.com/mohamedaymankills/Heap-Memory-Manger-HMM-/blob/main/Readme_Screenshots/Random.png)

``` sql
+-------------------+
|       Start       |
+-------------------+
         |
         v
+-------------------+
| Define Constants  |
+-------------------+
         |
         v
+-------------------------+
| Declare External Functions |
+-------------------------+
         |
         v
+-------------------+
| Seed RNG          |
+-------------------+
         |
         v
+-------------------+
| Initialize Array  |
+-------------------+
         |
         v
+--------------------------+
| Perform NUM_ALLOCATIONS  |
+--------------------------+
         |
         v
+------------------------------+
| Random Action (Allocate/Free)|
+------------------------------+
         |
   +-----------+---------------+
   |                           |
   v                           v
+-------------------+   +-------------------+
|   Allocate Memory |   |   Free Memory     |
|                   |   |                   |
| Generate Size     |   | Select Index      |
| Call HmmAlloc     |   | If valid, free    |
| Store/Print Info  |   | Print/Set NULL    |
+-------------------+   +-------------------+
         |
         v
+--------------------------+
| Free Remaining Allocations |
+--------------------------+
         |
         v
+-------------------+
|        End        |
+-------------------+
```

## HMM Reduced Flowchart
![3](https://github.com/mohamedaymankills/Heap-Memory-Manger-HMM-/blob/main/Readme_Screenshots/Reduced.png)

``` sql
+------------------------+
|         Start          |
+------------------------+
         |
         v
+------------------------+
| Initialize Constants   |
+------------------------+
         |
         v
+------------------------+
| Initialize Variables   |
+------------------------+
         |
         v
+------------------------+
|      HmmAlloc()        |
| 1. Size check          |
| 2. Align size          |
| 3. Find free block     |
| 4. Allocate & return   |
+------------------------+
         |
         v
+------------------------+
|      HmmFree()         |
| 1. Null check          |
| 2. Mark block as free  |
| 3. Add to free list    |
| 4. Merge free blocks   |
+------------------------+
         |
         v
+------------------------+
|   find_free_block()    |
| 1. Traverse free list  |
| 2. Return block        |
| 3. Extend heap         |
| 4. Split if necessary  |
+------------------------+
         |
         v
+------------------------+
|     split_block()      |
| 1. Split large block   |
| 2. Add to free list    |
+------------------------+
         |
         v
+------------------------+
|  add_to_free_list()    |
+------------------------+
         |
         v
+------------------------+
| merge_free_blocks()    |
+------------------------+
         |
         v
+------------------------+
|        main()          |
| 1. Allocate 256 KB     |
| 2. Allocate 128 KB     |
| 3. Free 256 KB         |
| 4. Allocate 64 KB      |
| 5. Print addresses     |
+------------------------+
         |
         v
+------------------------+
|          End           |
+------------------------+
```


## Author

Mohamed Ayman
