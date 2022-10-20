# Maximal-frequent-itemset-mining
Given a transaction database and a minimum support count(minsup), if the itemset's support in the transaction database is >= minsup, and none of its immediate supersets are frequent, which means its length is maximal(**not maximum**), then the itemset is so-called the maximal frequent itemset
# Example
![image](https://user-images.githubusercontent.com/64155027/196638346-d0b30af6-a8b9-47a7-ac8a-50766b3f3c90.png)  
with minsup = 1, maximal frequent itemset = ABCDE  
with minsup = 2, maximal frequent itemset = ABDE, BCDE  
with minsup = 4, maximal frequent itemset = C, BDE
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
