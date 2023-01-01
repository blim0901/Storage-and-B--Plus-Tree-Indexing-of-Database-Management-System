#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <set>
#include <climits>
#include <tgmath.h>

#include "record.h"
#include "block.h"
#include "disk.h"

using namespace std;

int MAX; //size of each node
class BPTree;
class LL
{
    int key;
    Block* pointToBlock;
    LL* nextLL;
    friend class Node;
    friend class BPTree;
public:
    LL();
};
class Node
{
    bool isLeaf;
    int *key, size;
    Node** children;
    Block** ptr;
    LL** LLholder;
    Node* leftSib;
    Node* rightSib;
    friend class BPTree;
    void delKeyInternal(Node* child, int x);
public:
    Node();
    bool canLeafBorrow();
    bool canInternalBorrow();
    bool canLeafMerge();
    bool canInternalMerge();
    bool addKey(int k, Block* block);
    void getContent();
};
class BPTree
{
    Node *root;
    void insertInternal(Node*,Node*,int);
    void removeInternal(Node*,Node*,int);
    Node* findParent(Node*,Node*);
    int numOfNodes;
    int numOfDeletion;
public:
    BPTree();
    void search(int);
    void searchRange(int, int);
    void insert(int, Block* block);
    void remove(int);
    Node* getRoot();
    int getNumOfNodes();
    int getNumDeletion();
    void printNode(Node*);
    Node* getFirstChild(Node*);
    int getHeight(Node*);
};
//give command line argument to load a tree from log
//to create a fresh tree, do not give any command line argument
Node::Node()
{
    //dynamic memory allocation
    key = new int[MAX];
    children = new Node*[MAX+1];
    ptr = new Block*[MAX+1];
    LLholder = new LL*[MAX+1];
}
BPTree::BPTree()
{
    root = NULL;
    numOfNodes = 0;
    numOfDeletion = 0;
}
LL::LL() {
    pointToBlock = NULL;
    nextLL = NULL;
}

void BPTree::search(int x)
{
    // Can insert search function here
    //search logic
    if(root==NULL)
    {
        //empty
        cout<<"Tree empty\n";
    }
    else
    {
        Node* cursor = root;
        int nodeAccessed = 1;

        cout << "-------Content of index blocks accessed--------" << endl;
        //in the following while loop, cursor will travel to the leaf node possibly consisting the key
        while(cursor->isLeaf == false)
        {
            if  (nodeAccessed < 6) {
                cout << "Index block " << nodeAccessed << endl;
                for (int i = 0; i < cursor->size; i++) {
                    cout << cursor->key[i] << '|';
                }
                cout << "\n";
            }
            for(int i = 0; i < cursor->size; i++)
            {
                if(x < cursor->key[i])
                {
                    cursor = cursor->children[i];
                    nodeAccessed++;
                    break;
                }
                if(i == cursor->size - 1)
                {
                    cursor = cursor->children[i+1];
                    nodeAccessed++;
                    break;
                }
            }
        }
        if  (nodeAccessed < 6) {
            cout << "Index block " << nodeAccessed << endl;
            for (int i = 0; i < cursor->size; i++) {
                cout << cursor->key[i] << '|';
            }
            cout << "\n";
        }

        cout << "-------Number of index blocks accessed--------" << endl;
        cout << "Index block accessed: " << nodeAccessed << endl;
        //in the following for loop, we search for the key if it exists
        for(int i = 0; i < cursor->size; i++)
        {
            if(cursor->key[i] == x)
            {
                //cout<<"Found\n";

                std::set<Block*> set = {cursor->ptr[i]};

                if (cursor->LLholder[i] != NULL) {
                    LL* linkedList = cursor->LLholder[i];
                    set.insert(linkedList->pointToBlock);


                    while (linkedList->nextLL!=NULL) {
                        linkedList = linkedList->nextLL;
                        set.insert(linkedList->pointToBlock);
                    }

                }

                std::set<Block*>::iterator itr;
                Block* blkContainingKeys;
                std::vector<Record> arrRecords;
                float avgRating;
                string tconst;
                int numVotes;
                float sum = 0;
                float count = 0;


                for (itr = set.begin();
                     itr != set.end(); itr++)
                {
                    blkContainingKeys = *(itr);
                    arrRecords = blkContainingKeys->retrieveRecords(x);

                    for (int j=0; j<arrRecords.size(); j++){
                        avgRating = arrRecords[j].retrieveAverageRating();
                        sum += avgRating;
                        count++;
                    }
                }

                float averageCalculation = sum/count;
                int dataCount=1;

                cout << "-------Number of data blocks accessed--------" << endl;
                cout << "number of data blocks = " << set.size() << endl;
                cout << "-------Content of data blocks accessed--------" << endl;
                for (itr = set.begin();
                     itr != set.end(); itr++)
                {
                    if (dataCount < 6) {
                        blkContainingKeys = *(itr);
                        arrRecords = blkContainingKeys->retrieveRecords(x);

                        cout << "Block " << dataCount << endl;

                        for (int j = 0; j < arrRecords.size(); j++) {
                            avgRating = arrRecords[j].retrieveAverageRating();
                            tconst = arrRecords[j].retrieveTCONST();
                            numVotes = arrRecords[j].retrieveNumVotes();

                            cout << avgRating << endl;
                            cout << tconst << endl;
                            cout << numVotes << endl;
                            cout<<"-------------------------------------\n";
                        }
                    }
                    else {
                        break;
                    }
                    dataCount++;
                }
                cout << "-------Average of averageRating--------" << endl;
                cout << "avgRating = " << averageCalculation << endl;

                return;
            }
        }
        cout<<"Not found\n";
    }
}

void BPTree::searchRange(int startInt, int endInt)
{
    // Can insert search function here
    //search logic
    if(root==NULL)
    {
        //empty
        cout<<"Tree empty\n";
    }
    else
    {
        Node* cursor = root;
        int nodeAccessed = 1;

        cout << "-------Content of index blocks accessed--------" << endl;
        while(cursor->isLeaf == false)
        {
            if  (nodeAccessed < 6) {
                cout << "Index block " << nodeAccessed << endl;
                for (int i = 0; i < cursor->size; i++) {
                    cout << cursor->key[i] << '|';
                }
                cout << "\n";
            }
            for(int i = 0; i < cursor->size; i++)
            {
                if(startInt < cursor->key[i])
                {
                    cursor = cursor->children[i];
                    nodeAccessed++;
                    break;
                }
                if(i == cursor->size - 1)
                {
                    cursor = cursor->children[i+1];
                    nodeAccessed++;
                    break;
                }
            }
        }
        if  (nodeAccessed < 6) {
            cout << "Index block " << nodeAccessed << endl;
            for (int i = 0; i < cursor->size; i++) {
                cout << cursor->key[i] << '|';
            }
            cout << "\n";
        }

        std::set<Block *> set;
        std::set<Block *> overallSet = {};
        std::set<Block *>::iterator itr;
        Block *blkContainingKeys;
        std::vector<Record> arrRecords;
        float avgRating;
        string tconst;
        int numVotes;
        float sum = 0;
        float count = 0;
        int dataCount = 1;

        // In this loop, check for the rightSib to see should enter there
        while (cursor->key[0] <= endInt) {
            //in the following for loop, we search for the key if it exists
            for (int i = 0; i < cursor->size; i++) {
                if (cursor->key[i] >= startInt && cursor->key[i] <= endInt) {

                    set = {cursor->ptr[i]};
                    overallSet.insert(cursor->ptr[i]);

                    if (cursor->LLholder[i] != NULL) {
                        LL *linkedList = cursor->LLholder[i];
                        set.insert(linkedList->pointToBlock);
                        overallSet.insert(linkedList->pointToBlock);


                        while (linkedList->nextLL != NULL) {
                            linkedList = linkedList->nextLL;
                            set.insert(linkedList->pointToBlock);
                            overallSet.insert(linkedList->pointToBlock);
                        }

                    }

                    for (itr = set.begin();
                         itr != set.end(); itr++) {
                        blkContainingKeys = *(itr);
                        arrRecords = blkContainingKeys->retrieveRecords(cursor->key[i]);

                        cout << "-------Content of data blocks accessed--------" << endl;
                        for (int j = 0; j < arrRecords.size(); j++) {
                            if (dataCount < 6) {
                                cout << "Block " << dataCount << endl;

                                avgRating = arrRecords[j].retrieveAverageRating();
                                tconst = arrRecords[j].retrieveTCONST();
                                numVotes = arrRecords[j].retrieveNumVotes();

                                cout << avgRating << endl;
                                cout << tconst << endl;
                                cout << numVotes << endl;
                                cout << "-------------------------------------\n";

                            }
                            dataCount++;
                            avgRating = arrRecords[j].retrieveAverageRating();
                            sum += avgRating;
                            count++;
                        }
                    }

                }
            }
            cursor = cursor->rightSib;
            nodeAccessed++;
            if  (nodeAccessed < 6) {
                cout << "Index block " << nodeAccessed << endl;
                for (int i = 0; i < cursor->size; i++) {
                    cout << cursor->key[i] << '|';
                }
                cout << "\n";
            }
        }


        float averageCalculation = sum / count;

        cout << "-------Number of index blocks accessed--------" << endl;
        cout << "Index block accessed: " << nodeAccessed << endl;
        cout << "-------Number of data blocks accessed--------" << endl;
        cout << "number of data blocks = " << overallSet.size() << endl;

        cout << "-------Average of averageRating--------" << endl;
        cout << "avgRating = " << averageCalculation << endl;
    }
}

int BPTree::getNumOfNodes()
{
    return numOfNodes;
}

int BPTree::getNumDeletion()
{
    return numOfDeletion;
}

void Node::getContent() {
    for(int i=0;i < size; i++)
    {
        cout << key[i] << '|';
    }
}

void BPTree::printNode(Node* cursor)
{
    cursor->getContent();
    return;
}

Node* BPTree::getFirstChild(Node* cursor)
{
    return cursor->children[0];
}

int BPTree::getHeight(Node* cursor)
{
    int height = 0;
    while (!cursor->isLeaf) {
        cursor = cursor->children[0];
        height++;
    }
    return height ;
}

void BPTree::insert(int x, Block* block)
{
    if (root == NULL)
    {
        root = new Node;
        root->key[0] = x;
        root->isLeaf = true;
        root->size = 1;
        root->ptr[0] = block;
        root->leftSib = NULL;
        root->rightSib = NULL;
        numOfNodes++;
        cout<<"Inserted "<<x<<"\n";
    }
    else
    {
        Node* cursor = root;
        Node* parent;

        //traverse to the leaf node where the key should be inserted into
        while (cursor->isLeaf == false)
        {
            parent = cursor;
            for(int i = 0; i < cursor->size; i++)
            {
                if(x < cursor->key[i])
                {
                    cursor = cursor->children[i];
                    break;
                }
                if(i == cursor->size - 1)
                {
                    cursor = cursor->children[i+1];
                    break;
                }
            }
        }

        for(int y = 0; y < cursor->size; y++)
        {
            if(cursor->key[y] == x)
            {
                cout<<"Key already exists. Will proceed to add to Linked List.\n";
                // Create a new LL with key and block address
                LL* newLL = new LL;
                newLL->key = x;
                newLL->pointToBlock = block;

                // Look into LLHolder and see if there are already LL inside
                // If no existing LL in LLHolder, add LL to LLHolder in that position
                if (cursor->LLholder[y] == NULL) {
                    cursor->LLholder[y] = newLL;
                }
                    // If there are LL already in the LLHolder, append LL to the end of existing LLs
                else {
                    LL* existingLL;
                    existingLL = cursor->LLholder[y];
                    while (true) {
                        if (existingLL->nextLL == NULL) {
                            existingLL->nextLL = newLL;
                            break;
                        }
                        existingLL = existingLL->nextLL;
                    }
                }

                return;
            }
        }

        if (cursor->size < MAX)
        {
            cursor->addKey(x, block);
        }

        else {
            cout<<"Inserted "<<x<<" successfully\n";
            int virNode[MAX+1];
            Block* virPtr[MAX+1];
            LL* virHolder[MAX+1];
            //store all the keys and records in a temporary list each
            for(int i = 0; i < MAX; i++)
            {
                virNode[i] = cursor->key[i];
                virPtr[i] = cursor->ptr[i];
                virHolder[i] = cursor->LLholder[i];
            }

            //make space
            int i, m=0;
            for(i = 0; i < MAX; i++)
            {
                if (x <= virNode[m]) {
                    for(int j = MAX+1;j > m; j--)
                    {
                        virNode[j] = virNode[j-1];
                        virPtr[j] = virPtr[j-1];
                        virHolder[j] = virHolder[j-1];
                    }
                    break;
                }
                m++;
            }
            // Insert  key
            virNode[m] = x;
            virPtr[m] = block;
            virHolder[m] = NULL;

            Node* newLeaf = new Node;
            newLeaf->isLeaf = true;
            numOfNodes++;

            cursor->size = (MAX+1)/2;
            newLeaf->size = MAX+1-cursor->size;

            //change the left and right siblings of all affected leaf nodes
            if (cursor->rightSib != NULL) {
                newLeaf->rightSib = cursor->rightSib;
                newLeaf->leftSib = cursor;
                cursor->rightSib->leftSib = newLeaf;
                cursor->rightSib = newLeaf;
            }
            else {
                newLeaf->rightSib = cursor->rightSib;
                newLeaf->leftSib = cursor;
                cursor->rightSib = newLeaf;
            }


            int j = cursor->size;
            for(i = 0; i < newLeaf->size; i++)
            {
                newLeaf->key[i] = virNode[j];
                newLeaf->ptr[i] = virPtr[j];
                newLeaf->LLholder[i] = virHolder[i];
                j++;
            }
            for(i = 0; i < cursor->size; i++)
            {
                cursor->key[i] = virNode[i];
                cursor->ptr[i] = virPtr[i];
                cursor->LLholder[i] = virHolder[i];
            }

            if (cursor == root)
            {
                //if cursor is a root node, we create a new root
                Node* newRoot = new Node;
                newRoot->isLeaf = false;
                newRoot->leftSib = NULL;
                newRoot->rightSib = NULL;
                newRoot->size = 1;
                newRoot->children[1] = newLeaf;
                newRoot->key[0] = newLeaf->key[0];
                newRoot->children[0] = cursor;
                root = newRoot;
                numOfNodes++;
            }

            else
            {
                insertInternal(parent, newLeaf, newLeaf->key[0]);
            }
        }
    }

}
void BPTree::insertInternal(Node* cursor, Node* child, int x)
{
    if(cursor->size < MAX)
    {

        int i, m=0;
        for(i = 0; i < cursor->size; i++)
        {
            if (x <= cursor->key[i]) {
                break;
            }
            m++;
        }

        for(int j = cursor->size+1; j > m+1; j--)
        {
            cursor->children[j] = cursor->children[j-1];
        }

        for(int j = cursor->size;j > m; j--)
        {
            cursor->key[j] = cursor->key[j-1];
        }
        cursor->size++;
        cursor->children[i+1] = child;
        cursor->key[i] = x;
        cout<<"Key inserted into internal node\n";
    }
        //if internal node has no empty space
    else
    {
        cout<<"Key inserted into internal node\n";

        //create virtual Internal Node;
        int virKey[MAX+1];
        Node* virChildren[MAX+2];
        for(int i = 0; i < MAX+1; i++)
        {
            virChildren[i] = cursor->children[i];
        }
        for(int i = 0; i < MAX; i++)
        {
            virKey[i] = cursor->key[i];
        }
        int i, m=0;
        for(i = 0; i < MAX; i++)
        {
            if (x <= virKey[i]) {
                break;
            }
            m++;
        }

        for(int j = MAX+2;j > m+1; j--)
        {
            virChildren[j] = virChildren[j-1];
        }

        for(int j = MAX+1;j > m; j--)
        {
            virKey[j] = virKey[j-1];
        }
        virChildren[m+1] = child;
        virKey[m] = x;


        Node* nodeInternal = new Node;
        numOfNodes++;
        //split cursor
        nodeInternal->size = MAX-(MAX+1)/2;
        cursor->size = (MAX+1)/2;
        nodeInternal->isLeaf = false;
        nodeInternal->leftSib = NULL;
        nodeInternal->rightSib = NULL;


        int j = cursor->size+1;
        for(i = 0; i < nodeInternal->size+1; i++)
        {
            nodeInternal->children[i] = virChildren[j];
            j++;
        }
        j = cursor->size+1;
        for(i = 0; i < nodeInternal->size; i++)
        {
            nodeInternal->key[i] = virKey[j];
            j++;
        }
        if(cursor == root)
        {

            Node* newRoot = new Node;
            newRoot->isLeaf = false;
            newRoot->leftSib = NULL;
            newRoot->rightSib = NULL;
            newRoot->children[0] = cursor;
            newRoot->children[1] = nodeInternal;
            newRoot->key[0] = cursor->key[cursor->size];
            newRoot->size = 1;
            root = newRoot;
            numOfNodes++;
            cout<<"Created new root\n";
        }
        else
        {
            Node* parent = findParent(root,cursor);
            insertInternal(parent ,nodeInternal, cursor->key[cursor->size]);
        }
    }
}
Node* BPTree::findParent(Node* cursor, Node* child)
{
    Node* parent;
    if(cursor->isLeaf || (cursor->children[0])->isLeaf)
    {
        return NULL;
    }
    for(int i = 0; i < cursor->size+1; i++)
    {
        if(cursor->children[i] == child)
        {
            parent = cursor;
            return parent;
        }
        else
        {
            parent = findParent(cursor->children[i],child);
            if(parent!=NULL) {
                return parent;
            }
        }
    }
    return parent;
}

void BPTree::remove(int x)
{
    // Check if root is null
    if(root==NULL)
    {
        cout<<"Empty tree.\n";
    }
    else
    {
        Node* cursor = root;
        Node* parent;
        int rightIndex,leftIndex;

        while(cursor->isLeaf == false)
        {
            for(int i = 0; i < cursor->size; i++)
            {
                parent = cursor;
                leftIndex = i-1; //leftIndex is the index of left sibling in the parent node
                rightIndex =  i+1; //rightIndex is the index of right sibling in the parent node
                if(x < cursor->key[i])
                {
                    cursor = cursor->children[i];
                    break;
                }
                if(i == cursor->size - 1)
                {
                    cursor = cursor->children[i+1];
                    rightIndex = i+2;
                    leftIndex = i;
                    break;
                }
            }
        }
        //in the following for loop, we search for the key if it exists
        bool found = false;
        int pos;
        for(pos = 0; pos < cursor->size; pos++)
        {
            if(cursor->key[pos] == x)
            {
                found = true;
                break;
            }
        }
        if(!found)//if key does not exist in that leaf node
        {
            cout<<"Not found\n";
            return;
        }
        //deleting the key
        for(int i = pos; i < cursor->size; i++)
        {
            cursor->key[i] = cursor->key[i+1];
            cursor->ptr[i] = cursor->ptr[i+1];
            cursor->LLholder[i] = cursor->LLholder[i+1];
        }
        cursor->size--;
        if(cursor == root)
        {
            cout<<"Removed "<<x<<" from the leaf node\n";

            for(int i = 0; i < MAX+1; i++)
            {
                cursor->children[i] = NULL;
            }

            cursor->leftSib = NULL;
            cursor->rightSib = NULL;
            if(cursor->size == 0)
            {
                cout<<"Tree died\n";
                delete[] cursor->key;
                delete[] cursor->children;
                delete[] cursor->ptr;
                delete[] cursor->LLholder;
                delete cursor;
                root = NULL;
                numOfNodes--;
                numOfDeletion++;
            }
            return;
        }

        cout<<"Removed "<<x<<" from leaf node \n";
        // Check if leaf is valid. IF valid return
        if(cursor->size >= (MAX+1)/2)//no underflow
        {
            return;
        }

        // Borrow: First, check the left sibling, then the right sibling
        // Borrow: Here, checking the left sibling to borrow
        //check if left sibling exists and is eligible for borrowing
        if(leftIndex >= 0 &&
           parent->children[leftIndex]->canLeafBorrow())
        {
            Node *sibling = parent->children[leftIndex];

            for(int i = cursor->size; i > 0; i--)
            {
                cursor->key[i] = cursor->key[i-1];
                cursor->ptr[i] = cursor->ptr[i-1];
                cursor->LLholder[i] = cursor->LLholder[i-1];
            }


            cursor->size++;


            int leftMax = sibling->key[sibling->size-1];
            cursor->key[0] = leftMax;
            cursor->LLholder[0] = sibling->LLholder[sibling->size-1];
            cursor->ptr[0] = sibling->ptr[sibling->size-1];


            sibling->size--;


            parent->key[leftIndex] = cursor->key[0];
            cout<<"Transferred "<<cursor->key[0]<<" from left sibling of leaf node\n";
            return;

        }
        //check if right sibling exists and is eligible for borrowing
        if(rightIndex <= parent->size &&
           parent->children[rightIndex]->canLeafBorrow())
        {

            Node *sibling = parent->children[rightIndex];


            cursor->size++;


            int rightMin = sibling->key[0];
            cursor->key[cursor->size-1] = rightMin;
            cursor->LLholder[cursor->size-1] = sibling->LLholder[0];
            cursor->ptr[cursor->size-1] = sibling->ptr[0];


            sibling->size--;


            for(int i = 0; i < sibling->size; i++)
            {
                sibling->key[i] = sibling->key[i+1];
                sibling->LLholder[i] = sibling->LLholder[i+1];
                sibling->ptr[i] = sibling->ptr[i+1];
            }


            parent->key[rightIndex-1] = sibling->key[0];
            cout<<"Transferred "<<cursor->key[cursor->size-1]<<" from right sibling of leaf node\n";
            return;

        }

        // First merge with left sibling and delete a node
        if(leftIndex >= 0)//if left sibling exist
        {
            Node* sibling = parent->children[leftIndex];

            int j = sibling->size;
            for(int i = 0; i < cursor->size; i++)
            {
                sibling->key[j] = cursor->key[i];
                sibling->LLholder[j] = cursor->LLholder[i];
                sibling->ptr[j] = cursor->ptr[i];
                j++;
            }


            if (cursor->rightSib != NULL) {
                sibling->rightSib = cursor->rightSib;
                cursor->rightSib->leftSib = sibling;
                cursor->leftSib = NULL;
                cursor->rightSib = NULL;
            }
            else {
                sibling->rightSib = cursor->rightSib;
                cursor->leftSib = NULL;
                cursor->rightSib = NULL;
            }


            int increaseSize = cursor->size;
            sibling->size += increaseSize;


            numOfNodes--;
            numOfDeletion++;
            removeInternal(cursor, parent, parent->key[leftIndex]);

        }
        else if(rightIndex <= parent->size)
        {
            Node* sibling = parent->children[rightIndex];

            for(int i = cursor->size, j = 0; j < sibling->size; i++, j++)
            {
                cursor->key[i] = sibling->key[j];
                cursor->LLholder[i] = sibling->LLholder[j];
                cursor->ptr[i] = sibling->ptr[j];
            }


            cursor->size += sibling->size;


            if (sibling->rightSib != NULL) {
                cursor->rightSib = sibling->rightSib;
                sibling->rightSib->leftSib = cursor;
                sibling->leftSib = NULL;
                sibling->rightSib = NULL;
            }
            else {
                cursor->rightSib = sibling->rightSib;
                sibling->leftSib = NULL;
                sibling->rightSib = NULL;
            }


            numOfNodes--;
            numOfDeletion++;
            removeInternal(sibling, parent, parent->key[rightIndex-1]);// delete parent node key

        }
    }
}
bool Node::canLeafBorrow() {
    return size >= (MAX+1)/2+1;
}

bool Node::canInternalBorrow() {
    return size >= (MAX+1)/2;
}


bool Node::addKey(int x, Block* block) // std::shared_ptr<Block> block)
{

    if (x >= key[size-1]) {
        key[size] = x;
        ptr[size] = block;
        LLholder[size] = NULL;
        size++;
        cout<<"Inserted "<<x<<" successfully\n";
    }

    else {
        int i, m=0;
        for(i=0;i < size; i++)
        {
            if (x <= key[i]) {
                break;
            }
            m++;
        }

        for(int j = size;j > m; j--)
        {
            key[j] = key[j-1];
            ptr[j] = ptr[j-1];
            LLholder[j] = LLholder[j-1];
        }
        key[m] = x;
        ptr[m] = block;
        LLholder[m] = NULL;
        size++;
        cout<<"Inserted "<<x<<" successfully\n";
    }

    return true;
}

void Node::delKeyInternal(Node* child, int x) {
    int pos;
    //Deleting the pointer child
    for(int j = 0; j < size+1; j++)
    {
        if(children[j] == child)
        {
            pos = j;
        }
    }
    for(int i = pos; i < size+1; i++)
    {
        children[i] = children[i+1];
    }
    for(int j = 0; j < size; j++)
    {
        if(key[j] == x)
        {
            pos = j;
        }
    }
    for(int i = pos; i < size; i++)
    {
        key[i] = key[i+1];
    }
    size--;
};

void BPTree::removeInternal(Node* child, Node* cursor, int x)
{

    if(cursor == root)
    {
        if(cursor->size == 1)
        {

            if(cursor->children[1] == child)
            {
                delete[] child->key;
                delete[] child->children;
                delete[] child->ptr;
                delete[] child->LLholder;
                delete child;
                numOfNodes--;
                numOfDeletion++;
                root = cursor->children[0];
                root->leftSib = NULL;
                root->rightSib = NULL;
                delete[] cursor->key;
                delete[] cursor->children;
                delete[] cursor->ptr;
                delete[] cursor->LLholder;
                delete cursor;
                numOfNodes--;
                numOfDeletion++;
                cout<<"Changed root node\n";
                return;
            }

            else if(cursor->children[0] == child)
            {
                delete[] child->key;
                delete[] child->children;
                delete[] child->ptr;
                delete[] child->LLholder;
                delete child;
                numOfNodes--;
                numOfDeletion++;
                root = cursor->children[1];
                root->leftSib = NULL;
                root->rightSib = NULL;
                delete[] cursor->key;
                delete[] cursor->children;
                delete[] cursor->ptr;
                delete[] cursor->LLholder;
                delete cursor;
                numOfNodes--;
                numOfDeletion++;
                cout<<"Changed root node\n";
                return;
            }
        }
    }

    int pos;


    cursor->delKeyInternal(child, x);

    if(cursor->size >= (MAX+1)/2-1)
    {
        cout<<"Deleted "<<x<<" from internal node\n";
        return;
    }

    if( cursor==root ) {
        return;
    }
    Node* parent = findParent(root, cursor);
    int leftIndex, rightIndex;

    for(pos = 0; pos < parent->size+1; pos++)
    {
        if(parent->children[pos] == cursor)
        {
            leftIndex = pos - 1;
            rightIndex = pos + 1;
            break;
        }
    }
    //try to transfer
    if(leftIndex >= 0 &&
       parent->children[leftIndex]->canInternalBorrow())
    {
        Node *sibling = parent->children[leftIndex];


        for (int i = cursor->size+1; i > 0; i--)
        {
            cursor->children[i] = cursor->children[i-1];
        }
        for(int i = cursor->size; i > 0; i--)
        {
            cursor->key[i] = cursor->key[i-1];
        }

        cursor->children[0] = sibling->children[sibling->size];

        cursor->key[0] = parent->key[leftIndex];
        parent->key[leftIndex] = sibling->key[sibling->size-1];


        sibling->size--;
        cursor->size++;
        return;

    }
    if(rightIndex <= parent->size &&
       parent->children[rightIndex]->canInternalBorrow())
    {
        Node *sibling = parent->children[rightIndex];

        cursor->children[cursor->size+1] = sibling->children[0];
        for (int i = 0; i < sibling->size; ++i)
        {
            sibling->children[i] = sibling->children[i+1];
        }

        cursor->key[cursor->size] = parent->key[pos];
        parent->key[pos] = sibling->key[0];
        for (int i = 0; i < sibling->size -1; i++)
        {
            sibling->key[i] = sibling->key[i+1];
        }
        sibling->size--;
        cursor->size++;
        return;

    }

    if(leftIndex >= 0)
    {

        Node *sibling = parent->children[leftIndex];
        sibling->key[sibling->size] = parent->key[leftIndex];
        int i = sibling->size+1;
        for(int j = 0; j < cursor->size+1; j++)
        {
            sibling->children[i] = cursor->children[j];
            cursor->children[j] = NULL;
        }
        for(int j = 0; j < cursor->size; j++)
        {
            sibling->key[i] = cursor->key[j];
        }
        sibling->size += cursor->size+1;
        cursor->size = 0;

        numOfNodes--;
        numOfDeletion++;
        removeInternal(cursor, parent, parent->key[leftIndex]);


    }
    else if(rightIndex <= parent->size)
    {

        Node *sibling = parent->children[rightIndex];
        cursor->key[cursor->size] = parent->key[rightIndex-1];
        int i = cursor->size+1;
        for(int j = 0; j < sibling->size+1; j++)
        {
            cursor->children[i] = sibling->children[j];
            sibling->children[j] = NULL;
        }
        for(int j = 0; j < sibling->size; j++)
        {
            cursor->key[i] = sibling->key[j];
        }
        cursor->size += sibling->size+1;
        sibling->size = 0;
        //delete cursor
        numOfNodes--;
        numOfDeletion++;
        removeInternal(sibling, parent, parent->key[rightIndex-1]);

    }
}

Node* BPTree::getRoot()
{
    return root;
}

int main(int argc, char** argv) {

    // get blockSize from user
    int BlockSize;
    cout << "Enter Block Size (in bytes)" << endl;
    cin >> BlockSize;
    cout << "Block Size " << BlockSize << " bytes\n" << endl;

    BPTree bpt;
    string command;
    string line;
    std::vector<std::string> fields;
    bool skipline = true;
    int i = 0, j = 0;
    int blocksUsed = 1;
    int numIO = 1;
    int numRecords = -1; //to skip first line

    // -------------------------------------
    // do modify accordingly
    string DATAFILE = "../data/data.tsv";
    // string DATAFILE = "../data/dummy.tsv";
    // -------------------------------------

    // read file once to get numRecords
    ifstream fin1(DATAFILE);
    while (getline(fin1, line)) {
        numRecords++;
    }
    fin1.close();
    cout << "numRecords = " << numRecords << endl;

    int n;
    if (BlockSize == 200) {
        n = 6;
    }
    else {
        n = 15;
    }
    cout << "n = " << n << endl;
    MAX = n;

    int numNodes = ceil(numRecords/n);
    cout << "num B+ leaf nodes = " << numNodes << endl;

    //for debug
    int heightOfTree = ceil(log2(numNodes));
    cout << "height/degree of B+ Tree = " << heightOfTree << endl;

    // initialise new Disk struc for storing blocks/records etc
    Disk Disk;
    // initialise new Block, with block size
    Block newBlock(BlockSize);
    // init first shared_ptr for BlockVector
    std::shared_ptr<Block> newBlockPtr = std::make_shared<Block>(std::move(newBlock));
    // add newBlock to array of Block pointers
    Disk.addBlock(newBlockPtr);
    cout << "Building Disk Struc..." << endl;

    Block blkToBeInserted = newBlock;

    ifstream fin(DATAFILE);
    cout << "Reading File......" << endl;

    while (getline(fin, line)) {
        // Skips the first line of headers which are titles
        if (skipline){
            skipline = false;
            continue;
        }

        // Split line into tab-separated parts
        std::istringstream iss(line);
        std::string afield;

        // Reads each line of tsv as a string
        // And splits each strings to smaller strings
        // Appends to the fields[] array
        while(std::getline(iss, afield, '\t')){
            fields.push_back(afield);
        }

        Record newRecord;
        // Add fields into Record
        newRecord.tconst = fields[i++];
        std::istringstream(fields[i++]) >> newRecord.averageRating;
        std::istringstream(fields[i++]) >> newRecord.numVotes;
        std::shared_ptr<Block> blockPointer;

        if (Disk.blockVector[blocksUsed-1]->numRecordsInBlock() < Disk.blockVector[blocksUsed-1]->maximumRecords){
            Disk.blockVector[blocksUsed-1]->insertRecord(newRecord);
            blockPointer = Disk.blockVector[blocksUsed-1];

            cout << "New record inserted into block with ID " << newRecord.retrieveTCONST() << endl;

            // insert record into the b+ tree with numVotes as key
            // bpt.insert(newRecord.retrieveNumVotes(), &blkToBeInserted);
            Block* blockToInsert = blockPointer.get();
            bpt.insert(newRecord.retrieveNumVotes(), blockToInsert);
            numIO++;
        }

        else{
            // after maxing out one block, need retrieve another block
            cout << "Block Full, initialising new Block..." << endl;
            numIO++;

            //initialise new block and add to pointerVector
            Block additionalBlock(BlockSize);

            additionalBlock.insertRecord(newRecord);
            cout << "New record inserted into block with ID " << newRecord.retrieveTCONST() << endl;
            blockPointer = std::make_shared<Block>(additionalBlock);
            Disk.addBlock(blockPointer);

            Block* blockToInsert = blockPointer.get();
            bpt.insert(newRecord.retrieveNumVotes(), blockToInsert);
            numIO++;

            blocksUsed++;
        }
    } //exit while loop

    // close the .tsv file as all data has been read already
    fin.close();

    cout << "--- All Data Stored in Disk ---" << endl;
    cout << " " << endl;

    // start of Experiments
    cout << "---Experiment 1: Storing Data on Disk---" << endl;
    cout << "Storage Stats:" << endl;
    cout << "Number of Blocks Used: " << blocksUsed << endl;
    cout << "Number of I/Os Used: " << numIO << endl;
    cout << "Number of Records: " << Disk.getNumOfRecords() << endl;
    cout << "Size of 1 Record: " << sizeof(Record) << endl;

    int totalSize = Disk.getSizeOfStorage(); //to add on B+ tree size
    totalSize += bpt.getNumOfNodes() * sizeof(Node);

    cout << "Size of Database: " << totalSize << " Bytes" << endl;
    cout << " " << endl;
    cout<<"\n";
    cout<<"\n";
    cout<<"\n";

    cout << "---Experiment 2: Building B+ tree using insertion---" << endl;
    cout << "Parameter n of B+ tree: " << MAX << endl;
    cout << "Number of Nodes in BP Tree: " << bpt.getNumOfNodes() << endl;
    cout << "Height of BP tree: " << bpt.getHeight(bpt.getRoot()) << endl;
    cout << "Content of Root Node: " << endl;
    bpt.printNode(bpt.getRoot());
    cout << " " << endl;

    cout << "Content of first child: " << endl;
    bpt.printNode(bpt.getFirstChild(bpt.getRoot()));
    cout << " " << endl;
    cout<<"\n";
    cout<<"\n";
    cout<<"\n";

    cout << "---Experiment 3: Retrieve movies with numVotes = 500---" << endl;
    bpt.search(500);
    cout<<"\n";
    cout<<"\n";
    cout<<"\n";


    cout << "---Experiment 4: INSERT HERE---" << endl;
    bpt.searchRange(30000, 40000);
    cout<<"\n";
    cout<<"\n";
    cout<<"\n";

    cout << "---Experiment 5: Delete movies given numVotes---" << endl;
    // Remove numVote = 1000
    int nodeBeforeRemove = bpt.getNumOfNodes();
    int heightBeforeRemove = bpt.getHeight(bpt.getRoot());
    bpt.remove(1000);

    cout << "Number of Nodes in BP Tree before removal: " << nodeBeforeRemove << endl;
    cout << "Number of Nodes in BP Tree after removal: " << bpt.getNumOfNodes() << endl;
    cout << "Number of Nodes removed: " << bpt.getNumDeletion() << endl;
    cout << "Height of BP tree before removal: " << heightBeforeRemove << endl;
    cout << "Height of BP tree after removal: " << bpt.getHeight(bpt.getRoot()) << endl;
    cout << "Content of root node after removal: " << endl;
    bpt.printNode(bpt.getRoot());
    cout << " " << endl;
    cout<<"\n";
    cout << "Content of first child: " << endl;
    bpt.printNode(bpt.getFirstChild(bpt.getRoot()));
    cout << " " << endl;
    cout<<"\n";
    cout<<"\n";
    cout<<"\n";


    return 0;
}