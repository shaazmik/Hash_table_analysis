# Hash_table_analysis

## Introduction
In this project I have studied how a __hash-table__ based on the __chain method__ works. 
The main point of study is certain hash functions and programme performance depending on hash functions, table sizes and various loads.
- Input data: L.N. Tolstoy's text 'War and Peace' (eng. version)
- Stress Testing: Search for each word in the list 5 times (element of Hash Table)
- Hash table capacity : **6900**
- List capacity: **200**

## Task 1: Learning hash functions

I had 6 hash functions to consider:
- always return 1
- value of the first ASCII character
- string length
- sum of the ASCII characters in word
- ROL
- CRC32

First of all, we need to determine the number of collisions depending on our hash functions, 
then take the speed of the programme and the reasons for their changes.


### 1. __hash_return_one__
![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/return1.png)
