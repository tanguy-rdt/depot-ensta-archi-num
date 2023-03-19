/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/

#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

#define CACHE_SIZE 512 
#define BLOCK_SIZE 4
#define NB_BLOC_CACHE CACHE_SIZE/BLOCK_SIZE

class Cache
{
    public:
        Cache();
        ~Cache();

        void write(int addr, int data);
        int read(int addr);
        int getCacheMiss();

    private: 
        void writeThrough(int addr, int data);
        void init();

        Memory _mem;

        typedef struct _cacheLines{
            int valid;
            int tag;
            int data;
            int miss;
        } _cacheLines_t;

        _cacheLines_t _line[NB_BLOC_CACHE];

        int _cacheMiss = 0;
};