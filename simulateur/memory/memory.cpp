/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/

#include "memory.h"


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

    for (int i = 0; i < NB_BLOC; i++){
        if (addr == i){
            fseek(_memoryPtr, 0, SEEK_CUR);
            fprintf(_memoryPtr, "0x%04x 0x%04x", addr, data);
        }
        fscanf(_memoryPtr, "%s %s\n", buf1, buf2);
    }

    return 0;
}

int Memory::read(int addr){
    char addrRead[11] = "";
    char dataRead[11] = "";

    fseek(_memoryPtr, 0, SEEK_SET);

    for (int i = 0; i < NB_BLOC; i++){
        fscanf(_memoryPtr, "%s %s", addrRead, dataRead);
        if (addr == (int)strtol(addrRead, NULL, 0)){
            return (int)strtol(dataRead, NULL, 0);
        }
    }
}

int Memory::init(){
    char* filePath = strcat(dirname(realpath(__FILE__, NULL)), "/.memory.bin");
    _memoryPtr = fopen(filePath, "w+");

    for(int i = 0; i < NB_BLOC; i++){
        fprintf(_memoryPtr, "0x%04x %s \n", i, "0x0000");
    }
}

