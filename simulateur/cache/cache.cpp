/********************************************************
*                                                       *
*    PROJECT: ISS in C                                  *
*    ORGANIZATION: ENSTA Bretagne FIPASE 2024           *
*    AUTEUR: Tanguy ROUDAUT, Melvin DUBEE               *
*    DATE: 15/03/2023                                   *
*                                                       *
*********************************************************/

#include "cache.h"

Cache::Cache() 
{
    init();
}

Cache::~Cache()
{
}

void Cache::write(int addr, int data){
    int indexLineHigherCacheMiss = _line[0].miss;
    for (int i = 0; i < NB_BLOC_CACHE; i++) {
        if (!_line[i].valid || _line[i].tag == addr){
            _line[i].data = data;
            _line[i].valid = 1;
            _line[i].tag = addr;
            _line[i].miss = 0;
            writeThrough(addr, data);
            break;
        }
        else if (_line[i].miss > indexLineHigherCacheMiss) {
            indexLineHigherCacheMiss = _line[i].miss;
        }
    }

    _line[indexLineHigherCacheMiss].data = data;
    _line[indexLineHigherCacheMiss].valid = 1;
    _line[indexLineHigherCacheMiss].tag = addr;
    _line[indexLineHigherCacheMiss].miss = 0;
    writeThrough(addr, data);
}

int Cache::read(int addr){
    for (int i = 0; i < NB_BLOC_CACHE; i++) {
        if (_line[i].valid && _line[i].tag == addr){
            return _line[i].data;
        }
        _line[i].miss++;
    }
    _cacheMiss = 1;

    int indexLineHigherCacheMiss = _line[0].miss;
    for (int i = 0; i < NB_BLOC_CACHE; i++) {
        if (!_line[i].valid){
            _line[i].data = _mem.read(addr);
            _line[i].valid = 1;
            _line[i].tag = addr;
            _line[i].miss = 0;
            return _line[i].data;
        }
        else if (_line[i].miss > indexLineHigherCacheMiss) {
            indexLineHigherCacheMiss = _line[i].miss;
        }
    }
     
    _line[indexLineHigherCacheMiss].data = _mem.read(addr);
    _line[indexLineHigherCacheMiss].valid = 1;
    _line[indexLineHigherCacheMiss].tag = addr;
    _line[indexLineHigherCacheMiss].miss = 0;
    return _line[indexLineHigherCacheMiss].data;
}

int Cache::getCacheMiss(){
    int lastCacheMiss = _cacheMiss;
    _cacheMiss = 0;
    return lastCacheMiss;
}

void Cache::writeThrough(int addr, int data){
    _mem.write(addr, data);
}

void Cache::init(){
    for (int i = 0; i < NB_BLOC_CACHE; i++) {
        _line[i] = {};
    }
}