#include <stdio.h>
#include <strings.h>
#include "mymalloc.h"

int main()
{
    void* ptr1 = mymalloc(128);
    void* ptr2 = mymalloc(128);
    void* ptr3 = mymalloc(128);
    void* ptr4 = mymalloc(128);
    void* ptr5 = mymalloc(128);
    myfree(ptr2);
    myfree(ptr4);
    myfree(ptr3);
    /*myfree(ptr4);
    myfree(ptr3);
    myfree(ptr2);*/
    myfree(mymalloc(416));
    mymalloc(417);
    return 0;
}
