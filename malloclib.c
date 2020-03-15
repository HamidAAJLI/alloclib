//
// Created by aajli on 01/03/2020.
//

#include "malloclib.h"

void afficheAllocInfo(){
    printf("\nmalloc Infos:");
    printf("\n total malloc demande : %lu", mallocDem);
    printf("\n total mmap alloue : %lu", mmapAlloc);
    printf("\n total free demande : %lu", mallocDem);
    printf("\n total free vidée : %lu", mallocDem);

};

void * rechMem(size_t size)
{
    strInformation *memR = mem;
    strInformation *memBs = NULL;
    int size_min = 2147483647;

    while(memR != NULL)
    {
        if(memR->size >= size && memR->size < size_min)
        {
            memBs = memR;
            size_min = memR->size;
        }
        memR = memR->next;
    }

    void *res = NULL;

    if(memBs != NULL)
    {
        if (memBs != mem)
        {
            memR = mem;
            while(memR != NULL && memR->next != memBs)
            {
                memR = memR->next;
            }
            if(memR != NULL)
            {
                memR->next = memBs->next;
            }
            memBs->next = NULL;
            res = (memBs + sizeof(strInformation));

        }
        else
        {
            mem = mem->next;
            memBs->next = NULL;
            res = (memBs + sizeof(strInformation));
        }
    }

    return res;
}

void * mmapfct(size_t size)
{

    pthread_mutex_lock(&mutexNb);
    mmapAlloc += size;
    pthread_mutex_unlock(&mutexNb);

    int pages =((size + sizeof(strInformation) - 1)/sysconf(_SC_PAGESIZE)) + 1;
    int size_pages = sysconf(_SC_PAGESIZE) * pages;

    void *res = mmap(NULL,size_pages,PROT_READ | PROT_WRITE,MAP_ANONYMOUS| MAP_PRIVATE,-1, 0);

    strInformation memR;
    memR.size = (size_pages - sizeof(strInformation));
    memR.next = NULL;

    res = memcpy(res, &memR, sizeof(strInformation));
    res = ((char*)res + sizeof(strInformation));

    return res;
}

void *allocMem(size_t size)
{

    void * res = NULL;
    if(mem != NULL)
    {
        res = rechMem(size);
    }

    if(res == NULL)
    {
        res = mmapfct(size);
    }
    return res;
}

void* malloc(size_t size)
{
    pthread_mutex_lock(&mutexNb);
    mallocDem++;
    pthread_mutex_unlock(&mutexNb);

    void * res = NULL;

    if(size < 0)
    {
        return NULL;
    }else
    {
        res = allocMem(size);
    }


    return res;
}

void free(void *memF)
{
    pthread_mutex_lock(&mutexNb);
    freeDem++;
    freeRange++;
    pthread_mutex_unlock(&mutexNb);

    if(memF != NULL)
    {
        strInformation *memB  = (memF - sizeof(strInformation));
        memset(memF, '\0', memB->size);

        strInformation **block = &mem;

        strInformation *checkB = *block;

        while(checkB != NULL)
        {
            if(checkB == memB)
            {
                return;
            }
            checkB = checkB->next;
        }

        memB->next = *block;
        *block = memB;
    }
}

void *realloc(void *memRe, size_t size)
{
    if(memRe == NULL)
    {
        return malloc(size);
    }

    void *memNe = malloc(size);

    if(NULL == memNe)
    {
        return NULL;
    }

    strInformation *old_block =(strInformation *)(memRe - sizeof(strInformation));

    memcpy(memNe, memRe, old_block->size);

    free(memRe);

    return memNe;
}

void *calloc(size_t nmemb, size_t size)
{
    void *memC = malloc(nmemb * size);
    strInformation *memR = (strInformation *)(memC - sizeof(strInformation));
    memset(memC, '\0', memR->size);
    return memC;
}
