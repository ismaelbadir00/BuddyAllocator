//
// Created by garam on 8/17/2024.
//

#include <unistd.h>
#include <cstddef>
#include <cstring>
#define HUNDRED_MILL 100000000
//10e7 is double

typedef struct MallocMetadata {
    size_t size;
    bool is_free;
    MallocMetadata* next;
    MallocMetadata* prev;
} MetaD;

class BList{
public:
    MetaD* head;
    size_t AllocatedBlocks;
    size_t FreeBlocks;
    size_t FreeBytes;
    size_t AllocatedBytes;

    BList(): head(NULL),AllocatedBlocks(0),FreeBlocks(0),FreeBytes(0),AllocatedBytes(0){}
    ~BList()=default;
    MetaD* Bfind(size_t size);//finds a block with size "size" if not found returns null
    MetaD *Ballocte(size_t size);//changes sbrk
    void* Bfree(size_t size);
    void Binsert(MetaD* m);//add a block to the list
   // friend void* smalloc(size_t size);


};

MetaD *BList::Bfind(size_t size) {
    MetaD* found=head;
    while(found){
        if(found->is_free && found->size>=size){
            found->is_free= false;
            FreeBlocks--;
            AllocatedBlocks++;
            FreeBytes-=found->size;
            AllocatedBytes+=found->size;
            return found;
        }else {
            found = found->next;
        }
    }
    return NULL;
}

void BList::Binsert(MetaD *m) {
    MetaD *nxt = head;
    MetaD *prev = head;
    if (!head) {//list is empty
        head = m;
        return;
    }
    while (nxt) {
        prev = nxt;
        nxt = nxt->next;
    }
    m->prev = prev;
    prev->next = m;
}

MetaD* BList::Ballocte(size_t size) {
    void* p= sbrk(size+sizeof(MetaD));//different!!
    if(p== (void*)(-1)){
        return NULL;
    }
    ///add a new block to the list
    MetaD * meta = (MetaD *) p;
    meta->is_free=false;
    meta->size=size;
    meta->next=NULL;
    meta->prev=NULL;
    Binsert(meta);
    AllocatedBlocks++;
    AllocatedBytes+=size;
    return meta;
}


////////////////////requested functions///////////////////////////
BList Blocks=BList();
void* smalloc(size_t size){
    if(size==0 || size> HUNDRED_MILL){
        return NULL;
    }
    MetaD* location=Blocks.Bfind(size);
    if(location) {
        return location+1;
    }else{
        location=Blocks.Ballocte(size);
        if(location){
            return location+1;
        }
        return NULL;
    }
}

void* scalloc(size_t num, size_t size){
    if(num==0 || size==0 || size*num>HUNDRED_MILL){
        return NULL;
    }
    size_t fullSize=size*num;
    void* p=smalloc(fullSize);
    if(p== NULL){
        return NULL;
    }
    memset(p,0,fullSize);
    return p;
}

void sfree(void* p){
  if(p){
      MetaD* meta= (MetaD*)p-1;
      if(!meta->is_free) {
          meta->is_free = true;
          Blocks.AllocatedBytes -= meta->size;
          Blocks.AllocatedBlocks--;
          Blocks.FreeBytes += meta->size;
          Blocks.FreeBlocks++;
      }
  }
}

void* srealloc(void* oldp, size_t size){
    if(size==0 || size> HUNDRED_MILL){
        return NULL;
    }
    if(oldp) {
        MetaD *OldMeta = (MetaD*) oldp - 1;
        if (size <= OldMeta->size) {
            return oldp;
        }else{
            void* p= smalloc(size);
            if(p== NULL){
                return NULL;
            }
           p= memmove(p,oldp,OldMeta->size);
           sfree(oldp);
           return p;
        }
    }else{
        void* p= smalloc(size);
        if(p== NULL){
            return NULL;
        }
        return p;
    }

}

size_t _num_free_blocks(){
    return Blocks.FreeBlocks;
}
// Returns the number of allocated blocks in the heap that are currently free.
size_t _num_free_bytes(){
    return Blocks.FreeBytes;
}
//Returns the number of bytes in all allocated blocks in the heap that are currently free,
//excluding the bytes used by the meta-data structs.
size_t _num_allocated_blocks(){
    return Blocks.AllocatedBlocks + Blocks.FreeBlocks;
}
// the overall (free and used) number of allocated blocks in the heap.
size_t _num_allocated_bytes(){
    return Blocks.AllocatedBytes +Blocks.FreeBytes;
}
// Returns the overall number (free and used) of allocated bytes in the heap, excluding
//the bytes used by the meta-data structs.

size_t _num_meta_data_bytes(){
    return (Blocks.AllocatedBlocks+Blocks.FreeBlocks)*sizeof(MetaD);
}
// Returns the overall number of meta-data bytes currently in the heap.
size_t _size_meta_data(){
    return sizeof(MetaD);

}
//Returns the number of bytes of a single meta-data structure in your system