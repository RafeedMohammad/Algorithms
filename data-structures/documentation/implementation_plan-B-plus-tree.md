# Implementation Plan: B+ Tree with STL (Full Details)

This document provides the full implementation details and the logical reasoning behind the B+ Tree design using C++ STL components.

## 1. Full C++ Implementation

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * B+ Tree Implementation using C++ STL
 * 
 * DESIGN RATIONALE:
 * 1. Data-Only Leaves: All keys are duplicated or moved to leaves. Internal nodes only route.
 * 2. Singly Linked Leaves: Allows O(N) range scanning.
 * 3. STL Vectors: Replaces manual array shifting (insert/erase) and 'n' tracking.
 */

const int t = 3; // Minimum degree. MAX_KEYS = 5, MIN_KEYS = 2.

struct BPlusNode {
    bool isLeaf;
    vector<int> keys;
    vector<BPlusNode*> children; 
    BPlusNode* next;             

    BPlusNode(bool leaf) : isLeaf(leaf), next(nullptr) {}
};

class BPlusTree {
    BPlusNode* root;

    /**
     * LOGIC: splitChildRefined
     * Why split differently? 
     * In a B-Tree, the median is always removed from children and moved up.
     * In a B+ Tree Leaf, the median MUST stay in the leaf to maintain the data set.
     */
    void splitChild(BPlusNode* parent, int i) {
        BPlusNode* y = parent->children[i];
        BPlusNode* z = new BPlusNode(y->isLeaf);
        int promotedKey;

        if (y->isLeaf) {
            // Leaf Split: [10, 20, 30, 40, 50]
            // z gets [30, 40, 50], y keeps [10, 20]. Promoted: 30.
            promotedKey = y->keys[t - 1];
            z->keys.assign(y->keys.begin() + (t - 1), y->keys.end());
            y->keys.erase(y->keys.begin() + (t - 1), y->keys.end());

            // Linked List Logic: Maintain the chain of leaves
            z->next = y->next;
            y->next = z;
        } else {
            // Internal Split: [10, 20, 30, 40, 50]
            // z gets [40, 50], y keeps [10, 20]. Promoted: 30.
            promotedKey = y->keys[t - 1];
            z->keys.assign(y->keys.begin() + t, y->keys.end());
            z->children.assign(y->children.begin() + t, y->children.end());
            
            y->keys.erase(y->keys.begin() + (t - 1), y->keys.end());
            y->children.erase(y->children.begin() + t, y->children.end());
        }

        parent->children.insert(parent->children.begin() + i + 1, z);
        parent->keys.insert(parent->keys.begin() + i, promotedKey);
    }

    void insertNonFull(BPlusNode* node, int key) {
        if (node->isLeaf) {
            // Binary search for insertion spot
            auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
            node->keys.insert(it, key);
        } else {
            // Binary search for correct child routing
            auto it = upper_bound(node->keys.begin(), node->keys.end(), key);
            int i = distance(node->keys.begin(), it);

            // Pre-emptive Split: Split full children on the way down.
            // Why? It avoids the need for parent pointers and backtracking.
            if (node->children[i]->keys.size() == (2 * t - 1)) {
                splitChild(node, i);
                if (key > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

public:
    BPlusTree() : root(nullptr) {}

    void insert(int key) {
        if (root == nullptr) {
            root = new BPlusNode(true);
            root->keys.push_back(key);
            return;
        }

        if (root->keys.size() == (2 * t - 1)) {
            BPlusNode* s = new BPlusNode(false);
            s->children.push_back(root);
            splitChild(s, 0);
            int i = (key > s->keys[0]) ? 1 : 0;
            insertNonFull(s->children[i], key);
            root = s;
        } else {
            insertNonFull(root, key);
        }
    }

    void traverse() {
        if (!root) return;
        BPlusNode* curr = root;
        while (!curr->isLeaf) curr = curr->children[0];
        while (curr) {
            for (int k : curr->keys) cout << k << " ";
            curr = curr->next;
        }
        cout << endl;
    }

    bool search(int key) {
        if (!root) return false;
        BPlusNode* curr = root;
        while (!curr->isLeaf) {
            auto it = upper_bound(curr->keys.begin(), curr->keys.end(), key);
            int i = distance(curr->keys.begin(), it);
            curr = curr->children[i];
        }
        return binary_search(curr->keys.begin(), curr->keys.end(), key);
    }
};
```

---

## 2. Logical Thinking & Rationale

### Why `std::vector`?
Traditional B-Tree implementations spend 40% of their code handling array shifts (e.g., `keys[i+1] = keys[i]`). By using `std::vector`, we delegate this complexity to the C++ Standard Library.
- **Insertions**: `keys.insert(it, value)` handles the shift in one line.
- **Splits**: `z->keys.assign(y->keys.begin() + offset, y->keys.end())` handles the "slicing" of nodes cleanly.

### Why `lower_bound` vs `upper_bound`?
- **In Leaves (`lower_bound`)**: We want the first element $\ge$ target. This is useful for finding the exact key or the spot where it should be inserted.
- **In Internal Nodes (`upper_bound`)**: We want the first element $>$ target. 
    - If `keys = [10, 20]` and we search for `10`, `upper_bound` returns a pointer to `20`.
    - This corresponds to index `1`, which is the child containing values $\ge 10$. This is the correct routing logic for a B+ Tree.

### The "Pre-emptive Split" Strategy
In this implementation, we split a child node *before* recursing into it if it is already full. 
- **Logical Reason**: This ensures that when we reach a leaf, we can always insert the new key without causing an overflow that ripples back up. Without this, we would need a `parent` pointer in every node, which increases memory overhead and code complexity.

### Why Singly Linked Leaves?
The `next` pointer in `BPlusNode` allows us to "jump" to the start of the data and walk through it linearly. This turns a complex tree traversal into a simple linked list iteration, which is the primary reason B+ Trees are the industry standard for database indexing.
