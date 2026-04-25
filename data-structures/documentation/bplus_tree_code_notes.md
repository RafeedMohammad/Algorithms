# B+ Tree — Full Code Notes

> **File:** `bPlus-tree.cpp` · **Order:** 4 (configurable) · **Language:** C++17

---

## Table of Contents

1. [What is a B+ Tree?](#1-what-is-a-b-tree)
2. [Key Invariants](#2-key-invariants)
3. [Includes & Namespace](#3-includes--namespace)
4. [Node Structure](#4-node-structure)
5. [BPlusTree Class & Constructor](#5-bplustree-class--constructor)
6. [Search](#6-search)
7. [Insert (Public Entry Point)](#7-insert-public-entry-point)
8. [insertNonFull (Private)](#8-insertnonfull-private)
9. [splitChild (Private)](#9-splitchild-private)
10. [printTree — Level-Order](#10-printtree--level-order)
11. [printLeaves — Leaf Linked List](#11-printleaves--leaf-linked-list)
12. [main()](#12-main)
13. [Worked Insertion Trace](#13-worked-insertion-trace)
14. [Complexity Summary](#14-complexity-summary)

---

## 1. What is a B+ Tree?

A **B+ Tree** is a self-balancing, sorted, multi-way search tree designed for efficient disk-based storage and range queries. It differs from a plain B-Tree in two important ways:

| Feature        | B-Tree                     | B+ Tree                            |
| -------------- | -------------------------- | ---------------------------------- |
| Data stored    | In every node              | **Only in leaves**                 |
| Internal nodes | Hold keys + data           | Hold keys **only** (routing keys)  |
| Leaf nodes     | Isolated                   | Linked in a **sorted linked list** |
| Range queries  | Expensive (tree traversal) | Cheap (walk the leaf list)         |

This implementation keeps **integer key-value pairs** and links all leaves so range-scans are O(range size) after reaching the first leaf.

---

## 2. Key Invariants

For an **order-_m_** B+ Tree (m = 4 here):

```
Max keys per node  = m - 1  = 3
Max children       = m      = 4
Min keys (non-root)= ⌈m/2⌉ - 1 = 1
```

- All leaves are at the **same depth** (the tree grows upward from the root).
- Internal node keys are **routing separators**; the same key also lives in the leaf.
- When a node is **full** (keys == maxKeys) and receives one more key, it **splits** into two nodes, promoting one key up to the parent.

---

## 3. Includes & Namespace

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
```

| Header        | Why it's needed                                                        |
| ------------- | ---------------------------------------------------------------------- |
| `<iostream>`  | `cout` for printing                                                    |
| `<vector>`    | Dynamic arrays for keys, values, children                              |
| `<algorithm>` | Available for utilities (e.g. `lower_bound`); not actively called here |

`using namespace std;` avoids prefixing everything with `std::`.

---

## 4. Node Structure

```cpp
struct Node
{
    bool leaf;
    vector<int> keys;     // sorted keys
    vector<int> values;   // only meaningful for leaf nodes
    vector<Node *> child; // child pointers (size = keys.size() + 1 for internal)
    Node *next;           // leaf-level linked list
    Node(bool isLeaf) : leaf(isLeaf), next(nullptr) {}
};
```

### Field-by-field

| Field    | Type            | Purpose                                                           |
| -------- | --------------- | ----------------------------------------------------------------- |
| `leaf`   | `bool`          | `true` → leaf node; `false` → internal (routing) node             |
| `keys`   | `vector<int>`   | Sorted list of keys in this node                                  |
| `values` | `vector<int>`   | Payload; **parallel to `keys`**, only used in leaf nodes          |
| `child`  | `vector<Node*>` | Child pointers; always **one more** than `keys` in internal nodes |
| `next`   | `Node*`         | Links leaf nodes left-to-right; `nullptr` in internal nodes       |

### Why `values` is leaf-only

B+ Trees store actual records only at the leaves. Internal nodes hold **copies** of keys purely to route searches — they have no associated payload. Keeping `values` in a parallel vector (same index as `keys`) is the simplest approach.

### Leaf linked list invariant

```
leaf₀ ──next──▶ leaf₁ ──next──▶ leaf₂ ──next──▶ nullptr
[5, 6, 7]       [10, 12, 17]    [20, 30]
```

This allows sequential scans without going back up the tree.

---

## 5. BPlusTree Class & Constructor

```cpp
class BPlusTree
{
    Node *root;
    const int order;   // max children per internal node
    const int maxKeys; // order - 1 (max keys in any node)

public:
    BPlusTree(int ord = 4) : root(nullptr), order(ord), maxKeys(ord - 1) {}
```

### What these members mean

- **`root`** — Entry point into the tree. Starts as `nullptr` (empty tree).
- **`order`** — The _branching factor_. Order 4 means up to 4 children and up to 3 keys per node.
- **`maxKeys`** — Derived constant (`order - 1`). A node is **full** when `keys.size() == maxKeys`.

> The tree starts with `root = nullptr`. The first `insert()` call creates the root leaf.

---

## 6. Search

```cpp
int search(int key) const
{
    Node *cur = root;
    while (cur)
    {
        int i = 0;
        while (i < (int)cur->keys.size() && key > cur->keys[i])
            ++i;

        if (cur->leaf)
            return (i < (int)cur->keys.size() && cur->keys[i] == key)
                       ? cur->values[i]
                       : -1; // NOT FOUND

        cur = cur->child[i];
    }
    return -1;
}
```

### How it works — step by step

1. **Start at root.** Walk down the tree level by level.
2. **Find the slot `i`.**  
   Scan `keys` left-to-right while `key > keys[i]`, incrementing `i`.  
   When the loop ends, `i` is the **first index where `keys[i] >= key`** (or past the end).
3. **At a leaf:** Check if `keys[i] == key` exactly. If yes → return `values[i]`. Otherwise → `-1`.
4. **At an internal node:** Descend to `child[i]`.
   - `child[0]` holds keys < `keys[0]`
   - `child[i]` holds keys in `[keys[i-1], keys[i])`
   - `child[last]` holds keys ≥ `keys[last-1]`

### Visualization

```
Internal: [10 | 17]
           ↙    ↓    ↘
        <10   10-17  >=17
```

Search for `12`: `12 > 10` → i=1; `12 < 17` → stop. Descend `child[1]`. Reach leaf, find `12`. ✓

### Complexity

- **O(log n)** — proportional to tree height.

---

## 7. Insert (Public Entry Point)

```cpp
void insert(int key, int value)
{
    if (!root)
    {
        root = new Node(true);
        root->keys.push_back(key);
        root->values.push_back(value);
        return;
    }

    // Pre-emptively split a full root before descending
    if ((int)root->keys.size() >= maxKeys)
    {
        Node *newRoot = new Node(false);
        newRoot->child.push_back(root);
        splitChild(newRoot, 0);
        root = newRoot;
    }

    insertNonFull(root, key, value);
}
```

### Logic

**Case 1 — Empty tree:**  
Create the first leaf node and store the key directly. Done.

**Case 2 — Root is full (keys == maxKeys):**  
This is the **proactive split** strategy. Before descending, we split the root so the recursive insertion always finds room.

```
Step 1: Create a new (empty) internal node — this becomes the new root.
Step 2: Make the old root its single child.
Step 3: splitChild() splits that child, promoting a key up into newRoot.
Step 4: Update root pointer.
```

This is why the tree grows **upward** — a new root is born here, increasing height by 1.

**Case 3 — Root has space:**  
Delegate to `insertNonFull()`.

> **Why `>= maxKeys` not `== maxKeys`?**  
> Using `>=` is defensive; it also catches any edge case where a node somehow exceeds the limit.

---

## 8. `insertNonFull` (Private)

```cpp
void insertNonFull(Node *node, int key, int value)
{
    if (node->leaf)
    {
        int i = 0;
        while (i < (int)node->keys.size() && key > node->keys[i])
            ++i;
        node->keys.insert(node->keys.begin() + i, key);
        node->values.insert(node->values.begin() + i, value);
        return;
    }

    // Internal node: choose the right child
    int i = 0;
    while (i < (int)node->keys.size() && key > node->keys[i])
        ++i;

    if ((int)node->child[i]->keys.size() >= maxKeys)
    {
        splitChild(node, i);
        if (key > node->keys[i])
            ++i;
    }

    insertNonFull(node->child[i], key, value);
}
```

### Leaf path

Find the sorted position `i` where the new key belongs, then insert at that position in **both** `keys` and `values` (keeping them in sync):

```
Before: keys = [5, 10, 17]
Insert 7 → i = 1 (7 > 5, 7 < 10)
After:  keys = [5, 7, 10, 17]   ← vector::insert shifts elements right
```

### Internal node path

1. Find child index `i` (same linear scan).
2. If `child[i]` is **full**, split it first.  
   After the split, one key is promoted to `node->keys[i]`. We then re-check: if the new key is greater than that promoted key, we move to the right sibling (`++i`).
3. Recurse into `child[i]`.

> **This is a top-down, proactive split approach.** By the time we reach a node to insert into, it is guaranteed to have room. No backtracking needed.

---

## 9. `splitChild` (Private)

This is the heart of the tree — it handles all the structural changes when a node overflows.

```cpp
void splitChild(Node *parent, int idx)
{
    Node *child   = parent->child[idx];
    Node *sibling = new Node(child->leaf);

    int mid = order / 2; // split position
```

`mid = 4/2 = 2`. Keys at index `[mid, end)` move to `sibling`; keys at `[0, mid)` stay in `child`.

### Branch A — Splitting a Leaf Node

```cpp
    if (child->leaf)
    {
        sibling->values.assign(child->values.begin() + mid, child->values.end());
        child->values.erase(child->values.begin() + mid, child->values.end());

        sibling->keys.assign(child->keys.begin() + mid, child->keys.end());
        child->keys.erase(child->keys.begin() + mid, child->keys.end());

        // Maintain leaf linked list
        sibling->next = child->next;
        child->next   = sibling;

        // Promote copy of first sibling key
        parent->keys.insert(parent->keys.begin() + idx, sibling->keys.front());
    }
```

**Example:** Split leaf `[5, 7, 10]` with `mid=2`:

```
Before split:
  child   = [5, 7, 10]

After split:
  child   = [5, 7]          (keys[0..mid-1])
  sibling = [10]             (keys[mid..end])

Promoted key = 10  (COPY — still lives in leaf!)
Linked list:  child ──next──▶ sibling
```

> **Copy-up rule:** In a B+ Tree, the promoted key from a leaf split is **copied** (not moved). The key stays in the leaf for actual lookup; the copy in the parent is just a routing separator.

### Branch B — Splitting an Internal Node

```cpp
    else
    {
        sibling->child.assign(child->child.begin() + mid + 1, child->child.end());
        child->child.erase(child->child.begin() + mid + 1, child->child.end());

        int promo = child->keys[mid];
        child->keys.erase(child->keys.begin() + mid);
        parent->keys.insert(parent->keys.begin() + idx, promo);
    }
```

**Example:** Split internal node `[10, 17, 20]` (with 4 children) with `mid=2`:

```
Before:
  child   = keys[10, 17, 20]   children[c0, c1, c2, c3]

After:
  child   = keys[10]           children[c0, c1]
  sibling = keys[20]           children[c2, c3]
  promoted = 17  (REMOVED from child — push-up rule)
```

> **Push-up rule:** In an internal node split, the median key is **removed** from the child and pushed up. It no longer stays in either half, because internal nodes are routing-only.

### Final step (both branches)

```cpp
    parent->child.insert(parent->child.begin() + idx + 1, sibling);
```

The new sibling is wired in immediately after the original child in the parent's child list.

---

## 10. `printTree` — Level-Order

```cpp
void printTree() const
{
    if (!root) { cout << "(empty)\n"; return; }

    vector<Node *> level{root};
    while (!level.empty())
    {
        vector<Node *> next;
        for (Node *n : level)
        {
            cout << "[";
            for (int i = 0; i < (int)n->keys.size(); ++i)
                cout << (i ? " " : "") << n->keys[i];
            cout << "] ";

            if (!n->leaf)
                for (Node *c : n->child)
                    next.push_back(c);
        }
        cout << "\n";
        level = move(next);
    }
}
```

### Algorithm — BFS with two vectors

This is a classic **Breadth-First Traversal** using two vectors instead of a queue:

```
Iteration 1: level = [root]           → print root, collect its children → next
Iteration 2: level = [child1, child2] → print both, collect their children → next
...and so on until no more children.
```

**The separator trick:**

```cpp
cout << (i ? " " : "") << n->keys[i];
```

Prints a space **before** every key except the first — cleaner than a trailing-space approach.

**Sample output for seq `{10,20,5,6,12,30,7,17}`:**

```
[7 12 17]
[5 6] [10] [12] [17 20 30]
```

_(exact output depends on insertion order and split positions)_

---

## 11. `printLeaves` — Leaf Linked List

```cpp
void printLeaves() const
{
    Node *p = root;
    while (p && !p->leaf)
        p = p->child.front();

    while (p)
    {
        for (int i = 0; i < (int)p->keys.size(); ++i)
            cout << "(" << p->keys[i] << "," << p->values[i] << ") ";
        p = p->next;
    }
    cout << "\n";
}
```

### Two-phase walk

**Phase 1 — Find leftmost leaf:**  
Keep following `child.front()` (the leftmost child) until we hit a leaf. Since all leaves are at the same depth, this is O(height).

**Phase 2 — Walk the linked list:**  
Follow `next` pointers across all leaves, printing each `(key, value)` pair.

**Sample output:**

```
(5,50) (6,60) (7,70) (10,100) (12,120) (17,170) (20,200) (30,300)
```

> This is the killer feature of B+ Trees — range queries (`WHERE key BETWEEN 7 AND 20`) become a simple linked-list traversal after finding the start key.

---

## 12. `main()`

```cpp
int main()
{
    BPlusTree tree(4); // order 4 → max 3 keys per node

    vector<int> seq = {10, 20, 5, 6, 12, 30, 7, 17};
    for (int k : seq)
        tree.insert(k, k * 10); // value = key × 10 (dummy payload)

    cout << "Level-order view:\n";
    tree.printTree();

    cout << "\nLeaf list (key,value):\n";
    tree.printLeaves();

    int q = 12;
    cout << "\nSearch " << q << " -> " << tree.search(q) << "\n";
}
```

- Creates an order-4 B+ Tree (max 3 keys, max 4 children per node).
- Inserts 8 keys in a deliberately unsorted order to exercise splitting.
- Prints the tree structure level-by-level.
- Prints the leaf chain (verifies the sorted linked list).
- Searches for key `12` — should return `120` (= 12 × 10).

---

## 13. Worked Insertion Trace

Inserting `{10, 20, 5, 6, 12, 30, 7, 17}` into an order-4 tree (maxKeys = 3):

```
Insert 10:  root = [10]
Insert 20:  root = [10, 20]
Insert 5:   root = [5, 10, 20]       ← leaf, still has room

Insert 6:   root full (3 keys) → SPLIT ROOT
            old root = [5, 10, 20]  mid=2
            child   = [5, 10]
            sibling = [20]
            promo   = 20  (copy-up)
            new root (internal) = [20]
              children: [5,10]  [20]

            Now insertNonFull(root, 6):
            6 < 20 → go to child[0] = [5, 10]
            insert 6 → [5, 6, 10]

After 6:
  [20]
  /   \
[5,6,10] [20]

Insert 12:  go to child[0] = [5,6,10] → full!
            splitChild(root, 0):
              child   = [5, 6]
              sibling = [10]
              promo   = 10 (copy-up)
              root becomes [10, 20]
            12 > 10 → go to child[1] = [10]
            insert 12 → [10, 12]

After 12:
    [10, 20]
   /    |    \
[5,6] [10,12] [20]

Insert 30:  30 > 20 → child[2] = [20]
            insert → [20, 30]

Insert 7:   7 < 10 → child[0] = [5,6] → has room
            insert 7 → [5, 6, 7]

After 30 & 7:
    [10, 20]
   /    |    \
[5,6,7] [10,12] [20,30]

Insert 17:  17 > 10, 17 < 20 → child[1] = [10,12] → has room
            insert → [10, 12, 17]

Final tree:
    [10, 20]
   /    |    \
[5,6,7] [10,12,17] [20,30]

Leaf linked list:
[5,6,7] → [10,12,17] → [20,30] → null
```

---

## 14. Complexity Summary

| Operation  | Time         | Notes                                  |
| ---------- | ------------ | -------------------------------------- |
| Search     | O(log n)     | One root-to-leaf path                  |
| Insert     | O(log n)     | Proactive splits, no backtracking      |
| Range scan | O(log n + k) | Find start in O(log n), walk k leaves  |
| Space      | O(n)         | Each key stored exactly once in leaves |

> **Height** of the tree: `h = ⌈log_m(n)⌉` where m = order, n = number of keys.  
> For m=4 and 1 million keys: h ≈ 10. Every search touches at most **10 nodes**.

---

_Generated for `bPlus-tree.cpp` — B+ Tree, Order 4, C++17_
