/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/


#include "memory.h"

#define MEMORY_SIZE 1000
#define MEMORY_NB_BITS 32


Memory::Memory() // Constructeur
{
    int ret = initialize(MEMORY_SIZE, MEMORY_NB_BITS);

    if (!ret){
        printf("Failed to init the memory");
        exit(1);
    }
}

Memory::~Memory() // Destructeur
{

}

int Memory::initialize(int size, int nBitsData){
    _memoryPtr = fopen("./.memory.bin", "w+");
    char* str = "toto";
    fwrite(str, 1, sizeof(str), _memoryPtr);
}

