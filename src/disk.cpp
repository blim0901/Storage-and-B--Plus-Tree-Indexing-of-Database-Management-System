# include <vector>
#include <memory>
# include "disk.h"
# include "record.h"
// defines methods for the Disk struc, which is an emulation of the actual disk hardware

int Disk::getNumOfBlocks(){
    // simply returns number of blocks currently in disk/memory/blockVector array
    // cout << "numblocks " << blockVector.size() << endl; // for debug purposes
    return blockVector.size();
}

int Disk::getNumOfRecords(){
    int numRecords = 0;
    for (int i=0 ; i<getNumOfBlocks() ; i++){
        numRecords += blockVector[i]->numRecordsInBlock();
    }
    cout << "numRecords " << numRecords << endl; // for debug, comment out
    return numRecords;
}

int Disk::getSizeOfStorage(){
    // returns size of all blocks currently in memory
    int storageSize = 0;

    // for (int j=0; j<getNumOfBlocks(); j++){
    //     storageSize += blockVector[j]->blockSize;
    //     cout << "storagesize is now  " << storageSize << endl; // for debug, comment out
    // }
    

    storageSize += getNumOfRecords() * sizeof(Record); //size of a record
    return storageSize;
}

void Disk::addBlock(std::shared_ptr<Block> newBlock){
    // adds a new block pointer to the block array
    // Block *blkptr = &newBlock;
    blockVector.push_back(newBlock);
}

// find block given block address? using the sharedpointer vector?