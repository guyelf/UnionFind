//
// Created by ניב שפק on 1/2/2020.
//

#include "library2.h"
#include "DataManager.h"
#include <new>

void* Init(int n){
    DataManager* DS = new DataManager(n);
    return (void*)DS;
}

StatusType MergeDataCenters(void *DS, int dataCenter1, int dataCenter2) {
    if (!DS) return INVALID_INPUT;
    try {
        ((DataManager *) DS)->mergeDC(dataCenter1, dataCenter2);
    }
    catch (const FailException){ return FAILURE; }
    catch (InvalidInputException) { return INVALID_INPUT; }
    catch (std::bad_alloc& e) { return ALLOCATION_ERROR; }
    catch(...){}
    return SUCCESS;
}

StatusType AddServer(void *DS, int dataCenterID, int serverID){
    if (!DS) return INVALID_INPUT;
    try {
        ((DataManager *)DS)->addServer(dataCenterID,serverID);
    }
    catch (const FailException){ return FAILURE; }
    catch (InvalidInputException) { return INVALID_INPUT; }
    catch (std::bad_alloc) { return ALLOCATION_ERROR; }
    catch(...){}
    return SUCCESS;
}

StatusType RemoveServer(void *DS, int serverID){
    if (!DS) return INVALID_INPUT;
    try {
        ((DataManager *) DS)->removeServer(serverID);
    }
    catch (const FailException){ return FAILURE; }
    catch (InvalidInputException) { return INVALID_INPUT; }
    catch (std::bad_alloc) { return ALLOCATION_ERROR; }
    catch(...){}
    return SUCCESS;
}

StatusType SetTraffic(void *DS, int serverID,  int traffic){
    if (!DS || traffic<0) return INVALID_INPUT;
    try {
        ((DataManager *) DS)->setTraffic(serverID,traffic);
    }
    catch (const FailException){ return FAILURE; }
    catch (InvalidInputException) { return INVALID_INPUT; }
    catch (std::bad_alloc) { return ALLOCATION_ERROR; }
    catch(...){}
    return SUCCESS;
}

StatusType SumHighestTrafficServers(void *DS, int dataCenterID,  int k, int *traffic){
    if (!DS || !traffic || k<0) return INVALID_INPUT;
    try {
        *traffic = ((DataManager *) DS)->sumTrafficK(dataCenterID,k);
    }
    catch (const FailException){ return FAILURE; }
    catch (InvalidInputException) { return INVALID_INPUT; }
    catch (std::bad_alloc) { return ALLOCATION_ERROR; }
    catch(...){}
    return SUCCESS;
}

void Quit(void** DS){
    delete *((DataManager**)DS);
    *DS = nullptr;
}