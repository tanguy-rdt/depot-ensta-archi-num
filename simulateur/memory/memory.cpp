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


Memory::Memory(){
    int ret = init();

    if (!ret){
        printf("Failed to init the memory");
        exit(1);
    }
}

Memory::~Memory(){
    fclose(_memoryPtr);
}

int Memory::write(int addr, int data){
    char buf1[11] = "";
    char buf2[11] = "";

    fseek(_memoryPtr, 0, SEEK_SET);

    for (int i = 0; i < MEMORY_SIZE; i++){
        if (addr == i){
            fseek(_memoryPtr, 0, SEEK_CUR);
            fprintf(_memoryPtr, "0x%08x 0x%08x", addr, data);
        }
        fscanf(_memoryPtr, "%s %s\n", buf1, buf2);
    }

    return 0;
}

int Memory::read(int addr){
    char addrRead[11] = "";
    char dataRead[11] = "";

    fseek(_memoryPtr, 0, SEEK_SET);

    for (int i = 0; i < MEMORY_SIZE; i++){
        fscanf(_memoryPtr, "%s %s", addrRead, dataRead);
        if (addr == (int)strtol(addrRead, NULL, 0)){
            return (int)strtol(dataRead, NULL, 0);

        }
    }
}

int Memory::init(){
    _memoryPtr = fopen("./.memory.bin", "r+");

    for(int i = 0; i < MEMORY_SIZE; i++){
        fprintf(_memoryPtr, "0x%08x %s \n", i, "0x00000000");
    }
}

