#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class BPlusTree
{
    struct Node
    {
        bool leaf;
        vector<int> keys;     // sorted keys
        vector<int> values;   // only meaningful for leaf nodes
        vector<Node *> child; // child pointers (size = keys.size() + 1 for internal)
        Node *next;           // leaf-level linked list
        Node(bool isLeaf) : leaf(isLeaf), next(nullptr) {}
    };

    Node *root;
    const int order;   // max children per internal node
    const int maxKeys; // order - 1 (max keys in any node)

public:
    BPlusTree(int ord = 4) : root(nullptr), order(ord), maxKeys(ord - 1) {}

    // ─── SEARCH ──────────────────────────────────────────────
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

    void rangeSearch(int low, int high) const
    {
        if (!root)
            return;

        Node *cur = root;

        // go down to leaf
        while (!cur->leaf)
        {
            int i = 0;
            while (i < (int)cur->keys.size() && low > cur->keys[i])
                ++i;
            cur = cur->child[i];
        }

        // traverse leaf list
        while (cur)
        {
            for (int i = 0; i < (int)cur->keys.size(); i++)
            {
                if (cur->keys[i] >= low && cur->keys[i] <= high)
                    cout << "(" << cur->keys[i] << "," << cur->values[i] << ") ";

                if (cur->keys[i] > high)
                {
                    cout << "\n";
                    return;
                }
            }
            cur = cur->next;
        }
        cout << "\n";
    }

    // ─── INSERT ──────────────────────────────────────────────
    void insert(int key, int value)
    {
        if (!root)
        {
            root = new Node(true);
            root->keys.push_back(key);
            root->values.push_back(value);
            return;
        }

        // Pitfall 4: split a FULL root before descending (root has no parent)
        // Pitfall 5: trigger is == maxKeys, not > maxKeys
        if ((int)root->keys.size() == maxKeys)
        {
            Node *newRoot = new Node(false);
            newRoot->child.push_back(root);
            splitChild(newRoot, 0);
            root = newRoot;
        }

        insertNonFull(root, key, value);
    }

    // ─── PRINT (level-order) ─────────────────────────────────
    void printTree() const
    {
        if (!root)
        {
            cout << "(empty)\n";
            return;
        }

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

    // ─── PRINT LEAF LINKED LIST ──────────────────────────────
    void printLeaves() const
    {
        // Walk down to the leftmost leaf
        Node *p = root;
        while (p && !p->leaf)
            p = p->child.front();

        // Traverse the linked list
        while (p)
        {
            for (int i = 0; i < (int)p->keys.size(); ++i)
                cout << "(" << p->keys[i] << "," << p->values[i] << ") ";
            p = p->next;
        }
        cout << "\n";
    }

private:
    // ─── INSERT NON-FULL ─────────────────────────────────────
    void insertNonFull(Node *node, int key, int value)
    {
        if (node->leaf)
        {
            // Find the sorted insertion position
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

        // Pitfall 5: split when == maxKeys (full), not after overflow
        if ((int)node->child[i]->keys.size() == maxKeys)
        {
            splitChild(node, i);
            // Pitfall 3: re-check which child to descend AFTER split
            // node->keys[i] is now the newly promoted separator
            if (key > node->keys[i])
                ++i;
        }

        insertNonFull(node->child[i], key, value);
    }

    // ─── SPLIT CHILD ─────────────────────────────────────────
    void splitChild(Node *parent, int idx)
    {
        Node *child = parent->child[idx];
        Node *sibling = new Node(child->leaf);

        if (child->leaf)
        {
            // ── Leaf split: COPY-UP ──────────────────────────────
            // Pitfall 1: promote the FIRST key of the right node (copy — key stays in leaf)
            // left  keeps keys[0 .. mid-1]
            // right gets  keys[mid .. end]   ← sibling->keys.front() is promoted
            int mid = order / 2;

            sibling->keys.assign(child->keys.begin() + mid, child->keys.end());
            child->keys.erase(child->keys.begin() + mid, child->keys.end());

            sibling->values.assign(child->values.begin() + mid, child->values.end());
            child->values.erase(child->values.begin() + mid, child->values.end());

            // Pitfall 2: set sibling->next FIRST, then redirect child->next
            sibling->next = child->next;
            child->next = sibling;

            // Pitfall 1: copy-up — the promoted key is a copy, still lives in the leaf
            parent->keys.insert(parent->keys.begin() + idx, sibling->keys.front());
        }
        else
        {
            // ── Internal split: PUSH-UP ──────────────────────────
            // Pitfall 6: mid key is REMOVED from child (not kept in either half)
            // left  keeps keys[0 .. mid-1]  and children[0 .. mid]
            // promo =     keys[mid]          (pushed up, gone from child)
            // right gets  keys[mid+1 .. end] and children[mid+1 .. end]
            int mid = maxKeys / 2; // = 1 for order-4 (maxKeys=3)

            // Save promo BEFORE any erase (avoids the UB of reading after erase)
            int promo = child->keys[mid];

            // Right sibling gets keys strictly to the RIGHT of mid
            sibling->keys.assign(child->keys.begin() + mid + 1, child->keys.end());
            // Left child keeps keys strictly to the LEFT of mid; mid key is removed
            child->keys.erase(child->keys.begin() + mid, child->keys.end());

            // Mirror the child-pointer split
            sibling->child.assign(child->child.begin() + mid + 1, child->child.end());
            child->child.erase(child->child.begin() + mid + 1, child->child.end());

            // Push-up: promoted key is removed from child, lives only in parent
            parent->keys.insert(parent->keys.begin() + idx, promo);
        }

        // Wire the new sibling into the parent's child list
        parent->child.insert(parent->child.begin() + idx + 1, sibling);
    }
};

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

    tree.rangeSearch(1, 30);
}