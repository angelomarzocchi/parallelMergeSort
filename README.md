# parallelMergeSort
Parallel Merge Sort alghoritm using openMP
The strategy to parallelize the merge sort algorithm is as follows:
1. Divide the vector of size N into p slices, where p is the number of processors.
   Each section is large N/p
   
2. Each pi processor does merge sort on one of the sections.

3. In log2(p) steps the algorithm will merge the ordered sections like this:

    3.1 At each step p/2^i processors will be involved, where i is the current step
      Each of them merges 2 consecutive sections.
