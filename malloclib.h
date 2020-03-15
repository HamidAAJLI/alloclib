//
// Created by aajli on 01/03/2020.
//

#ifndef ALLOCLIB_MALLOCLIB_H
#define ALLOCLIB_MALLOCLIB_H



#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>



void * malloc(size_t);

void free(void *p);

void * calloc(size_t nmemb, size_t size);

void * realloc(void *ptr, size_t size);

typedef struct strInformation
{
    int size;
    struct strInformation *next;
}strInformation;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mutexNb = PTHREAD_MUTEX_INITIALIZER;

unsigned long mmapAlloc = 0;

unsigned long mallocDem = 0;

unsigned long freeDem = 0;

unsigned long freeRange = 0;

__thread strInformation *mem = NULL;

void * rechMem(size_t size);

void * mmapFct(size_t size);

void * allocMem(size_t size);

void afficheAllocInfo();


#endif //ALLOCLIB_MALLOCLIB_H
