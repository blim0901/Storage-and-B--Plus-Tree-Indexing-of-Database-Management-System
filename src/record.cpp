# include <string>
# include <iostream>

# include "record.h"

void Record::insertRecord(std::string tconst, std::string averageRating, std::string numVotes){
    // the record takes data from tsv as String type, but stores internally as float/int type
    tconst = tconst;
    averageRating = stof(averageRating);
    numVotes = stoi(numVotes);
}

std::string Record::retrieveTCONST(){
    return tconst;
};
float Record::retrieveAverageRating(){
    return averageRating;
};
int Record::retrieveNumVotes(){
    return numVotes;
};
