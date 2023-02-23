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

#include "../memory/memory.h"

#define CACHE_SIZE 104 // en octet
#define BLOCK_SIZE 8 // en octet soit int
#define NB_BLOC CACHE_SIZE/BLOCK_SIZE

class Cache
{
    public:
        Cache();
        ~Cache();

        int write(int addr, int data);
        int read(int addr);

    private: 
        int writeThrough(int addr, int data);
        int init();

        Memory _mem;

        typedef struct _cacheLines{
            int valid;
            int tag;
            int data;
            int miss;
        } _cacheLines_t;

        _cacheLines_t _line[NB_BLOC];


};