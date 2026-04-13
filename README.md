# 📌 Algorithms & Data Structures (algo-ds)

This repository contains implementations of common **Data Structures** and **Sorting/Searching Algorithms** in C++ with clear explanations and simulations.

The goal is not only to implement algorithms, but also to understand:

- **Why they work**
- **When to use them**
- **Why some algorithms perform better in real-world scenarios**
- **Time complexity vs real machine performance**

---

# 📂 Sorting Algorithms Included

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort
- (More coming...)

---

# ⚡ Why Quick Sort is Faster than Merge Sort in Real Life

Both Quick Sort and Merge Sort have an average time complexity of:

- **Quick Sort:** `O(n log n)` (average case)
- **Merge Sort:** `O(n log n)` (always)

So why is Quick Sort often faster in real life?

---

## ✅ 1. Quick Sort is In-Place (Low Memory Usage)

Quick sort sorts the array mostly inside the same memory region.

- It does not require extra arrays.
- It only needs recursion stack memory.

Merge sort requires an additional temporary array during merging:

- Merge Sort needs extra space: `O(n)`
- Quick Sort needs extra space: `O(log n)` (recursion stack)

Because of less memory usage, Quick Sort avoids extra memory allocation overhead.

---

## ✅ 2. Better Cache Performance (CPU Friendly)

Modern CPUs are very fast, but accessing RAM is slow.

Quick Sort performs sorting in the same array continuously, meaning:

- it accesses nearby memory locations frequently
- CPU cache works efficiently

Merge sort continuously creates temporary arrays and copies data back and forth, causing:

- more cache misses
- more memory movement

So even though both are `O(n log n)`, Quick Sort tends to run faster on real hardware.

---

## ✅ 3. Merge Sort Does More Copying

Merge sort merges by copying elements into a temporary array and then copying them back.

That means:

- each merge step involves extra copying operations
- large arrays require heavy memory bandwidth

Quick sort performs mostly swapping operations and partitioning.

Less copying = faster execution.

---

## ✅ 4. Quick Sort Has Smaller Constant Factors

Time complexity ignores constants, but constants matter in real life.

Quick Sort often has fewer operations per step compared to Merge Sort.

So even if both are `O(n log n)`, quicksort usually wins due to smaller overhead.

---

## ⚠️ When Merge Sort is Better

Merge sort is better when:

- stability is required (stable sorting)
- worst-case performance must be guaranteed
- sorting linked lists
- external sorting (disk-based sorting)

Merge sort always guarantees:

- `O(n log n)` worst case

Quick sort worst case:

- `O(n^2)` (if pivot selection is poor)

---

# ⚡ Why Insertion Sort is Fast for Small Arrays

Insertion sort has worst-case time complexity:

- `O(n^2)`

So why is it still used in real-world libraries?

Because for small arrays, insertion sort is extremely efficient.

---

## ✅ 1. Very Low Overhead

Insertion sort has:

- no recursion
- no extra memory allocation
- very simple operations (comparisons and shifting)

For small input sizes, algorithms like merge sort and quick sort introduce overhead such as:

- recursion calls
- partitioning/merging logic

Insertion sort avoids all of that.

---

## ✅ 2. Best Case is O(n)

If the array is already sorted or almost sorted:

Insertion sort runs in:

- `O(n)`

Because it only checks each element once and does almost no shifting.

This makes it perfect for nearly sorted data.

---

## ✅ 3. Adaptive Sorting

Insertion sort is adaptive, meaning:

- the more sorted the array is, the faster it becomes

For example:

- If only a few elements are out of place, insertion sort performs very few shifts.

---

## ✅ 4. Used Inside Quick Sort and Merge Sort Implementations

Many standard libraries use hybrid sorting strategies:

- Quick Sort for large partitions
- Insertion Sort for small partitions (like size < 10 or < 20)

Because insertion sort finishes small parts faster than recursion-based sorts.

This is why real-world sort implementations are often called:

- **Introsort**
- **Timsort**
- Hybrid sorting

---

# 📌 Summary Table

| Algorithm       | Average Time | Worst Time | Extra Space | Real-Life Speed |
|----------------|-------------|------------|-------------|----------------|
| Quick Sort     | O(n log n)  | O(n²)      | O(log n)    | Very Fast |
| Merge Sort     | O(n log n)  | O(n log n) | O(n)        | Fast but more memory |
| Insertion Sort | O(n²)       | O(n²)      | O(1)        | Best for small/nearly sorted |

---

# 🚀 Future Plans

- Add linked list sorting
- Add heap sort
- Add binary search variations
- Add graph algorithms (BFS/DFS)
- Add DP problems (LIS, knapsack)
- Add recursion backtracking problems

---

# 👨‍💻 Author Notes

This repository is built for learning, practice, and interview preparation.

Focus areas:
- clean implementation
- step-by-step algorithm understanding
- strong fundamentals
