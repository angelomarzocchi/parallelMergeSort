# ⚡ Parallel Sorting Algorithms

A high-performance implementation of sorting algorithms using **Parallel Computing** techniques. This project leverages **OpenMP** and **C** to efficiently sort large datasets by distributing the workload across multiple processor cores.

## 🚀 Project Overview

This repository contains robust implementations of two major sorting algorithms, optimized for multi-core processors. The goal is to demonstrate the speedup and efficiency gained by parallelizing standard sorting strategies.

* **Parallel Merge Sort:** Uses a divide-and-conquer strategy adapted for parallel execution.
* **Parallel Bitonic Sort:** Implements a sorting network algorithm highly suitable for parallel hardware.

## ✨ Key Features

* **Multi-threading with OpenMP:** Utilizes the OpenMP API to manage threads and shared memory, automatically scaling based on the available hardware threads.
* **Dynamic Workload Distribution:**
    * **Merge Sort:** Divides the array into $p$ slices (where $p$ is the number of processors). Each core sorts a local slice, followed by a parallel merge phase in $\log_2(p)$ steps.
    * **Bitonic Sort:** Performs comparison and swapping phases in parallel, utilizing barrier synchronization to ensure data consistency.
* **Performance Benchmarking:** Includes built-in timing measurements to calculate execution time for specific sorting phases (sorting vs. merging).
* **Scalability:** Automatically calculates the optimal number of threads (powers of 2) to ensure algorithm stability.

## 🛠 Tech Stack

* **Language:** C
* **Parallel Framework:** [OpenMP](https://www.openmp.org/)
* **Environment:** Compatible with GCC and Visual Studio (MSVC).

## 🧩 How It Works

### Parallel Merge Sort Strategy
1. **Partitioning:** The array of size $N$ is divided into $p$ equal subsections.
2. **Local Sort:** Each processor $P_i$ independently sorts its assigned section.
3. **Parallel Merge:** The sorted sections are merged in a tree-like manner. At each step $i$, $p/2^i$ processors merge two consecutive sorted blocks until the entire array is combined.

### Parallel Bitonic Sort Strategy
Utilizes a sorting network approach where comparisons and swaps are independent and executed simultaneously by available threads. The algorithm alternates between *Bitonic Merge* sequences to produce a fully sorted sequence.

## 🚀 Getting Started

To compile and run the project, ensure you have a C compiler with OpenMP support (like `gcc` or MSVC).

### Using GCC
```bash
# Compile Merge Sort
gcc -fopenmp mergesort.c -o mergesort -lm

# Compile Bitonic Sort
gcc -fopenmp ParallelBitonicSort.c -o bitonicsort -lm

# Run
./mergesort
