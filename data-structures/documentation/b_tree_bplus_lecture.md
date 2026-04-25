# 📚 B‑Tree & B⁺‑Tree Lecture Notes
*Lab‑exam preparation – concise, clear, exam‑friendly*

---

## Part 1 – B‑Tree (Very Summarized)

| **Item** | **Brief notes (≤ 15 lines total)** |
|----------|------------------------------------|
| **Definition** | A balanced multi‑way search tree where each node can contain multiple keys and children. |
| **Properties** | • Every node (except root) has **≥ t – 1** keys and **≤ 2t – 1** keys (t = minimum degree). <br>• Keys in a node are sorted. <br>• Child *i* holds keys **between** key *i‑1* and key *i*. |
| **Why used** | Keeps height **O(logₜ N)** even for massive data → few disk I/Os, ideal for databases & filesystems. |
| **Search** | Start at root, binary‑search the keys of a node, follow the appropriate child pointer; repeat until leaf or key found. |
| **Insertion** | 1. Search to the leaf where the key belongs.<br>2. Insert key in sorted order.<br>3. If node overflows (**2t – 1** keys), split it: middle key moves up, node divided into two half‑full nodes. |
| **Diagram** *(one small illustration)* |
```
        [10 | 20]               ← root (t=2)
       /    |    \
  [2 5]  [12 15]  [22 30]      ← leaves
```

---

## Part 2 – B⁺‑Tree (Comprehensive)

### 1. Definition
A **B⁺‑Tree** is a variant of the B‑Tree where **all actual data records live in the leaf level**, and internal nodes store only **separator keys** to guide the search. Leaf nodes are linked together in a doubly‑linked list.

### 2. Difference from B‑Tree

| Feature | B‑Tree | B⁺‑Tree |
|--------|--------|----------|
| **Data location** | May reside in internal nodes **and** leaves. | Only in leaf nodes. |
| **Internal node keys** | Duplicate keys may appear in children. | Keys are *copies* of the smallest key in each right child (no duplication of actual data). |
| **Leaf ordering** | Leaves are independent. | Leaves are linked → fast **range queries** and sequential scans. |
| **Tree height** | Slightly higher (more keys per internal node). | Typically **lower** because internal nodes hold fewer keys (only separators). |

### 3. Node Structure
```
InternalNode
  keys[0 … m-2]           // m = max children = order
  children[0 … m-1]       // pointers to child nodes
  isLeaf = false

LeafNode
  keys[0 … L-1]           // up to max leaf keys (often same as m-1)
  values[0 … L-1]         // actual data (or record pointers)
  next ← pointer to next leaf (nullptr for rightmost)
  prev ← pointer to previous leaf (optional)
  isLeaf = true
```
*Why `next`?* Enables **O(log N + k)** range scans: after locating the first key, walk the leaf list to retrieve the next *k* records without descending the tree again.

### 4. Minimum Degree / Order
- **Order (d)** = maximum number of children per internal node.
- **Maximum keys per internal node** = *d – 1*.
- **Minimum keys** = ⌈*d / 2⌉ – 1 (except root).
- **Leaf node capacity** = *d – 1* keys (often the same as internal).

### 5. Search
1. Start at root.
2. In an internal node, perform binary search among `keys` to find the child interval.
3. Follow `children[i]`.
4. Repeat until reaching a leaf.
5. Linear scan leaf's keys (max *d – 1*) to locate the exact key or report “not found”.

Complexity: **O(log₍d₎ N)** disk accesses, plus a small linear scan in the leaf.

### 6. Insert – Step‑by‑Step
Assume order *d = 4* (max 3 keys per node).

```
Insert(key, value):
1. Find leaf L where key belongs (search).
2. Insert key/value into L keeping keys sorted.
3. If L now has ≤ d-1 keys → DONE.
4. Else L overflows (d keys). Split leaf:
   a) Create new leaf L2.
   b) Distribute keys: first ⌈d/2⌉ keys stay in L, rest move to L2.
   c) L.next points to L2; L2.prev points to L (maintain linked list).
   d) Promote the first key of L2 (call it Kpromo) to the parent.
5. Insert Kpromo into parent internal node (as a separator).
6. If parent overflows → recursively split internal node (see below).
7. If root overflows, create new root with two children.
```

#### Splitting a Leaf Node (visual)
Before insertion (`d=4`):
```
Leaf L: [10 20 30]   next → NIL
```
Insert **25** → overflow (4 keys).
After split:
```
L : [10 20]   next → L2
L2: [25 30]   next → NIL
```
Promotion key = **25** (the first key of L2).

#### Splitting an Internal Node
```
Internal P (order 4, max 3 keys):
[5 | 15 | 25]  children → C0 C1 C2 C3
```
Insert promoted key **12** → overflow (4 keys).
Split steps:
1. New internal node **P2** created.
2. Distribute keys: left keeps ⌈4/2⌉ = 2 keys `[5 | 12]`, right gets `[15 | 25]`.
3. Middle key **15** is **promoted** to P’s parent (or becomes new root).
4. Children are re‑assigned accordingly.

Visualization:
```
Before split (parent = R):
    R
    |
   [5 | 12 | 15 | 25]    ← overflow
```
After split:
```
          R
        /   \
   [5 | 12]   [25]       ← children
   (P)          (P2)
Promoted key = 15 goes up to R as a separator.
```
**Promotion rule:** The *middle* key (⌈(max+1)/2⌉‑th) moves up because it separates the two halves; it is not duplicated in leaves (leaves keep original keys).

### 7. Range Queries
1. Search for the *leftmost* key of the range → leaf L.
2. Walk the leaf linked list, output keys/values until the right bound is passed.
**Why B⁺‑Tree shines?** Because all data lives in leaves and leaves are sequentially linked, range scans involve **only leaf‑level I/Os**, no repeated tree traversals.

### 8. Time‑Complexity Summary
| Operation | Complexity (disk I/Os) |
|-----------|------------------------|
| Search    | O(log₍d₎ N) |
| Insert    | O(log₍d₎ N) (splits may propagate up, still logarithmic) |
| Delete*   | O(log₍d₎ N) (not covered in depth here) |
| Range Scan| O(log₍d₎ N + k) where *k* = number of retrieved records |

---

## Part 3 – Implementation Guidance (C++)

### 1. Design Plan
| Component | Reason |
|-----------|--------|
| **Node struct** (template‑free) | Simpler for exams; use `std::vector` for dynamic size (easy to print) or fixed‑size array for deterministic memory. |
| **Tree class** | Encapsulates order `d`, root pointer, and public APIs (`search`, `insert`, printing). |
| **Leaf linking** | `Node* next` in leaf nodes; no `prev` needed for basic range queries. |
| **Memory management** | Use `new`/`delete` manually (avoid STL containers for pointers). |

**Choice:** `std::vector<int>` for keys/values – gives automatic resizing, convenient for printing, and is allowed in most exam environments. Fixed‑size arrays would need manual shifting; vectors keep code short and clear.

### 2. Node Structure
```cpp
struct Node {
    bool isLeaf;                     // true → leaf node
    std::vector<int> keys;           // stored keys (max order-1)
    std::vector<int> values;         // only valid if isLeaf == true
    std::vector<Node*> children;     // child pointers (size = keys+1) – only for internal nodes
    Node* next;                      // leaf‑level linked list (nullptr for rightmost)
    Node(bool leaf) : isLeaf(leaf), next(nullptr) {}
};
```
*Explanation*: keys are always sorted. For leaves, `children` stays empty; `values` stores payloads (e.g., record IDs). `next` creates the leaf chain.

### 3. Pseudocode
#### Search
```
search(node, key):
    i = first index where key <= keys[i]   // binary search or linear (small)
    if node.isLeaf:
         if i < keys.size && keys[i] == key
                return values[i]
         else return NOT_FOUND
    else
         return search(children[i], key)
```
#### Insert (public)
```
insert(key, value):
    if root is null:
        root = new Node(true)
        root.keys.push_back(key)
        root.values.push_back(value)
        return
    if root.keys.size == maxKeys:
        // root full → split
        Node* newRoot = new Node(false)
        newRoot->children.push_back(root)
        splitChild(newRoot, 0)        // split old root
        root = newRoot
    insertNonFull(root, key, value)
```
#### insertNonFull(node, key, value)
```
if node.isLeaf:
    // insert key/value in sorted order
    pos = lower_bound(node.keys, key)
    node.keys.insert(pos, key)
    node.values.insert(pos, value)
    return
else:
    // find child to descend
    i = upper_bound(node.keys, key)   // child index
    if node.children[i].keys.size == maxKeys:
         splitChild(node, i)
         // after split, decide which of the two children to descend
         if key > node.keys[i]   i++   // key belongs to right split
    insertNonFull(node->children[i], key, value)
```
#### splitChild(parent, idx) – works for both leaf & internal
```
child = parent->children[idx]
newNode = new Node(child->isLeaf)
mid = order / 2   // integer division, e.g., order=4 → mid=2
// move second half keys (and values/children) to newNode
newNode->keys = child->keys[mid … end]
child->keys.erase(mid … end)
if child.isLeaf:
    newNode->values = child->values[mid … end]
    child->values.erase(mid … end)
    // link leaf list
    newNode->next = child->next
    child->next = newNode
    // promotion key = first key of newNode
    promo = newNode->keys[0]
else:
    // internal node: also move corresponding children
    newNode->children = child->children[mid+1 … end]
    child->children.erase(mid+1 … end)
    // promotion key = child's median (removed from child)
    promo = child->keys[mid]
    child->keys.erase(mid)
parent->keys.insert(idx, promo)
parent->children.insert(idx+1, newNode)
```
### 4. Full C++ Implementation
```cpp
// ----------------------  B+ Tree (exam version) ----------------------
#include <iostream>
#include <vector>
#include <algorithm>

class BPlusTree {
    struct Node {
        bool leaf;
        std::vector<int> keys;          // sorted
        std::vector<int> values;        // only for leaf
        std::vector<Node*> child;       // child pointers (size = keys+1)
        Node* next;                     // leaf linked list
        Node(bool isLeaf) : leaf(isLeaf), next(nullptr) {}
    };

    Node* root;
    const int order;                    // max children per internal node
    const int maxKeys;                  // order-1 (max keys in any node)

public:
    BPlusTree(int ord = 4) : root(nullptr), order(ord), maxKeys(ord - 1) {}

    // ---------- SEARCH ----------
    int search(int key) const {
        Node* cur = root;
        while (cur) {
            size_t i = 0;
            while (i < cur->keys.size() && key > cur->keys[i]) ++i;
            if (cur->leaf) {
                if (i < cur->keys.size() && cur->keys[i] == key)
                    return cur->values[i];
                return -1;               // NOT FOUND
            }
            cur = cur->child[i];
        }
        return -1;
    }

    // ---------- INSERT ----------
    void insert(int key, int value) {
        if (!root) {
            root = new Node(true);
            root->keys.push_back(key);
            root->values.push_back(value);
            return;
        }
        if ((int)root->keys.size() == maxKeys) {
            Node* newRoot = new Node(false);
            newRoot->child.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }
        insertNonFull(root, key, value);
    }

    // ---------- PRINT (level‑order) ----------
    void printTree() const {
        if (!root) { std::cout << "(empty)\n"; return; }
        std::vector<Node*> cur{root};
        while (!cur.empty()) {
            std::vector<Node*> next;
            for (Node* n : cur) {
                std::cout << "[";
                for (size_t i = 0; i < n->keys.size(); ++i) {
                    std::cout << n->keys[i];
                    if (i + 1 != n->keys.size()) std::cout << " ";
                }
                std::cout << "] ";
                if (!n->leaf) {
                    for (Node* c : n->child) next.push_back(c);
                }
            }
            std::cout << "\n";
            cur.swap(next);
        }
    }

    // ---------- PRINT LEAF LIST ----------
    void printLeaves() const {
        Node* p = root;
        while (p && !p->leaf) p = p->child.front();
        while (p) {
            for (size_t i = 0; i < p->keys.size(); ++i)
                std::cout << "(" << p->keys[i] << "," << p->values[i] << ") ";
            p = p->next;
        }
        std::cout << "\n";
    }

private:
    // ---------- INSERT NON‑FULL ----------
    void insertNonFull(Node* node, int key, int value) {
        size_t i = node->keys.size();
        if (node->leaf) {
            // insert in leaf keeping order
            i = 0;
            while (i < node->keys.size() && key > node->keys[i]) ++i;
            node->keys.insert(node->keys.begin() + i, key);
            node->values.insert(node->values.begin() + i, value);
            return;
        }

        // internal node: find child
        i = 0;
        while (i < node->keys.size() && key > node->keys[i]) ++i;
        Node* child = node->child[i];
        if ((int)child->keys.size() == maxKeys) {
            splitChild(node, i);
            // after split, decide which child to descend
            if (key > node->keys[i]) ++i;
        }
        insertNonFull(node->child[i], key, value);
    }

    // ---------- SPLIT CHILD ----------
    void splitChild(Node* parent, size_t idx) {
        Node* child = parent->child[idx];
        Node* newNode = new Node(child->leaf);

        int mid = order / 2;                 // integer division
        // move second half keys (and payload) to newNode
        newNode->keys.assign(child->keys.begin() + mid, child->keys.end());
        child->keys.erase(child->keys.begin() + mid, child->keys.end());

        if (child->leaf) {
            newNode->values.assign(child->values.begin() + mid, child->values.end());
            child->values.erase(child->values.begin() + mid, child->values.end());

            // maintain leaf linked list
            newNode->next = child->next;
            child->next = newNode;

            // promotion key is first key of newNode
            int promo = newNode->keys.front();
            parent->keys.insert(parent->keys.begin() + idx, promo);
        } else {
            // internal node: also move child pointers
            newNode->child.assign(child->child.begin() + mid + 1, child->child.end());
            child->child.erase(child->child.begin() + mid + 1, child->child.end());

            // promotion key = child's median (removed from child)
            int promo = child->keys[mid];
            child->keys.erase(child->keys.begin() + mid);
            parent->keys.insert(parent->keys.begin() + idx, promo);
        }

        // insert the new sibling into parent
        parent->child.insert(parent->child.begin() + idx + 1, newNode);
    }
};
// -----------------------------------------------------------------
```

### 5. Sample Usage (for testing)
```cpp
int main() {
    BPlusTree tree(4);                 // order = 4 → max 3 keys per node
    std::vector<int> seq = {10,20,5,6,12,30,7,17};

    for (int k : seq) tree.insert(k, k*10);   // value = dummy payload

    std::cout << "Level‑order view:\n";
    tree.printTree();

    std::cout << "\nLeaf list (key,value):\n";
    tree.printLeaves();

    int q = 12;
    std::cout << "\nSearch " << q << " -> " << tree.search(q) << "\n";
}
```

---

## Part 4 – Lab Exam Preparation Questions

### A) Conceptual Questions
| # | Question | Expected short answer |
|---|----------|------------------------|
| A1 | List three fundamental differences between a B‑Tree and a B⁺‑Tree. | Data stored only in leaves; internal nodes contain only separator keys; leaves are linked for sequential scans. |
| A2 | Why does a leaf‑linked list make range queries O(log N + k) instead of O(k·log N)? | After one tree search (log N), subsequent keys are read by walking the leaf list, requiring only O(k) I/Os. |
| A3 | What is the “promotion key” when splitting an internal node and why is it chosen? | The median key (⌈(max+1)/2⌉‑th) moves up because it cleanly separates the two halves and keeps ordering. |
| A4 | Explain why the minimum number of keys in a non‑root node is ⌈order/2⌉ − 1. | Guarantees that after any split each child gets at least half the keys, preventing the tree from becoming too shallow and keeping height bounded. |
| A5 | In a B⁺‑Tree, can an internal node ever store duplicate keys that also appear in leaves? | No – internal keys are only copies for navigation; the real data duplicates exist only in leaves. |

### B) Dry‑Run (Insertion) Questions
| # | Question | Hint / Expected Diagram |
|---|----------|--------------------------|
| B1 | Insert the sequence **[15, 5, 20, 25, 10, 30]** into an empty B⁺‑Tree of order 4. Draw the tree after each insertion. | Show leaf splits after the 4th insertion (25) and root split after inserting 30. |
| B2 | When inserting **27** into the tree obtained from B1, which key is promoted from the leaf split and why? | The first key of the newly created right leaf (the smallest key that moved right) – here **27** itself becomes the promotion key. |
| B3 | Show the internal node split that occurs when a leaf promotion causes the root to overflow. Indicate the new root and its two children. | New root contains the promoted median key; its two children are the former root’s left and right halves. |

### C) Code‑Modification / “What‑If” Questions
| # | Scenario | Expected answer / guidance |
|---|----------|---------------------------|
| C1 | **Change** the order from 4 to 6 (max 5 keys). Which lines of the implementation must be updated? | Only the constructor call (`BPlusTree tree(6);`). `maxKeys` is derived automatically, no other code changes. |
| C2 | **Alter** the promotion rule to always promote the **rightmost** key of the split leaf. What impact does this have on range queries? | The leaf list is still ordered, but the promotion key no longer equals the first key of the right leaf, breaking the guarantee that a search can stop at the promoted key; range scans may need to check the predecessor leaf, adding complexity. |
| C3 | **Allow duplicate keys** in the tree (multiple values per key). How would you modify `Node` and the insert routine? | Store `std::vector<std::vector<int>> valuesPerKey` or a `std::vector<std::pair<int, std::vector<int>>>`. In `insertNonFull`, if `keys[i] == key` simply `values[i].push_back(value)` instead of creating a new leaf entry. |
| C4 | **Implement delete** for a key (partial). What steps are required after physically removing the key from a leaf? | After removal, if the leaf has <⌈order/2⌉ − 1 keys, try borrowing from left/right sibling; if borrowing fails, merge with a sibling and delete the separator key from the parent (recursively propagate). |
| C5 | **Bias splits to the left** (keep ⌈order/2⌉ keys in the **right** leaf). Explain the effect on tree height and on the promotion key. | Height unchanged; promotion key becomes the smallest key of the **left** leaf instead of the right, flipping the usual invariant. |
| C6 | **Remove the leaf `next` pointer** entirely. How would range queries be implemented, and what performance penalty occurs? | Each successive key would require a fresh tree search → O(k·log N) instead of O(k). The sequential scan advantage disappears. |
| C7 | **Store values only in internal nodes** (like a classic B‑Tree). What code changes are needed and why does this defeat the B⁺‑Tree’s purpose? | Delete `values` and `next` from `Node`, move value storage to internal nodes, adjust `search`/`insert` to handle internal data. This eliminates the fast leaf‑level sequential scan, making range queries as expensive as point queries. |

---

**Common Pitfalls to Avoid**
- Promoting the *median* key from a leaf (instead of the first key of the new right leaf). 
- Forgetting to update the `next` pointer after a leaf split. 
- Using the wrong child index after a split (must re‑compare key with the newly inserted separator). 
- Not handling root overflow (must create a new root). 
- Mixing up `order` vs. `maxKeys` in calculations.

Good luck on your lab exam – you now have both the theory and a clean, exam‑ready C++ implementation!   

---
