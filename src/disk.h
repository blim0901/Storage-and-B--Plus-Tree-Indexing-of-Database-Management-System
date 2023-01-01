// prevents double declaration of any identifiers such as types etc
# ifndef DISK_H
# define DISK_H

# include <vector>
#include <memory>
# include "block.h"

// an emulation of the disk in software, using a vector/array/list of pointers to block strucs that in totality makes up a disk
// such an approach was chosen in order to overcome the limitation that a machine may not have enough contiguous memory space to store the entire database

struct Disk{

    std::vector <std::shared_ptr<Block>> blockVector;
    public:
        // returns the number of blocks in array currently
        int getNumOfBlocks();

        // returns the number of records in total across all blocks in array currently
        int getNumOfRecords();

        // returns the current storage size (for logging)
        int getSizeOfStorage();

        // add a block to array
        void addBlock(std::shared_ptr<Block> newBlock);
};

#endif //