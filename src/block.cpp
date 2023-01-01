# include <vector>
# include <iostream>
# include <string>

# include "block.h"

int Block::numRecordsInBlock(){
    return records.size(); // returns number of records stored currently
}

void Block::insertRecord(Record aRecord){
    // error checking for full block
    if (numRecords >= maximumRecords){
        std::cout << "Block is full as records size = " << numRecords << std::endl;
        return;
    }
    records.push_back(aRecord);
    numRecords++;
    //std::cout << "recordsnum++ to " << numRecords << std::endl;
    //std::cout << records.size() << std::endl;
}

// delete (index+1)th element
void Block::deleteRecord(int index){
    // error checking for empty block
    if (records.size() == 0){
        std::cout << "Block is already empty" << std::endl;
        return;
    }
    numRecords--;
    records.erase(records.begin()+index);
}

std::vector<Record> Block::retrieveRecords(int numVotes){
    // initialise local variable of array of records to store records found as there will be duplicates
    std::vector<Record> recordsWithSameKey;

    // iterate through all records in block and append those with same numVotes to array
    for (Record record:records){
        if (record.numVotes == numVotes){
            recordsWithSameKey.push_back(record);
        }
    }
    // std::cout << "Num of Records found: " << recordsWithSameKey.size() << std::endl;
    return recordsWithSameKey;
}