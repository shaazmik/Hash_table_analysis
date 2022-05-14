# Hash_table_analysis

## Table of contents

[1.Introduction](#Introduction)

[2.Task 1](#Task_1)

[3.Task 2](#Task_2)

## ___Introduction___
In this project I have studied how a __hash-table__ based on the __chain method__ works. <br/>
The main point of study is certain hash functions and programme performance depending on hash functions, table sizes and various loads. <br/>
- Input data: L.N. Tolstoy's text 'War and Peace' (eng. version)
- Stress Testing: Search for each word in the list 5 times (element of Hash Table)
- Hash table capacity : **6900**
- List capacity: **200**

### To compile:
  ___Main version with optimization___:
  >nasm -f elf64 crc32_optimization.s -o crc32_optimization.o
 
  >g++ crc32_optimization.o main.cpp List/List_functions.cpp phash_table_func.cpp phash_func.cpp -mavx2 -O2 -o name_file

  ___Version without optimization___:
>g++ main.cpp ../List/List_functions.cpp phash_table_func.cpp phash_func.cpp -o name_file

Also you can compile with flag ```-O2``` for optimization.

____
____

## ___Task_1___ 
## ___Learning hash functions___

I had 6 hash functions to consider:
- always return 1
- value of the first ASCII character
- string length
- sum of the ASCII characters in word
- ROL
- CRC32

First of all, we need to determine the number of collisions depending on our hash functions, <br/>
then take the speed of the programme and the reasons for their changes.For clarity, here are the charts for each hash function, <br/>
which show **the distribution of list lengths depending on the hash table element**: <br/>


### 1. ___hash always returns 1___

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/return1.png)


### Opinion:
The running time of the programme with this hash function is ___6,287 seconds___. <br/>
This is really a lot, most likely due to the sheer number of collisions. <br/>
The large number of collisions caused our list to expand forever (*realloc*).  <br/>
But the biggest burden is checking the words in the list, <br/>
since the length of the list is huge, so the check takes a very long time. <br/>

### 2. ___hash returns value of the first ASCII character___

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/first%20ASCII%20symbol.png)

### Opinion:
The running time of a programme with this hash function is ___0,673 seconds___. The reasons for the load and the speed of the programme are the same as in the first case. However, there are fewer collisions, so the speed is higher.

### 3. ___hash returns length of string___

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/length%20of%20string.png)

### Opinion:
Obviously, word lengths are limited, so there are plenty of collisions. The running time of the programme: ___1,725 seconds___

### 4. ___hash returns sum of ASCII characters___

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/sum%20of%20ASCII%20symbols.png)

### Opinion:
This hash function gives the least amount of collisions, so it works faster than the previous ones. Running time: ___0,099 seconds___. <br/>
Interesting deviations can be seen in the graph. I have checked it and indeed these values correspond to reality. <br/>
Our test contains words consisting of more than 32 characters, and the ASCII value of a character is really that big **(more than 255)** <br/>


### 5. ___ROL hash function___

This feature is more interesting than the ones before it. <br/>
To understand how this hash function works, here is a **screenshot of its implementation**. <br/>

```cpp
size_t hash_rol(char* str)
{
    assert (str != nullptr);

    int hash = *str;
    str++;

    while (*str != '\0')
    {
        hash = (hash << 1) ^ *str;

        str++;
    }

    return hash;
}
```
![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/ROL.png)
### Opinion:
This function gives a very small number of collisions. It works very quickly, <br/>
running time: ___0,96 seconds___ <br/>
There are so few matches that the list does not have to expand and the search is very fast,  <br/>
this hash function can be called "good" in our situation. <br/>


### 6. ___CRC32 hash function___

Finally, CRC32 is the best hash function on out list. <br/>

***Implementation***:
```cpp
size_t hash_CRC32(char* str)
{
    assert(str != nullptr);

    unsigned int crc = 0xFFFFFFFFUL;

    while (*str != '\0')
    {
        crc = CRCTable[(crc ^ *(str)) & 0xFF] ^ (crc >> 8);
        str++;
    }

    return crc;
}
```
![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/hash_statistics/CRC32.png)

### Opinion:
This hash function is very interesting, it gives the least amount of collisions, which will result in a big performance gain on more data. <br/>
Especially when **the stress testing** is to find items in a list. However, in our case the program performs slightly less than the ROL hash function. <br/>
This is due to the timing of the hash function itself. <br/>
Running time: ___0,97 seconds___. <br/>
**ROL** hash function works faster than **CRC32**. <br/>
But I will work with CRC32 as in our case lesser number of matches is much more useful than the hash algorithm itself. <br/>

## ___Conclusion for the Task 1:___
|**HASH FUNCTION**|**TIME**|
|-------------|-------------|
|return 1     |6,287 seconds|
|first ASCII  |0,673 seconds|
|length of str|1,725 seconds|
|sum of ASCII |0,099 seconds|
|ROL          |0,063 seconds|
|CRC32        |0,064 seconds|


As you can see, the program with ROL is slightly faster, but the variance of ROL is greater than that of CRC32.<br/>
If we increase the amount of input data,<br/> 
the performance of the program with CRC32 will be much better, <br/>
because the cost of finding an element in the list is much higher than the hash calculation. <br/>
And CRC32 has obviously less elements in one list <br/>
It is possible to optimise the hash function and make it faster than ROL .Next we will do all the work with the CRC32 function
<br/>
### ___Obviously the running time of the hash table is directly related to the hash function and the size of the table and list___
____
____
## ___Task_2___ 
## ___Optimization___

### **Run the original programme via the vallgrind tool - callgrind**

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/analytics/screenshots/NO_OPTIMIZATION.PNG)

Based on this result (the self column), we can see that the function with the highest load is **"phash_table_find_el"**.<br/>
And the **hash function** itself accordingly

### For clarity, the results of compiler optimisation with the ```-O2``` flag are as follows

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/analytics/screenshots/O2.PNG)
____
____
### The original ```phash_table_find_el``` version 

```cpp
struct Plist* phash_table_find_el(Phash_table* hash_table, char* word, int* item_num_of_list)
{
    size_t offset = hash_table->hash_func(word) % hash_table->capacity;

    int flag = 0;

    for (int i = 1; (hash_table->hash_list[offset].size >= i) && (flag == 0) ; i++)
    {
        if (strncmp(word, hash_table->hash_list[offset].data[i].value, strlen(word) + 1) == 0)
        {
            flag = i;
        }
    }

    *item_num_of_list = flag;

    return hash_table->hash_list + offset;
}
```
____
### I used optimization using AVX2 to compare strings

```cpp
struct Plist* phash_table_find_el(Phash_table* hash_table, char* word, int* item_num_of_list)
{
    size_t offset = hash_table->hash_func(word) % hash_table->capacity;

    __m128i string = _mm_loadu_si128 ((__m128i *)word);
    __m128i listStr {};

    int flag = 0;

    for (int i = 1; (hash_table->hash_list[offset].size >= i) && (flag == 0) ; i++)
    {
        listStr = _mm_loadu_si128 ((__m128i *)hash_table->hash_list[offset].data[i].value);

        int cmp = _mm_cmpestri (string, strlen (word) + 1, listStr, hash_table->hash_list[offset].data[i].len_str + 1, _SIDD_CMP_EQUAL_EACH | _SIDD_CMP_EQUAL_ORDERED | _SIDD_UBYTE_OPS); // | _SIDD_NEGATIVE_POLARITY);

        if (cmp == 0) 
            flag = i;
    }

    *item_num_of_list = flag;

    return hash_table->hash_list + offset;
}
```
____

### This optimization gave a relatively good increase in the performance of the program, below is a table with time values and callgrind results.
### I use flag ```-O2``` because AVX2 doesn't work correctly without this flag.

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/analytics/screenshots/INTRINSICS_O2.PNG)

As you can see, our function has accelerated by **3 million** units self. which is an excellent result, <br\>
but if we talk about more down to earth values, here is a table of the running times. You can find all of the proufs in the ```./analytics/screenshots/``` folder

|WITHOUT OPTIMIZATION|0,064 seconds|
|--------------------|-------------|
|WITH -O2            |0,043 seconds|
|WITH -O2 + AVX2     |0,038 seconds|

### Total increase of 13,16%
### Now let's rewrite our hash function into assembler
### Original version ```hash_CRC32```

```cpp
size_t hash_CRC32(char* str)
{
    assert(str != nullptr);

    unsigned int crc = 0xFFFFFFFFUL;

    while (*str != '\0')
    {
        crc = CRCTable[(crc ^ *(str)) & 0xFF] ^ (crc >> 8);
        str++;
    }

    return crc;
}
```
### After optimization:

```asm
section .text

global crc32_asm


crc32_asm:
    mov eax, 0FFFFFFFFh

.loop:
    cmp byte [rdi], 00h
    je .exit

    movzx r10d, byte [rdi]

    crc32 eax, r10b

    inc rdi
    jmp .loop

.exit:
    not eax
    ret
```

![Image alt](https://github.com/shaazmik/Hash_table_analysis/blob/main/analytics/screenshots/O2_INTR_ASM.PNG)

### We got an increase of 10.2 million units .self

As for the other functions, we may notice a large value in ```strlen```, ```fill_words_text```, ```plist_constructor```, <br/>
we can omit them because these functions are only used once when filling the table. <br/>
There is no repetition of them, so there will be no improvement in optimization when the table is filled permanently <br/>
This optimisation is therefore sufficient to improve performance. The results of the stress test run times are shown below.<br/>
**Finally, reduce the number of function calls and various cosmetic changes** <br/>

### Total:

|WITHOUT OPTIMIZATION |0,064 seconds|
|---------------------|-------------|
|WITH -O2             |0,043 seconds|
|WITH -O2 + AVX2      |0,038 seconds|
|WITH -O2 + AVX2 + ASM|0,036 seconds|

#### All prufs can be checked in ```./analytics/screenshots/```


