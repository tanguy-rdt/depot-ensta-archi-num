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

class Memory
{
    
    public:
        Memory();
        ~Memory();
        
        int write(int addr, int data);
        int read(int addr);

    private: 
        int init();

        char* _memoryFile = "./.memory.bin";
};