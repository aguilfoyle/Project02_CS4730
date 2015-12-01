# Project 02 - Virtual Memory Simulator

## Description

In this project, you will implement a virtual memory simulator in order to understand the behavior of a page table and the page replacement algorithms. Virtual memory allows the execution of processes that are not completely in memory. This is achieved by page table and page replacement techniques. A page table stores the mapping between virtual addresses and physical addresses. A page fault mechanism by which the memory management unit (MMU) can ask the operating system (OS) to bring in a page from the disk. The system we are simulating is 16-bit machine (8 bits for index and 8 bits for offset). 

## Part 1: Page Table

Implemented: 'hit_test()' and 'pagefault_handler()' functions in pagetable.c file

## Part 2: Directory

Implemented: FIFO (First-in-first-out), LRU (Least-recently-used), LFU (Least-frequently-used)

## How to run:

To run the program you can run the MAKEFILE by using the 'make' command. It will compile 8 files:
disk.c, disk.h, input_gen.c, pagetable.c, pagetable.h, replacement.c, vm.c, vm.h

After compling simply use the command: './vm [0-3] < [fileName.txt]'
option 0: will random choose a victim frame
option 1: will use fifo page replacement algorithm to get a victim frame
option 2: will use lru page replacement algorithm to get a victim frame
option 3: will use lfu page replacement algorithm to get a victim frame
