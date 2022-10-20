# Maximal-frequent-itemset-mining
Given a transaction database and a minimum support count(minsup), if the itemset's support in the transaction database is >= minsup, and none of its immediate supersets are frequent, which means its length is maximal(**not maximum**), then the itemset is so-called the maximal frequent itemset.
# Example
![image](https://user-images.githubusercontent.com/64155027/196638346-d0b30af6-a8b9-47a7-ac8a-50766b3f3c90.png)  
with minsup = 1, maximal frequent itemset = ABCDE  
with minsup = 2, maximal frequent itemset = ABDE, BCDE  
with minsup = 4, maximal frequent itemset = C, BDE
# Introduction
Clm-miner is a graph-based maximal frequent itemset mining method, it represents database as a graph and store the graph to the completely linked matrix(clm) which cost O(Nx((NxN)+N)) space complexity, then clm-miner find all maximal frequent itemset from clm. The method cost a large space complexity in exchange of mining in constant time.  
Clm exists a lot of zero that is totally useless for our mining algorithm, to reduce the space, there is another version call clm-miner-sparse. It only store the non-zero value to clm. Although it still cost N^3 space complexity, it greatly reduce the matrix space.  
# Dataset information
connect  ----> 129 items, 67557 transactions  
accident ----> 468 items, 340183 transactions
# Compiler
```
g++ .\clm_miner.cpp
```
OR
```
g++ .\clm_miner_sparse.cpp
```
THEN
```
.\a.exe <dataset> <dataset's total item count>
```
