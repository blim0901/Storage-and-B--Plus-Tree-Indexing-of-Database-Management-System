// prevents double declaration of any identifiers such as types etc
#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <iostream>
#include <cmath>
#include "record.h"

using std::cout;
using std::endl;

// Defines a Block struc for storing records as a vector of Record strucs
// Since Records all of same length, store the Records contiguously in a Block
// (no need for linked lists/pointers, ie records are *unspanned* within a block)

struct Block{
    public: 
        // Max number of records per block
        // 100 - 500 MB disk capacity
        // 200 - 500B block size
        // Record : 9bytes (9char - "string") + 4bytes (int) + 4bytes (float) = 17bytes + 6 bytes of attributes below:
        int maximumRecords; // 4 byte
        int blockSize; // 4 byte
        int numRecords; // 4 bytes 

        // vector of Records in this block
        std::vector <Record> records; // assume 0 bytes used to set up and maintain vector

        // constructor with setting blockSize
        explicit Block(int blockSize) : blockSize(blockSize) {
            Block::setBlockSize(blockSize);
            Block::numRecords = 0;
        }

        // set size for each block (in Bytes)
        // assuming that records are stored in an *unspanned* manner
        void setBlockSize(int size){
            cout << size << endl;
            cout << sizeof(Record) <<endl;
            cout << (size-12)/40 << endl;

            //cout << "Block Size set at: " << size << "bytes" <<  endl; // for debugging/UX purposes
            int maximumRecords = floor((size-12)/40); // need to minus 12 for the  cos of the attributes of Block struc (maxRecords is an int thats 4 bytes long, so is BlockSize)
            cout << "Max Number of Records per Block: " << maximumRecords << endl;

            // store maxRecords and blockSize into the Block struc
            // to be retreived later on when calculating disk size etc
            Block::maximumRecords = maximumRecords;
            Block::blockSize = blockSize;
        }

        // return current number of records stored in block
        int numRecordsInBlock();

        // insert Record to Block
        void insertRecord(Record aRecord);

        // delete Record at this index from Block
        void deleteRecord(int index);

        // retrieve Records with numVotes = key 
        // in this specific project, key refers to the numVotes attribute
        std::vector<Record> retrieveRecords(int numVotes);

};

#endif