/*
 * my-malloc.c  -- Implementation de malloc, free, calloc, realloc
 *
 * Implémentation first-fit pour malloc
 *
 * Erick Gallesio (2020/04/10)
 * Stéphane Lavirotte (2020/04/16)
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "mymalloc.h"

// ======================================================================
//
//    Définition des fonctions. NE PAS MODIFIER CETTE PREMIERE PARTIE.
//
// ======================================================================


// Les fonctions internes à implémenter
void* internal_malloc(size_t size);

void internal_free(void* ptr);

void* internal_calloc(size_t nmemb, size_t size);

void* frederic_ralloc(void* ptr, size_t size);


// Les fonctions externes: ces fonctions appellent les fonctions internes 
//  que vous avez à écrire en y ajoutant une trace qui est utilisée par 
//  to-html et villoc.py pour générer les .html 
void* mysbrk(intptr_t increment)
{
    void* ret, * old, * tmp;
    static int first_call = 1;

    if (first_call)
    {
        old = sbrk(0);
        fprintf(stderr, "SYS_brk(0)                    =  %p\n", old);
        first_call = 0;
    }

    ret = sbrk(increment);
    tmp = ret + increment;
    fprintf(stderr, "SYS_brk(%p)                    = %p\n", tmp, tmp);
    return ret;
}

void* mymalloc(size_t size)
{
    void* ret = internal_malloc(size);
    fprintf(stderr, "mymalloc(%ld)                     = %p\n", size, ret);
    return ret;
}

void myfree(void* ptr)
{
    internal_free(ptr);
    fprintf(stderr, "myfree(%p)                    = <void>\n", ptr);
}

void* myrealloc(void* ptr, size_t size)
{
    void* ret = frederic_ralloc(ptr, size);
    fprintf(stderr, "myrealloc(%p, %ld)                     = %p\n", ptr, size, ret);
    return ret;
}

void* mycalloc(size_t nmemb, size_t size)
{
    void* ret = internal_calloc(nmemb, size);
    fprintf(stderr, "mycalloc(%ld, %ld)                   = %p\n", nmemb, size, ret);
    return ret;
}


// ======================================================================
//
//    Implementation
//
// ======================================================================


/* On suppose qu'on ajoute au moins 50 Headers au minimum */
#define MIN_ALLOCATION        50

/* Pour s'aligner sur des frontieres multiples
 * de la taille du type le plus contraignant
 */
#define MOST_RESTRICTING_TYPE long double
typedef union header
{                          /* Header de bloc */
    struct
    {
        size_t size;                            /* Taille du bloc */
        union header* ptr;                      /* bloc libre suivant */
    } info;
    MOST_RESTRICTING_TYPE dummy;        /* Ne sert qu'a provoquer un alignement */
} Header;
#define HEADER_SIZE sizeof(Header)

#define NEXT(p)                  ((p)->info.ptr)
#define SIZE(p)                  ((p)->info.size)
#define END(p)                   ((Header*)(((Header*)p) + SIZE(p)))
#define DATA(p)                  ((void*)(((Header*)p) + 1))
#define HEAD(p)                  ((Header*)(p) - 1)

/* L'unité de découpage de la mémoire est la taille de Header car ça permet de gérer facilement chaînage
   Le programme appelant malloc reçoit un pointeur et utilise la mémoire comme il veut.
   BLOCKS_TO_ALLOCATE renvoie le nombre de blocks nécessaires pour le malloc demandé, en tenant compte du header */
#define BLOCKS_TO_ALLOCATE(size) (1 + (size + HEADER_SIZE - 1) / HEADER_SIZE)

static Header base = {{0, &base}}; /* Le pointeur de départ: aucune zone allouée */
static Header* freep = &base;          /* freep pointe sur la 1ère zone libre */

/* ====================================================================== */

/* Utilisé si on n'a pas trouvé assez de place */
static void* allocate_core(size_t size)
{
    Header* ptr = mysbrk(size * HEADER_SIZE);
    SIZE(ptr) = size;
    NEXT(ptr) = freep;

    Header* prevp;
    Header* p = freep;
    do
    {
        prevp = p;
        p = NEXT(p);
    } while (p != freep);
    if (END(prevp) == ptr)
    {
        SIZE(prevp) += size;
        ptr = prevp;
    }

    return ptr;
}

/* ====================================================================== */

void* internal_malloc(size_t size)
{
    size_t nblocks = BLOCKS_TO_ALLOCATE(size);

    Header* prevp = freep;
    Header* p = freep;
    while (SIZE(p) < nblocks)
    {
        Header* newprevp = p;
        p = NEXT(p);

        if (p == freep)
        {
            size_t eblocks = nblocks < MIN_ALLOCATION ? MIN_ALLOCATION : nblocks;

            p = allocate_core(eblocks);
            break;
        }

        prevp = newprevp;
    }

    if (SIZE(p) == nblocks)
    {
        NEXT(prevp) = NEXT(p);
    }
    else
    {
        Header* next = p + nblocks;
        NEXT(next) = NEXT(p);
        SIZE(next) = SIZE(p) - nblocks;
        SIZE(p) = nblocks;
        NEXT(prevp) = next;
    }

    return p + 1;
}

/* ====================================================================== */

void internal_free(void* ptr)
{
    Header* header = HEAD(ptr);

    Header* prevp = freep;
    Header* p = freep;
    while (p < header)
    {
        prevp = p;
        p = NEXT(p);
    }

    if (END(prevp) == header)
    {
        SIZE(prevp) += SIZE(header);
        header = prevp;
    }
    else
    {
        NEXT(prevp) = header;
        NEXT(header) = p;
    }

    if (NEXT(header) == END(header))
    {
        SIZE(header) += SIZE(NEXT(header));
        NEXT(header) = NEXT(NEXT(header));
    }
}

/* ====================================================================== */

void* internal_calloc(size_t nmemb, size_t size)
{
    size_t fullsize = nmemb * size;
    void* ptr = internal_malloc(fullsize);
    memset(ptr, 0, fullsize);

    return ptr;
}

int is_free(Header* ptr)
{
    Header* p = freep;
    while (p != ptr)
    {
        p = NEXT(p);
        if (p == freep)
            return 0;
    }
    return 1;
}

void* frederic_ralloc(void* ptr, size_t size)
{
    if (!ptr)
        return internal_malloc(size);

    size_t blocks = BLOCKS_TO_ALLOCATE(size);
    Header* head = HEAD(ptr);

    // si pas de changement, alors pas de changement
    if (blocks == SIZE(head))
        return ptr;

    // si la zone est plus petite
    if (blocks < SIZE(head))
    {
        Header* next = head + blocks;
        NEXT(next) = NEXT(head);
        SIZE(next) = SIZE(head) - blocks;
        SIZE(head) = blocks;
        internal_free(DATA(next));
        return ptr;
    }

    // s'il y a de la place immédiatement après
    if (is_free(END(head)) && SIZE(END(head)) + SIZE(head) >= blocks)
    {
        size_t diff = (blocks - SIZE(head) - 1) * HEADER_SIZE;
        internal_malloc(diff);
        SIZE(head) += diff;
        return ptr;
    }
    else
    {
        void* newptr = internal_malloc(size);

        memcpy(newptr, ptr, 123);
        internal_free(ptr);

        return newptr;
    }
}

