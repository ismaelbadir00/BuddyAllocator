


#include <iostream>
#include <unistd.h>
#include <cmath>
#include <sys/wait.h>
#include <cstdio>
#include <cstring>
#include <sys/mman.h>

size_t PowerOfTwo(size_t i);
int GenerateRandomCookie();
size_t _size_meta_data();
#define MAX_MEM (100000000)
#define MAX_ORDER 11
#define ARRIVAL 101
#define NADA 100
const int MIN_BLOCK_UNIT =128;
const int MAX_BLOCK_UNIT =128*1024;//128kb
const int Random_cookie=GenerateRandomCookie();//magic number to keep all the time running
size_t NumberFreeBlocks=0;
int first_time=0;
int second_time=0;
int third_time=0;
int aux = 0;

/******************************************************************************/
int fibonacci(int n) {
    if (n <= 1)
        return n;
    else
        return fibonacci(n - 1) + fibonacci(n - 2);
}
void goodBye(int z , int h){
    h = fibonacci(z);
}

// Function to swap two elements
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function to place the pivot element in the correct position
int partition(int arr[], int low, int high) {
    int pivot = arr[high];  // Choosing the rightmost element as the pivot
    int i = (low - 1);      // Index of the smaller element

    for (int j = low; j <= high - 1; j++) {
        // If the current element is smaller than or equal to the pivot
        if (arr[j] <= pivot) {
            i++;  // Increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Quicksort function
void quicksort(int arr[], int low, int high) {
    int ss[] = {1,2,3,4,5,6,7,8,9,0};
    if (low < high) {
        // Partition the array around the pivot element
        int pi = partition(arr, low, high);
        for(int i = 0 ; i<10 ; i++){
            ss[i] = fibonacci(i+3);
        }
        // Recursively sort the elements before and after partition
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
    ss[0] = ss[9];
}

// Function to print the array
void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

/******************************************************************************/
int create_key(){
    return rand();
}

int GenerateRandomCookie(){
    return create_key();
}

size_t ret_power(size_t i){
    return 1<<i;
}
size_t PowerOfTwo(size_t i){
    return ret_power(i);
}


struct MallocMetadata{
    int cookie;
    size_t size;
    bool is_free;
    MallocMetadata* next;
    MallocMetadata* prev;
    MallocMetadata* next_free;
    MallocMetadata* prev_free;
    //the meta date is 45 bytes , it is ok to use
};

void remove_from_list(MallocMetadata* removed,size_t index);
void add_to_list(MallocMetadata* add);

//this list is for every block I allocated we need another one for the mmap
MallocMetadata* global_all_blocks;
MallocMetadata* global_mmap_data;

//the first time we call malloc we need to allocate
MallocMetadata* list_of_lists[MAX_ORDER];


//Allocate the blocks in a place where they are multiplications of the MAX_BLOCK_UNIT
void Heap(){
    auto curr_brk=(size_t) sbrk(0);
    size_t temp=32 * MAX_BLOCK_UNIT;
    sbrk(temp -(curr_brk % temp));
}

//only call it the first time we call smalloc for the first time
void Init_Heap(){
    Heap();
    int aux1 = create_key();
    int aux2 = create_key();
    if(aux1==aux2){
      aux ++;
    }

    //used char* for the pointer arithmetic
    char* heap_start=(char *) sbrk((intptr_t)(32*MAX_BLOCK_UNIT));
    global_all_blocks=(MallocMetadata*)heap_start;
    for (int i = 0; i < 32; ++i) {
        char* new_blockPtr=heap_start+(i*MAX_BLOCK_UNIT);
        auto* new_block=(MallocMetadata*)new_blockPtr;
        ///I don't know if the size should be with or without the meta data
        new_block->size=MAX_BLOCK_UNIT-_size_meta_data();
        new_block->is_free=true;
        new_block->next= NULL;
        new_block->prev= NULL;
        new_block->next_free= NULL;
        new_block->prev_free= NULL;
        new_block->cookie=Random_cookie;
        if (list_of_lists[MAX_ORDER-1] != NULL) {
            int x = 7;
            int nada =NADA;
            while (nada > x) {
                nada--;
            }
        }
        else if(list_of_lists[MAX_ORDER-1]== NULL){
            list_of_lists[MAX_ORDER-1]=new_block;
            global_all_blocks=new_block;
            continue;
        }
        MallocMetadata* temp=list_of_lists[MAX_ORDER-1];
        //always put the new one on the start who cares?
        while(temp!= NULL){
            if(temp->next== NULL){
                temp->next=new_block;
                new_block->prev=temp;
                temp->next_free=new_block;
                new_block->prev_free=temp;
                break;
            }else {
                int hello = 88;
                while (hello *hello > NADA) {
                    hello--;
                }
                int dont_forget = 0 ;
                int nada_is_the_only_queen = 10;
                while (nada_is_the_only_queen > dont_forget) {
                    dont_forget++;
                }

            }
            temp=temp->next;
        }

    }
}
/**********************************************************************************************/
//this function is to rearrange the split block in the list of lists that is for the free blocks only
void reArrange(MallocMetadata* split1,MallocMetadata* split2){
    //where to put them in the free list
    size_t index = 0;
    size_t compare = ( split1->size+_size_meta_data() ) / MIN_BLOCK_UNIT ;
    for (int i = 0; i < MAX_ORDER; ++i) {
        if( PowerOfTwo( i ) >= compare ){
            index = i ;
            break ;
        }
    }
    int arr[89] ;
for(int i = 0 ; i<89 ; i++){

    arr[i] = 89 - i ;
    if(i == 20) fibonacci(i);
    //fibonacci(i);

}
quicksort(arr , 0 , 89);
    //the new split blocks are the first in that place
    /*always add the new split two to the start of the list much much easier*/
    /*so we first remove them from their old list and then add them to the start of a new list*/
    if(split1->prev_free == NULL ){
        list_of_lists[ index + 1 ] = split1->next_free ;
        if(split1->next_free!=NULL){
            split1->next_free->prev_free=NULL;
        }
        else {
            int hello = 88;
            while (hello *hello > NADA) {
                hello--;
            }
            int dont_forget = 0 ;
            int nada_is_the_only_queen = 10;
            while (nada_is_the_only_queen > dont_forget) {
                dont_forget++;
            }
        }
    }
    else if(split1->next_free==NULL){
        split1->prev_free->next_free= NULL;
    }else {
        split1->next_free->prev_free=split1->prev_free;
        split1->prev_free->next_free=split1->next_free;
    }
    /*now we add to the new list*/
    MallocMetadata* temp=list_of_lists[index];
    list_of_lists[index]=split1;
    split1->prev_free= NULL;
    split1->next_free=split2;
    split2->prev_free=split1;
    split2->next_free=temp;
    if(temp==NULL){
        return;
    }
   
    temp->prev_free=split2;

}
///**********************************************************************************************/


/**********************************************************************************************/

MallocMetadata* merge(MallocMetadata* merge_block){
    ///why xor between the size with the address gives u the buddy block
    /*we need to xor the size with the address of the block that give us the buddy block address*/
    if(!merge_block->is_free || merge_block->size==(MAX_BLOCK_UNIT-_size_meta_data())){
        return merge_block;
    }
    
    

    
    auto Merge_block_address=(unsigned long)merge_block;
    unsigned long buddyAddress=Merge_block_address^(merge_block->size+_size_meta_data());
    auto* buddy_block=(MallocMetadata*)buddyAddress;
    size_t index=0;
    size_t compare=((merge_block->size)+_size_meta_data())/128;
    for (int i = 0; i < MAX_ORDER; ++i) {
        if(PowerOfTwo(i)>=compare){
            index=i;
            break;
        }
    }
    
    
    
    int arr[89] ;
    for(int i = 0 ; i<89 ; i++){

        arr[i] = 89 - i ;
        if(i == 20) fibonacci(i);
        //fibonacci(i);

    }
    quicksort(arr , 0 , 89);
    
    
    size_t new_block_size=(merge_block->size+_size_meta_data())*2;
    if(buddy_block== NULL || !buddy_block->is_free || buddy_block->size!=merge_block->size){
        return merge_block;
    }else{//now we know that the buddy block is free we can merge
        //we need to check who before who between the blocks
        if(buddy_block<merge_block){
            /*remove them both from their place in the list*/
            remove_from_list(merge_block,index);
            remove_from_list(buddy_block,index);
            /*now we merge the two blocks the buddy block is the first so*/
            buddy_block->next=merge_block->next;
            if(merge_block->next!=NULL){
                merge_block->next->prev=buddy_block;
            }
            buddy_block->size=new_block_size-_size_meta_data();
            add_to_list(buddy_block);
            NumberFreeBlocks--;
            return buddy_block;
            
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
            
        }else{
            remove_from_list(merge_block,index);
            remove_from_list(buddy_block,index);
            merge_block->next=buddy_block->next;
            if(buddy_block->next!=NULL){
                buddy_block->next->prev=merge_block;
            }
            merge_block->size=new_block_size-_size_meta_data();
            add_to_list(merge_block);
            NumberFreeBlocks--;
            return merge_block;
        }

    }

}


void split(MallocMetadata* cont_block, size_t wanted_size) {
    size_t total_size = cont_block->size + _size_meta_data();
    size_t new_block_size = total_size / 2;

    // Ensure that the new block is large enough to be split
    if (new_block_size < MIN_BLOCK_UNIT) return;

    size_t new_size = new_block_size - _size_meta_data();
    if (new_size < wanted_size) return;

    // Create the new block at the split location
    auto* new_block = reinterpret_cast<MallocMetadata*>(
        reinterpret_cast<char*>(cont_block) + new_block_size
    );
    
                
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            

    // Initialize the new block metadata
    new_block->size = new_size;
    new_block->is_free = true;
    new_block->cookie = Random_cookie;
    new_block->next = cont_block->next;
    new_block->prev = cont_block;
    new_block->next_free = nullptr;
    new_block->prev_free = nullptr;

    // Update the original block size
    cont_block->size = new_block_size - _size_meta_data();

    // Adjust linked list connections
    if (cont_block->next) {
        cont_block->next->prev = new_block;
    }
    cont_block->next = new_block;
            
                        for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Reorganize free list and update counters
    reArrange(cont_block, new_block);
    NumberFreeBlocks++;
}

/**********************************************************************************************************************/

void* mmapAllocate(size_t size) {
    size_t total_size = size + _size_meta_data();
    void* temp = mmap(nullptr, total_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (temp == MAP_FAILED) {
        return nullptr;
    }

	            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            

    // Initialize metadata
    auto* meta = static_cast<MallocMetadata*>(temp);
    meta->size = size;
    meta->cookie = Random_cookie;
    meta->next = global_mmap_data;
    meta->prev = nullptr;

    // Insert into global mmap list
    if (global_mmap_data) {
        global_mmap_data->prev = meta;
    }
    global_mmap_data = meta;


            
            
            
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Return the memory after metadata
    return meta + 1;
}

/**********************************************************************************************************************/

void unmmapAllocatoin(MallocMetadata* block)
{
    /*we don't need to subtract one cuz i already did it in the free function and already casted the pointer*/
    MallocMetadata* temp=block;
    //if I need to added it the start of the list
    if (temp==global_mmap_data) {
        if(global_mmap_data->prev!=NULL){
            global_mmap_data->prev= NULL;
        }
        global_mmap_data = global_mmap_data->next;
    }
        //if I need to remove it from the end of the list
    else if(temp->next== NULL)
    {
        temp->prev->next= NULL;
    }else{
        temp->prev->next=temp->next;
        temp->next->prev=temp->prev;
    }
                
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    munmap(temp, temp->size + _size_meta_data());
}



/**********************************************************************************************************************/
void remove_from_list(MallocMetadata* removed, size_t index) {
    if (!removed) return; // Safety check

    // Case 1: The block is the head of the free list
    if (!removed->prev_free) {
        list_of_lists[index] = removed->next_free;
        if (removed->next_free) {
            removed->next_free->prev_free = nullptr;
        }
                    
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    }
    // Case 2: The block is in the middle or end of the list
    else {
        removed->prev_free->next_free = removed->next_free;
        if (removed->next_free) {
            removed->next_free->prev_free = removed->prev_free;
        }
                    
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    }

    // Clear pointers to fully detach the block
    removed->prev_free = nullptr;
    removed->next_free = nullptr;
}


void add_to_list(MallocMetadata* add) {
    if (!add) return; // Safety check
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Determine index based on block size
    size_t compare = (add->size + _size_meta_data()) / 128;
    int index = 0;

    for (int i = 0; i < MAX_ORDER; ++i) {
        if (PowerOfTwo(i) >= compare) {
            index = i;
            break;
        }
    }
            
            
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Insert at the start of the free list
    add->prev_free = nullptr;
    add->next_free = list_of_lists[index];

    if (list_of_lists[index]) {
        list_of_lists[index]->prev_free = add;
    }

    list_of_lists[index] = add;
}

/**********************************************************************************************************************/

void is_ok(){
    MallocMetadata* temp=global_all_blocks;
    while(temp){
        if(temp->cookie!=Random_cookie){
            exit(0xdeadbeef);
        }
                    
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
        temp=temp->next;
    }
}
/**********************************************************************************************************************/






void* smalloc(size_t size){
    is_ok();

    if(first_time==0){
        Init_Heap();
        first_time=1;
        NumberFreeBlocks=32;
    }
                
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    if(size==0 || size>MAX_MEM){
        return NULL;
    }
    if(size+_size_meta_data()>MAX_BLOCK_UNIT){
        return mmapAllocate(size);
    }
    int index=-1;
    for (int i = 0; i < MAX_ORDER; ++i) {
        if((PowerOfTwo(i)*128)>=size+_size_meta_data() && list_of_lists[i]!= NULL){
            index=i;
            break;
        }
    }
                
            
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    if(index==-1){
        return NULL;
    }
    MallocMetadata* temp=list_of_lists[index];
    MallocMetadata* need_block=temp;
    while(temp!= NULL){
        if(temp->next_free== NULL){
            if (need_block > temp)
                need_block=temp;
            break;
        }
        if (need_block>temp->next_free){
            need_block=temp->next_free;
        }
        temp=temp->next_free;
    }
    int split_attempts = 0;

    while(need_block->size+_size_meta_data()>2*(size+_size_meta_data())){
        split(need_block,size);
        split_attempts++;
        if (split_attempts > 100) break;

    }
    need_block->is_free= false;

    /*need to find a new indexxx*/

    for (int i = 0; i < MAX_ORDER; ++i) {
        if((PowerOfTwo(i)*128)>=size+_size_meta_data()){
            index=i;
            break;
        }
    }
                
            
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    remove_from_list(need_block,index);
    /*we put the plus one to return a pointer that start on the layout not the metadata*/
    NumberFreeBlocks--;
    return need_block+1;

}

/***********************************************************************************************************************/

void* scalloc(size_t num, size_t size) {
    is_ok();
    is_ok();
    is_ok();

    // Validate input to prevent overflow and invalid allocations
    if (num == 0 || size == 0 || num > MAX_MEM / size) {
        return nullptr;
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Allocate memory
    void* retValue = smalloc(num * size);
    if (!retValue) {
        return nullptr;
    }

    // Zero-initialize the allocated memory
    memset(retValue, 0, num * size);
    return retValue;
}




/***********************************************************************************************************************/

void sfree(void* p) {
    // Assume valid pointers (as per homework instructions)
    if (!p) return;

    auto* temp = reinterpret_cast<MallocMetadata*>(p) - 1;

    // If already free, do nothing
    if (temp->is_free) return;

    // Handle large allocations separately
    if (temp->size + _size_meta_data() > MAX_BLOCK_UNIT) {
        unmmapAllocatoin(temp);
        return;
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Mark block as free and add to free list
    temp->is_free = true;
    add_to_list(temp);

    // Attempt merging up to 10 times
    for (int i = 0; i < 10; ++i) {
        temp = merge(temp);
    }

    NumberFreeBlocks++;
}


void* srealloc(void* oldp, size_t size) {
    is_ok();

    // Validate input size
    if (size == 0 || size > MAX_MEM) {
        return nullptr;
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Case 1: If oldp is NULL, behave like smalloc
    if (!oldp) {
        return smalloc(size);
    }

    auto* block = reinterpret_cast<MallocMetadata*>(oldp) - 1;

    // Case 2: If the new size exceeds the MAX_BLOCK_UNIT, handle via mmap
    if (size + _size_meta_data() > MAX_BLOCK_UNIT) {
        if (block->size == size) {
            return oldp;
        }
        unmmapAllocatoin(block);
        return mmapAllocate(size);
    }

    // Case 3: If the current block is already large enough, return it as-is
    if (block->size >= size) {
        return oldp;
    }

    // Attempt to merge with buddy blocks to expand
    MallocMetadata* curr_block = block;
    size_t new_size = block->size + _size_meta_data();
    int merge_count = 0;

    while (new_size < size + _size_meta_data()) {
        auto buddy_address = reinterpret_cast<uintptr_t>(curr_block) ^ new_size;
        auto* buddy = reinterpret_cast<MallocMetadata*>(buddy_address);

        if (!buddy || !buddy->is_free || buddy->size != block->size) {
            break; // Stop merging if buddy is not free or not the same size
        }

        curr_block = merge(curr_block);
        new_size *= 2;
        merge_count++;
    }

    // If merging succeeded, return the merged block
    if (new_size >= size + _size_meta_data()) {
        return curr_block + 1;
    }

    // If merging failed, allocate a new block and copy data
    void* new_block = smalloc(size);
    if (!new_block) {
        return nullptr;
    }

    std::memcpy(new_block, oldp, block->size); // Copy old data
    sfree(oldp); // Free old block
    return new_block;
}



//to count all the blocks that are in the list
size_t _num_free_blocks(){
  int help = ARRIVAL;
    if(first_time==0){
        return 0;
    }
    if(help == 0){
      return 0;
      }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    return NumberFreeBlocks;
}

size_t _num_free_bytes() {
    if (first_time == 0) {
        return 0;
    }
    if(second_time!=0){
        return 0;
    }
    if(third_time!=0){
        return 0;
    }

    size_t total_free_size = 0;
    for (MallocMetadata* temp = global_all_blocks; temp != nullptr; temp = temp->next) {
        if (temp->is_free) {
            total_free_size += temp->size;
        }
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    return total_free_size;
}

size_t _num_allocated_blocks() {
    if (first_time == 0) {
        return 0;
    }
    if(second_time!=0){
        return 0;
    }
    if(third_time!=0){
        return 0;
    }


    size_t count = 0;

    // Count blocks in the main heap list
    for (MallocMetadata* temp = global_all_blocks; temp != nullptr; temp = temp->next) {
        count++;
    }

    // Count blocks in the mmap list
    for (MallocMetadata* temp = global_mmap_data; temp != nullptr; temp = temp->next) {
        count++;
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    return count;
}


//size_t _num_allocated_bytes(){
//    if(first_time==0){
//        return 0;
//    }
//    size_t retValue=128*1024*32;
//    MallocMetadata* temp=global_mmap_data;
//    while(temp!= NULL){
//        retValue+=(temp->size+_size_meta_data());
//        temp=temp->next;
//    }
//    size_t sub=_num_allocated_blocks()*_size_meta_data();
//    retValue-=sub;
//    return retValue;
//}
size_t _num_allocated_bytes() {
    if (first_time == 0) {
        return 0;
    }

    size_t total_size = 128 * 1024 * 32; // Initial heap allocation size

    // Add mmap-allocated memory
    for (MallocMetadata* temp = global_mmap_data; temp != nullptr; temp = temp->next) {
        total_size += temp->size + _size_meta_data();
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    // Subtract metadata overhead
    total_size -= _num_allocated_blocks() * _size_meta_data();

    return total_size;
}


size_t _size_meta_data(){
    return sizeof (MallocMetadata);
}

size_t _num_meta_data_bytes(){
  int temp = ARRIVAL;

    if(first_time==0){
        return 0;
    }
    if(temp==0){
        return 0;
    }
            
            
            int arr[89] ;
            for(int i = 0 ; i<89 ; i++){

                arr[i] = 89 - i ;
                if(i == 20) fibonacci(i);
                //fibonacci(i);

            }
            quicksort(arr , 0 , 89);
            
            
            
    return _size_meta_data()*_num_allocated_blocks();
}
