//
// Created by ניב שפק on 1/2/2020.
//

#include "UnionFind.h"


#ifndef WET2_DATAMANAGER_H
#define WET2_DATAMANAGER_H


class DataManager {
    int size;
    UnionFInd<HTData> DM;
public:
    DataManager(int n):size(n), DM(n) {};
    void mergeDC(int id1, int id2){
        if(id1<=0 || id2 <=0 || id1>this->size || id2>this->size)
            throw InvalidInputException();
        this->DM.unionGroups(this->DM.getGroup(id1), this->DM.getGroup(id2));
    }
    void addServer(int DC_id, int server_id){
        if (DC_id<=0 || DC_id>this->size || server_id<=0)
            throw InvalidInputException();
        this->DM.addData(DC_id, server_id);
    }
    void removeServer(int server_id){
        if (server_id<=0)
            throw InvalidInputException();
        this->DM.removeData(server_id);
    }
    void setTraffic(int server_id, int traffic){
        if (traffic<0 || server_id<=0)
            throw InvalidInputException();
        this->DM.setParam(server_id, traffic);
    }
    int sumTrafficK(int DC_id, int k){
        if (k<0 ||DC_id>this->size || DC_id<0)
            throw InvalidInputException();
        return this->DM.sumHeighestParam(DC_id, k);
    }
    UnionFInd<HTData>* getUF(){
        return &(this->DM);
    }

};


#endif //WET2_DATAMANAGER_H
