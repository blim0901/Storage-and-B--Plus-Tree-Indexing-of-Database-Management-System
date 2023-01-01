// prevents double declaration of any identifiers such as types etc
#ifndef RECORD_H
#define RECORD_H

#include <string>

// Defines a Record struc for storing fields as:
// tconst: String (9 chars --> 9 bytes)
// averageRating: Float (4 bytes)
// numVotes: Int (4 bytes)

// Storing the above as the datatypes ensures that size of records are fixed and constant
// Easier to enforce Block Size limit

struct Record{
    public: 
        std::string tconst;
        float averageRating;
        int numVotes;

        // insert fields into a Record
        void insertRecord(std::string tconst, std::string averageRating, std::string numVotes);

        // retrieve fields from Records
        std::string retrieveTCONST();
        float retrieveAverageRating();
        int retrieveNumVotes();


};

#endif