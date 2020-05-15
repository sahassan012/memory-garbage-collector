# A Memory Garbage Collector for C
## About
In C, dynamic memory accessed via ``malloc`` and ``free`` allow control and interpretability of programs. Memory Garbage Collector is a type of dynamic memory management that is an implicit allocator. This means that we don't need to explicitly free the unused memory. This tool finds the blocks that are not being used and returns them to the process as free chunks of memory. 

## How it works
Starting from a set of root pointers, this program traverses object graphs(implemented as singly-linked lists) to find all reachable chunks. These chunks are marked using the second lowest order bit in the header of each chunk. 

Here is a visual for the heap(dynamic memory).
Each allocated or free block starts with a header **h** and contains payload **x**. It then ends with a footer **f**.

```console
----------------------------------------------------------------------
|h|x|x|f|h| | | | |f|h|x|x|x|x|f|h| | | | |...
----------------------------------------------------------------------
  ^       ^           ^           ^
  |       |           |           |
  a       b           c           the rest of the heap...
  (inuse) (free)      (inuse) 
```
**a** represents an allocated section of the heap with a payload plus its padding.
**b** is a free node of size 4 words.

A brief summary of what is implemented in ``mas.c`` :
The program interacts with the memory allocator and the root table to find/free the dead blocks. 

The garbage collector uses mark-and-sweep algorithm.
- MARK phase - mark all blocks in the hashtable
- SWEEP phase - scan the list of blocks and free every block that isn't marked

## How to run (from command line)
Run the Makfile:
```console
$ make
```
This should generate an executable ``mas`` 
Run the executable:
```console
$ ./mas
```
