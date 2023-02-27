/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <libgen.h>

#define MEMORY_SIZE 1024 // en octet
#define BLOCK_SIZE 8 // en octet soit int
#define NB_BLOC MEMORY_SIZE/BLOCK_SIZE


class Memory
{
    
    public:
        Memory();
        ~Memory();
        
        int write(int addr, int data);
        int read(int addr);

    private: 
        int init();

        FILE* _memoryPtr;
};