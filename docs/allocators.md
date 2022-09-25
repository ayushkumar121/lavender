# Allocators

## Bump allocator
In this type of allocator we linearly allocate the memory using a 
pointer next which keeps tracks of pointer to unused memory 

## Linked list allocator

## Fixed size block allocator

In this type of allocator we give out or allocate fixed size blocks,
It does waste memory due to internal fragmantation but it drastically reduces 
the time it takes to look for free blocks and reusing of memory.