# 🌳 Comprehensive B+ Tree Lecture — Theory, Visuals & Simulations

---

## Table of Contents

1. [What is a B+ Tree?](#1-what-is-a-b-tree)
2. [B-Tree vs B+ Tree](#2-b-tree-vs-b-tree)
3. [Node Structure Deep-Dive](#3-node-structure-deep-dive)
4. [Order, Capacity & Invariants](#4-order-capacity--invariants)
5. [Search — How it Works](#5-search--how-it-works)
6. [Insertion — Full Step-by-Step Simulation](#6-insertion--full-step-by-step-simulation)
7. [Leaf Split — Detailed Mechanics](#7-leaf-split--detailed-mechanics)
8. [Internal Node Split — Detailed Mechanics](#8-internal-node-split--detailed-mechanics)
9. [Root Overflow & New Root Creation](#9-root-overflow--new-root-creation)
10. [Full Trace: Insert {10,20,5,6,12,30,7,17} into Order-4 Tree](#10-full-trace-insert-10205612307-17-into-order-4-tree)
11. [Range Query Simulation](#11-range-query-simulation)
12. [Code Walkthrough — Every Line Explained](#12-code-walkthrough--every-line-explained)
13. [splitChild() Deep Dive with Concrete Example](#13-splitchild-deep-dive-with-concrete-example)
14. [Common Pitfalls](#14-common-pitfalls)
15. [Quick Reference Card](#15-quick-reference-card)

---

## 1. What is a B+ Tree?

A **B+ Tree** is a self-balancing, multi-way search tree designed for systems where data lives on disk (databases, file systems). Its key insight:

```
ALL ACTUAL DATA lives only at the LEAF level.
Internal nodes hold only SEPARATOR KEYS to guide search.
Leaf nodes are chained in a LINKED LIST for fast range scans.
```

### Why it matters:

- **Disk I/O is expensive** — minimizing tree height = fewer disk reads
- **Range queries are cheap** — walk the leaf chain, no need to re-traverse the tree
- **Predictable performance** — always O(log_d N), never degenerates

---

## 2. B-Tree vs B+ Tree

```
B-TREE                              B+ TREE
═══════                             ════════

     [15]                                [15]
    /    \                              /    \
 [10]   [20]                        [10]   [20]
  ↑       ↑                                  ↑
data     data                              [10]→[15]→[20]
stored             only leaves              linked list
anywhere           have data
```

| Feature        | B-Tree             | B+ Tree                           |
| -------------- | ------------------ | --------------------------------- |
| Data location  | Any node           | Leaves only                       |
| Internal nodes | Store actual data  | Separator keys only               |
| Leaf linking   | Not linked         | Doubly/singly linked              |
| Range query    | Slow (re-traverse) | Fast (walk leaf list)             |
| Duplicate keys | Possible           | Internal keys = navigation copies |

---

## 3. Node Structure Deep-Dive

```
INTERNAL NODE
┌──────────────────────────────────────────────────┐
│ isLeaf = false                                   │
│                                                  │
│ keys:     [ K0 | K1 | K2 ]                       │
│                                                  │
│ children: [ C0 | C1 | C2 | C3 ]                  │
│                                                  │
│ Invariant: C0 < K0 ≤ C1 < K1 ≤ C2 < K2 ≤ C3    │
│                                                  │
│ next = nullptr (unused)                          │
└──────────────────────────────────────────────────┘

LEAF NODE
┌──────────────────────────────────────────────────┐
│ isLeaf = true                                    │
│                                                  │
│ keys:   [ K0 | K1 | K2 ]                         │
│ values: [ V0 | V1 | V2 ]  ← actual data/records │
│                                                  │
│ next ──────────────────────────────→ [next leaf] │
│                                                  │
│ children = [] (empty, unused)                    │
└──────────────────────────────────────────────────┘
```

### C++ struct:

```cpp
struct Node {
    bool leaf;                   // am I a leaf?
    vector<int> keys;            // sorted separator/data keys
    vector<int> values;          // payload (leaf only)
    vector<Node*> child;         // child pointers (internal only)
    Node* next;                  // next leaf pointer (leaf only)

    Node(bool isLeaf) : leaf(isLeaf), next(nullptr) {}
};
```

---

## 4. Order, Capacity & Invariants

For **order d = 4** (our running example):

```
┌─────────────────────────────────────────────────────┐
│                  ORDER d = 4                        │
│                                                     │
│  Max children per internal node  = d     = 4        │
│  Max keys    per node            = d-1   = 3        │
│  Min keys    per non-root node   = ⌈d/2⌉-1 = 1     │
│  Split triggers when keys count  = d     = 4        │
│  After leaf split: left keeps    = ⌈d/2⌉ = 2 keys  │
│  After leaf split: right gets    = d-⌈d/2⌉= 2 keys │
└─────────────────────────────────────────────────────┘
```

### The Overflow Rule:

```
Node has d-1 keys → FULL (no more insertions without split)
Node has d   keys → OVERFLOW → MUST SPLIT
```

---

## 5. Search — How it Works

### Algorithm (verbal):

```
search(key):
  1. Start at root
  2. At each internal node:
     - Scan keys left→right
     - Find smallest index i where key ≤ keys[i]
     - Follow children[i]
  3. At leaf: scan keys, return value if found
```

### Visual Search for key=12 (order-4 tree):

```
Tree state:
              [ 10 | 20 ]                ← root (internal)
             /     |     \
        [5|7]   [10|12]  [20|30]         ← leaves

Search(12):

Step 1: At root [10 | 20]
        Is 12 > 10? YES → continue
        Is 12 > 20? NO  → go to children[1]
                     ↓
Step 2: At leaf [10 | 12]
        Is keys[0]=10 == 12? NO
        Is keys[1]=12 == 12? YES → return values[1] ✓
```

### Code path through `search()`:

```cpp
int search(int key) const {
    Node* cur = root;
    while (cur) {
        size_t i = 0;
        // scan: advance i while key > keys[i]
        while (i < cur->keys.size() && key > cur->keys[i]) ++i;
        //   key=12, keys=[10,20]: 12>10 → i=1, 12>20? NO → i=1

        if (cur->leaf) {
            // at leaf: check if keys[i] actually equals key
            if (i < cur->keys.size() && cur->keys[i] == key)
                return cur->values[i];   // FOUND
            return -1;                    // NOT FOUND
        }
        cur = cur->child[i];   // go to child[1]
    }
    return -1;
}
```

---

## 6. Insertion — Full Step-by-Step Simulation

### The Big Picture:

```
insert(key, value):
  ┌─────────────────────────────────────────────────┐
  │  Is root null?                                  │
  │  YES → create leaf root, done                   │
  │  NO  → Is root FULL (keys == maxKeys)?          │
  │         YES → split root first, then insert     │
  │         NO  → insertNonFull(root, key, value)   │
  └─────────────────────────────────────────────────┘

insertNonFull(node, key, value):
  ┌─────────────────────────────────────────────────┐
  │  node is a LEAF?                                │
  │  YES → insert key/value in sorted position      │
  │  NO  → find correct child index i               │
  │         Is child[i] FULL?                       │
  │           YES → splitChild(node, i) first       │
  │                 re-check which child to go to   │
  │         recurse into child[i]                   │
  └─────────────────────────────────────────────────┘
```

---

## 7. Leaf Split — Detailed Mechanics

### The Golden Rules for Leaf Split:

```
Given order d=4, max keys = 3, split at 4 keys:

  mid = d/2 = 4/2 = 2

  Original leaf L: [A | B | C | D]   (overflow: 4 keys)
                    0   1   2   3

  Left  (L):  keys[0..mid-1] = [A | B]    keeps first 2
  Right (L2): keys[mid..end] = [C | D]    gets last  2

  Promotion key = L2.keys[0] = C   ← first key of RIGHT leaf

  L.next → L2 → (old L.next)       ← update linked list
```

### Concrete Example — Insert 25 into [10 | 20 | 30]:

```
BEFORE (leaf L, d=4):
┌─────────────────┐
│  10 | 20 | 30   │ → next: NIL
└─────────────────┘
       ↑
   Insert 25 → sorted position → [10 | 20 | 25 | 30]
   Now has 4 keys = OVERFLOW!

splitChild is called:
  mid = 4/2 = 2

SPLIT:
  Left  L:  [10 | 20]          (keys[0..1])
  Right L2: [25 | 30]          (keys[2..3])

AFTER SPLIT:
┌──────────┐    next     ┌──────────┐    next
│  10 | 20 │ ──────────→ │  25 | 30 │ ──────────→ NIL
└──────────┘             └──────────┘
     L                        L2

Promotion key = L2.keys[0] = 25
→ 25 is inserted into the PARENT internal node
```

---

## 8. Internal Node Split — Detailed Mechanics

### The Golden Rules for Internal Split:

```
Given order d=4, max keys=3, overflow at 4 keys:

  mid = d/2 = 2

  Original internal node P: [A | B | C | D]  children [C0|C1|C2|C3|C4]
                              0   1   2   3

  Left  (P):  keys[0..mid-1]    = [A | B]     children[0..mid]   = [C0|C1|C2]
  Promoted:   keys[mid]          = C           → goes UP to parent
  Right (P2): keys[mid+1..end]  = [D]         children[mid+1..]  = [C3|C4]

  KEY DIFFERENCE FROM LEAF:
  - Promoted key LEAVES the node (not kept in either child)
  - Children are split: left gets [0..mid], right gets [mid+1..end]
```

### Concrete Example — Internal overflow:

```
BEFORE: Internal node with keys [5 | 12 | 15 | 25]  ← overflow (4 keys)
        children:               [C0| C1| C2 | C3|C4]

  mid = 4/2 = 2

SPLIT:
  Left  P:  keys = [5 | 12]    children = [C0 | C1 | C2]
  Promo:    key  = 15          → goes up to parent
  Right P2: keys = [25]        children = [C3 | C4]

PARENT receives:
  - Promotion key 15 inserted at correct position
  - P2 added as new child right of P

RESULT:
         Parent
        /  15  \
       /         \
   [5|12]        [25]
  /  |  \        /  \
 C0  C1  C2    C3   C4
```

---

## 9. Root Overflow & New Root Creation

```
When the ROOT overflows, we cannot push a key "up" — there is no parent!

Solution:
  1. Create a NEW empty internal node: newRoot
  2. Set newRoot.children[0] = old root
  3. Call splitChild(newRoot, 0)  — splits the old root
  4. Set tree.root = newRoot

BEFORE (root is full leaf [5 | 10 | 20]):
    ┌─────────────┐
    │  5 | 10 | 20│   ← root (leaf)
    └─────────────┘

Insert 15 → overflow:
    leaf becomes [5 | 10 | 15 | 20]  → needs split

Create newRoot (internal, empty):
    [      ]
       |
    [5|10|15|20]  ← child[0]

splitChild(newRoot, 0):
  mid=2, left=[5|10], right=[15|20], promo=15

AFTER:
         [15]          ← newRoot (new root!)
        /    \
    [5|10]  [15|20]    ← leaves, linked
```

---

## 10. Full Trace: Insert {10,20,5,6,12,30,7,17} into Order-4 Tree

**Order d=4 → maxKeys=3, mid=2**

---

### Insert 10:

```
Tree is empty → create leaf root.

ROOT: [10]   (leaf)
```

---

### Insert 20:

```
Root [10] has room (1 < 3 keys).
insertNonFull → it's a leaf → insert 20 after 10.

ROOT: [10 | 20]   (leaf)
```

---

### Insert 5:

```
Root [10|20] has room. Insert 5 → sorted → before 10.

ROOT: [5 | 10 | 20]   (leaf, now FULL at 3 keys)
```

---

### Insert 6:

```
Root [5|10|20] is FULL (keys==maxKeys==3).

Pre-split the root:
  1. newRoot = new internal node
  2. newRoot.children[0] = old root [5|10|20]
  3. splitChild(newRoot, 0):
       old root becomes child to split
       Insert 6 → [5|6|10|20] first? NO — split happens BEFORE insert

  Actually the code splits the root (which has maxKeys=3 keys at this point),
  then calls insertNonFull.

  splitChild(newRoot, 0):
    child = [5|10|20]
    mid = 4/2 = 2
    left  = [5|10]       (keys[0..1])
    right = [20]         (keys[2..2])  → newNode
    promo = 20
    newRoot.keys = [20]
    newRoot.children = [ [5|10] | [20] ]

  Tree after split:
        [20]              ← newRoot (internal)
       /    \
   [5|10]  [20]          ← leaves

  Now insertNonFull(newRoot, 6):
    At internal [20]: 6 > 20? NO → go to children[0] = [5|10]
    At leaf [5|10]:  insert 6 sorted → [5|6|10]

FINAL TREE AFTER INSERT 6:
        [20]
       /    \
  [5|6|10]  [20]         ← leaves, linked: [5|6|10]→[20]
```

---

### Insert 12:

```
Root [20] has room. insertNonFull(root, 12):
  At internal [20]: 12 > 20? NO → children[0] = [5|6|10]
  [5|6|10] is FULL (3 keys)!

  splitChild(root, 0):
    child = [5|6|10]
    mid=2
    left  = [5|6]    (keys[0..1])
    right = [10]     (keys[2])   → newNode
    promo = 10
    root.keys → insert 10 at idx 0 → [10|20]
    root.children → [ [5|6] | [10] | [20] ]

  Tree after split:
          [10 | 20]
         /    |    \
      [5|6]  [10]  [20]    ← leaves linked: [5|6]→[10]→[20]

  Now: 12 > root.keys[0]=10? YES → i becomes 1
       12 > root.keys[1]=20? NO  → go to children[1] = [10]
  At leaf [10]: insert 12 → [10|12]

FINAL TREE AFTER INSERT 12:
          [10 | 20]
         /    |    \
      [5|6] [10|12] [20]   ← leaves linked: [5|6]→[10|12]→[20]
```

---

### Insert 30:

```
insertNonFull(root, 30):
  At internal [10|20]: 30>10 YES, 30>20 YES → i=2 → children[2]=[20]
  [20] has room (1 key). Insert 30 → [20|30]

FINAL TREE AFTER INSERT 30:
          [10 | 20]
         /    |    \
      [5|6] [10|12] [20|30]   ← linked: [5|6]→[10|12]→[20|30]
```

---

### Insert 7:

```
insertNonFull(root, 7):
  At [10|20]: 7>10? NO → i=0 → children[0]=[5|6]
  [5|6] has room. Insert 7 → [5|6|7]   (now FULL)

FINAL TREE AFTER INSERT 7:
          [10 | 20]
         /    |    \
     [5|6|7] [10|12] [20|30]   ← linked
```

---

### Insert 17:

```
insertNonFull(root, 17):
  At [10|20]: 17>10 YES, 17>20 NO → i=1 → children[1]=[10|12]
  [10|12] has room (2 keys < 3). Insert 17 → [10|12|17]  (now FULL)

FINAL TREE AFTER INSERT 17:
          [10 | 20]
         /    |    \
     [5|6|7] [10|12|17] [20|30]
```

---

### 🎯 Final Tree State (all 8 keys inserted):

```
                   [ 10 | 20 ]
                  /     |     \
            [5|6|7]  [10|12|17]  [20|30]

Leaf linked list (left → right):
  [5|6|7] → [10|12|17] → [20|30] → NIL

Level-order print:
  Level 0: [10 20]
  Level 1: [5 6 7] [10 12 17] [20 30]
```

---

## 11. Range Query Simulation

### Query: Find all keys between 6 and 20 (inclusive)

```
Step 1: Search for leftmost key ≥ 6
  Root [10|20]: 6>10? NO → child[0] = [5|6|7]
  Leaf [5|6|7]: scan → keys[1]=6 ✓  FOUND at position 1

Step 2: Walk leaf list, collect until key > 20
  Current leaf [5|6|7]:
    keys[1]=6  → ✓ collect (6, val)
    keys[2]=7  → ✓ collect (7, val)
    (end of leaf)

  Follow next → [10|12|17]:
    keys[0]=10 → ✓ collect
    keys[1]=12 → ✓ collect
    keys[2]=17 → ✓ collect
    (end of leaf)

  Follow next → [20|30]:
    keys[0]=20 → ✓ collect (20 ≤ 20)
    keys[1]=30 → ✗ 30 > 20, STOP

Result: {6, 7, 10, 12, 17, 20}

Cost: 1 tree traversal (O(log N)) + 3 leaf reads (O(k))
      Total: O(log N + k)   ← vs O(k·log N) without linked list!
```

---

## 12. Code Walkthrough — Every Line Explained

### `insert()` — The Entry Point:

```cpp
void insert(int key, int value) {
    // Case 1: Tree is empty → create first leaf as root
    if (!root) {
        root = new Node(true);          // isLeaf=true
        root->keys.push_back(key);
        root->values.push_back(value);
        return;
    }

    // Case 2: Root is FULL → must split root before inserting
    // Why? insertNonFull assumes the node it receives is NOT full.
    if ((int)root->keys.size() == maxKeys) {
        Node* newRoot = new Node(false);      // new internal node
        newRoot->child.push_back(root);       // old root becomes child[0]
        splitChild(newRoot, 0);               // split child at index 0
        root = newRoot;                       // update tree root pointer
    }

    // Case 3: Root has space → descend and insert
    insertNonFull(root, key, value);
}
```

### `insertNonFull()` — The Recursive Descent:

```cpp
void insertNonFull(Node* node, int key, int value) {
    size_t i = node->keys.size();

    if (node->leaf) {
        // We're at a leaf → find sorted insertion position
        i = 0;
        while (i < node->keys.size() && key > node->keys[i]) ++i;
        // i now points to first key >= our key
        node->keys.insert(node->keys.begin() + i, key);
        node->values.insert(node->values.begin() + i, value);
        return;
    }

    // Internal node → find which child to descend into
    i = 0;
    while (i < node->keys.size() && key > node->keys[i]) ++i;
    // i = index of child subtree where key belongs

    Node* child = node->child[i];
    if ((int)child->keys.size() == maxKeys) {
        // Child is full → split it now (before descending)
        splitChild(node, i);
        // After split: node->keys[i] = newly promoted separator
        // If our key > separator, we should go to right child (i+1)
        if (key > node->keys[i]) ++i;
    }

    insertNonFull(node->child[i], key, value);
}
```

---

## 13. splitChild() Deep Dive with Concrete Example

```cpp
void splitChild(Node* parent, size_t idx) {
    Node* child = parent->child[idx];   // the full child to split
    Node* newNode = new Node(child->leaf);

    int mid = order / 2;  // e.g., order=4 → mid=2
```

### For a LEAF split (child is leaf):

```
child->keys = [10, 20, 25, 30]   (overflow, mid=2)

newNode->keys = child->keys[2..end] = [25, 30]
child->keys after erase        = [10, 20]

newNode->values = child->values[2..end]
child->values after erase      = [val10, val20]

Leaf list update:
  newNode->next = child->next      (new right leaf takes old next)
  child->next   = newNode          (old left leaf points to new right)

promo = newNode->keys[0] = 25     ← FIRST key of right leaf
parent->keys.insert(at idx, 25)
parent->child.insert(at idx+1, newNode)
```

### For an INTERNAL split (child is internal):

```
child->keys     = [5, 12, 15, 25]    (overflow, mid=2)
child->children = [C0, C1, C2, C3, C4]

newNode->keys     = child->keys[3..end]     = [25]
newNode->children = child->children[3..end] = [C3, C4]

child->keys after erase     = [5, 12]       (keep keys[0..mid-1])
child->children after erase = [C0, C1, C2]  (keep children[0..mid])

promo = child->keys[mid] = 15    ← MIDDLE key, REMOVED from child
child->keys.erase(mid)           ← 15 leaves the child!

parent->keys.insert(at idx, 15)
parent->child.insert(at idx+1, newNode)
```

### Side-by-side comparison:

```
                  LEAF SPLIT          INTERNAL SPLIT
                  ──────────          ──────────────
mid index:        d/2 = 2             d/2 = 2
Left keeps:       keys[0..mid-1]      keys[0..mid-1]
Right gets:       keys[mid..end]      keys[mid+1..end]
Promotion key:    right.keys[0]       child.keys[mid] (REMOVED)
Key stays in:     right leaf          NEITHER (goes up only)
Children split:   N/A (leaf)          left:[0..mid], right:[mid+1..]
Linked list:      YES update next     NO (internal nodes not linked)
```

---

## 14. Common Pitfalls

```
❌ PITFALL 1: Wrong promotion key for leaves
   Leaf split promotion = FIRST key of RIGHT node (NOT the median removed)
   The median key stays in the leaf; its copy goes up.

❌ PITFALL 2: Forgetting to update next pointer
   After leaf split:
     newNode->next = child->next;   ← must do FIRST
     child->next = newNode;          ← then update

❌ PITFALL 3: Wrong child index after split
   After splitChild(node, i):
     node->keys[i] = newly inserted separator
     if (key > node->keys[i]) ++i;  ← must re-check!

❌ PITFALL 4: Not handling root overflow
   You MUST check if root is full BEFORE inserting.
   The root has no parent to receive a promoted key!

❌ PITFALL 5: Confusing order vs maxKeys
   order = 4 → maxKeys = 3 (order - 1)
   Split when keys.size() == maxKeys (== 3), not > maxKeys

❌ PITFALL 6: Wrong mid for internal node
   Internal node: left = keys[0..mid-1], promo = keys[mid], right = keys[mid+1..]
   mid key is REMOVED from child. Don't keep it in either child!
```

---

## 15. Quick Reference Card

```
┌───────────────────────────────────────────────────────────────┐
│                    B+ TREE CHEAT SHEET                        │
│                      (order d = 4)                            │
├─────────────────────┬─────────────────────────────────────────┤
│ Max keys/node       │ d - 1 = 3                               │
│ Min keys (non-root) │ ⌈d/2⌉ - 1 = 1                          │
│ Overflow threshold  │ keys.size() == d (= 4)                  │
│ Split mid index     │ d / 2 = 2                               │
│ Left after split    │ keys[0 .. mid-1]                        │
│ Right after split   │ keys[mid ..] (leaf) / keys[mid+1..] (int)│
│ Promotion (leaf)    │ right.keys[0]   — copy stays in leaf    │
│ Promotion (internal)│ child.keys[mid] — key is REMOVED        │
├─────────────────────┼─────────────────────────────────────────┤
│ Search              │ O(log_d N)                              │
│ Insert              │ O(log_d N)                              │
│ Range(k results)    │ O(log_d N + k)                          │
├─────────────────────┼─────────────────────────────────────────┤
│ Leaf linked list    │ child->next = newNode (after split)     │
│ Data location       │ LEAVES ONLY                             │
│ Internal keys       │ Navigation separators only              │
└─────────────────────┴─────────────────────────────────────────┘

INSERTION DECISION TREE:
  insert(key)
    │
    ├─ root null? → create leaf root
    │
    ├─ root full? → split root, create newRoot
    │
    └─ insertNonFull(root)
         │
         ├─ leaf? → insert sorted in keys/values
         │
         └─ internal? → find child index i
                          │
                          ├─ child[i] full? → splitChild(node,i)
                          │                   re-check i
                          │
                          └─ recurse into child[i]
```

---

_End of Comprehensive B+ Tree Lecture_
_Sequence traced: {10, 20, 5, 6, 12, 30, 7, 17} with order=4_
_Every split, promotion, and pointer update shown step by step._
