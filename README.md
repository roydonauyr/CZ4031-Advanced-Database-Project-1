# CZ4031-Advanced-Database

![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/roydonauyr/CZ4031-Advanced-Database-Project-1)
![GitHub repo size](https://img.shields.io/github/repo-size/roydonauyr/CZ4031-Advanced-Database-Project-1)
![GitHub language count](https://img.shields.io/github/languages/count/roydonauyr/CZ4031-Advanced-Database-Project-1)
![GitHub last commit](https://img.shields.io/github/last-commit/roydonauyr/CZ4031-Advanced-Database-Project-1)

The aim of this project is to implement a B+ Tree in C++ which supports searching (both search
and range queries), insertion and deletion operations.

Our implementation consists of the following classes:
1. BlockManager: Manages the creation and deletion of new blocks. Handles all the logic
related to blocks.
2. Record class: Holds the fields of a record.
3. B+ Tree: Handles the logic of search, insertion and deletion operations of nodes in the
tree.
4. Block classes: Consists of B+ tree nodes, recordBlocks and linkedList Blocks

More information can be found in our pdf report.

B+ Tree Structure:
![image](https://user-images.githubusercontent.com/44868878/224474247-565e913a-e8de-4225-8680-eea90997ac26.png)

### Functions:
We have 4 vital functions to help us perform the experiments, namely Insert(), Delete(), searchKeys(),
searchRangeOfKeys(). 
Details can be found in the pdf report

### Requirements and how to fun
To install, run a C++ compiler with minimum C++11 support to compile main.cpp, then run the produced binary. Assuming g++, run the following command: g++ main.cpp -O2 -o main

To run: .\main.exe

In the case that some middle output disappears in the terminal (Happened sometimes in vsc terminal, could be due to too many information to print out), do output it out into a text file. That way the full output always comes out correctly. (Use .\main.exe > someTxtFile.txt)
