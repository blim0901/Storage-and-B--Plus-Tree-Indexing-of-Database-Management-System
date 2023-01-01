# Storage-and-B--Plus-Tree-Indexing-of-Database-Management-System
The objective of this project is to implement following two components of a database management system, storage and indexing. For the storage component, a fraction of main memory is allocated to be used as disk storage. The disk storage is organised and accessed with a block as a unit. For the indexing component, a B+ tree is used and each node is stored in the memory region bounded by the block size. Some functions were also created to build the B+ tree, conduct search using the B+ tree, add to the B+ tree and deletion using B+ tree. 


## Install Dependencies and Build Project on Windows

### Install Dependencies
1. Install  MinGW-x64 from MSYS2
2. Add MinGW to Windows PATH environment by following the steps:
- Open Windows Settings
- Search ‘Edit the system environment variables’
- From the ‘Advanced’ tab, click on ‘Environment Variables’
- Select ‘Path’ from the system variables and click ‘Edit’
- Click on ‘New’ and add ‘C:\msys64\mingw64\bin’ to the Path
- Click ‘OK’ to save the Path


### Run Project
1. Ensure you have the above dependencies installed
2. Check to make sure that the data.tsv file is being read and loaded:
  -  Go to line 1213 in main.cpp to ensure the data being loaded is "../data/data.tsv", not "../data/dummy.tsv"
3. Go into project's src directory:
```
cd CZ4031-DB-Project
cd src
```
4. Run the a.exe file in command prompt: <br>
```
a.exe
```
5. Alternatively, to download and run the project from scratch, clone Github Repository or download the project as zip file
- Open command prompt on windows
- Go into Project directory: <br>
```
cd CZ4031-DB-Project
```
- Go into src directory: <br>
```
cd src
```
- Compile the source code: <br>
```
g++ main.cpp block.cpp record.cpp disk.cpp 
```
- Execute the program: <br>
```
a.exe
```
6. After executing a.exe, when prompted to enter Block size, input either 200 or 500 to start loading the data for the experiments. 
7. After that, the experiments 1-5 will then execute by itself
