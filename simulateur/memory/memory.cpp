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
    int ret = initialize();

    if (!ret){
        printf("Failed to init the memory");
        exit(1);
    }
}

Memory::~Memory(){

}

int Memory::write(int addr, int data){
    FILE* _memoryPtr = fopen(_memoryFile, "r");
    FILE* _tmpPtr = fopen("./.memory.tmp.bin", "w");

    char addrRead[11] = "";
    char dataRead[11] = "";

    for (int i = 0; i < MEMORY_SIZE; i++){
        fscanf(_memoryPtr, "%s %s\n", addrRead, dataRead);
        if (addr == (int)strtol(addrRead, NULL, 0)){
            fprintf(_tmpPtr, "0x%08x 0x%08x\n", addr, data);
        }
        else {
            fprintf(_tmpPtr, "%s %s\n", addrRead, dataRead);
        }
    }

    fclose(_memoryPtr);
    fclose(_tmpPtr);

    remove(_memoryFile);
    rename("./.memory.tmp.bin", _memoryFile);

    if (read(addr) == data)
        return 0;

    return -1;
}

int Memory::read(int addr){
    FILE* _memoryPtr = fopen(_memoryFile, "r");

    char addrRead[11] = "";
    char dataRead[11] = "";

    for (int i = 0; i < MEMORY_SIZE; i++){
        fscanf(_memoryPtr, "%s %s", addrRead, dataRead);
        if (addr == (int)strtol(addrRead, NULL, 0)){
            fclose(_memoryPtr);
            return (int)strtol(dataRead, NULL, 0);

        }
    }

    fclose(_memoryPtr);
}

int Memory::initialize(){
    FILE* _memoryPtr = fopen(_memoryFile, "w+");

    for(int i = 0; i < MEMORY_SIZE; i++){
        fprintf(_memoryPtr, "0x%08x %s \n", i, "0x00000000");
    }

    fclose(_memoryPtr);
}

