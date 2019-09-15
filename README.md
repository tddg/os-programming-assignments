# Building a multi-threaded in-memory key-value store

In this project, you'll be building a simplified in-memory key-value store (KVS). While somewhat easier to implement a classic chained hash table, there are still numerous challenges, mostly in building the correct concurrency support. Thus, you'll have to think a bit about how to build the KVS implementation, and then build it to work efficiently and correctly.

There are three specific objectives to this assignment:

- To learn about the general nature of an in-memory KVS.
- To implement a correct and efficient in-memory KVS using threads and related functions.
- To gain more experience writing concurrent code.

## Background

To understand how to make progress on any project that involves concurrency, you should understand the basics of thread creation, mutual exclusion (with locks), and signaling/waiting (with condition variables). These are described in the following book chapters:

- [Intro to Threads](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-intro.pdf)
- [Threads API](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-api.pdf)
- [Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks.pdf)
- [Using Locks](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-locks-usage.pdf)
- [Condition Variables](http://pages.cs.wisc.edu/~remzi/OSTEP/threads-cv.pdf)

Read these chapters carefully in order to prepare yourself for this project.

## General Idea

Let's now get into the exact code you'll have to build. The in-memory KVS infrastructure you will build supports the execution of user-defined `ht_init()`, `ht_insert()`, and `ht_lookup()` functions.

`ht_init()` takes an input `unsigned int` integer as the size of the hash table data structure and initializes a chained hash table with a total number of  `num_buckets` buckets.

`ht_insert()`, also written by the user, takes an input key-value pair and inserts it to the end of the bucket chain mapped by `key`.  

The `ht_lookup()` function, also written by the user, performs a hashed lookup over the buckets with provided `key`, and set the second parameter `char **val` once the matched `key` is found.

What's quite interesting is how easy it is to parallelize: many worker threads can be running at the same time. Users observe speedup when scaling up the in-memory KVS by adding more worker threads to the program.

## Code Overview

We give you here the
[`imkv.h`](https://github.com/tddg/os-programming-assignments/imkv.h)
header file that specifies exactly what you must build in your in-memory KVS library:

```
#ifndef __imkv_h__
#define __imkv_h__

// err code
typedef enum {
	HT_INIT_OK,
	HT_INIT_FAILED,
	HT_INSERT_OK,
	HT_INSERT_FAILED,
	HT_FOUND,
	HT_NOTFOUND,
	HT_DELETE_OK,
	HT_DELETE_FAILED,
} HT_RET;

/* External functions: these are what you must define */

// Initialize the data structure:
// num_buckets specifies the maximum number of buckets in the bucket hash table
HT_RET ht_init(unsigned int num_buckets);

// Tries to insert a new key-value pair
HT_RET ht_insert(char *key, char *val);

// Tries to lookup a value given a key; found val should set the 2nd parameter
HT_RET ht_lookup(char *key, char **val);

// Tries to delete a key-value pair specified by the parameter
HT_RET ht_delete(char *key);

/* End of API declaration */

#endif // __imkv_h__
```

One basic assumption is that, when a user is writing his/her own application with your in-memory KVS library, they will create multiple threads that concurrently access (with both the read and write operations) your KVS implemented in the library. Therefore, it is necessary for your library to use pthread locks and/or CVs to provide mutually exclusive access for concurrent `ht_insert()` and `ht_lookup()` operations. More on this below.

## Considerations

Here are a few things to consider in your implementation:

- **Step 1: Supporting Concurrency**. This part is fairly straightforward. Your central hash table data structure should be concurrent (and thread-safe), allowing multiple threads to read and write in parallel, correctly and efficiently. You should think about what type of locking is needed throughout this process for correctness.

- **Step 2: Slab Memory Management**. Another concern is memory management. The `ht_insert()` function is passed a key-value pair; it is the responsibility of the in-memory KVS library to provide efficient slab memory management. That is, when the function `ht_init()` is called, a large memory chunk is allocated by calling `malloc()`; and all subsequent `ht_insert()` and `ht_delete()` calls would interact with this pre-allocated memory chunk. When the `ht_insert()` function is called, it is the responsibility of the in-memory KVS library to retrieve a piece of memory out from the large memory chunk for storing the copy of the key-value pair. Correspondingly, when the `ht_delete()` function is complete, it is the responsibility of the in-memory KVS library to free the used memory of the key-value pair back to the pre-allocated memory chunk. 

## Grading

Your code should turn in `imkv.c` which implements the above functions correctly and efficiently. It will be compiled with test applications with the `-Wall -Werror -pthread -O` flags; it will also be valgrinded to check for memory errors.

Your code will first be measured for correctness, ensuring that it guarantees thread-safety correctly. If you pass the correctness tests, your code will be tested for performance; higher performance will lead to better scores.





