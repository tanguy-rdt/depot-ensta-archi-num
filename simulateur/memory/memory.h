/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/


class Memory
{
    
    public:
        int write(int addr, int data);
        int read(int addr, int data);

    private: 
        int initialize(int size, int nBitsData);


};