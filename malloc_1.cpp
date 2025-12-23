//
// Created by garam on 8/17/2024.
//

#include <unistd.h>
#include <cstddef>
#define ONE_HUNDRED_MILLION 100000000
//10e7 is double
void* smalloc(size_t size){
    if(size==0 || size> ONE_HUNDRED_MILLION){
        return NULL;
    }
    void* res=sbrk(size);
    if(res==(void*)(-1)){
      return NULL;
    }
    return res;
}



