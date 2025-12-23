\# BuddyAllocator – Custom Memory Allocator in C++



A custom dynamic memory allocator written in C++, implementing `malloc/free/calloc/realloc`

style APIs with progressively more advanced strategies (free-list reuse → buddy allocator).



\## Highlights

\- Heap allocation with `sbrk`

\- Block metadata + free-list reuse

\- Buddy allocator: power-of-two blocks, splitting \& coalescing

\- Large allocations handled with `mmap/munmap`



\## Files

\- `malloc\_1.cpp` – naive allocator

\- `malloc\_2.cpp` – metadata + free list + realloc/calloc

\- `malloc\_3.cpp` – buddy allocator + merging/splitting + large alloc handling



