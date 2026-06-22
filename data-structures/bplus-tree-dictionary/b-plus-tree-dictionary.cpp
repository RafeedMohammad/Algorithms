#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

#include "data_parsers.cpp"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

template <typename Key, typename Value>
class BPlusTree
{
    struct Node
    {
        bool leaf;
        vector<Key> keys;     // sorted keys
        vector<Value> values; // only meaningful for leaf nodes
        vector<Node *> child; // child pointers (size = keys.size() + 1 for internal)
        Node *next;           // leaf-level linked list
        Node(bool isLeaf)
            : leaf(isLeaf), next(nullptr)
        {
        }
    };

    Node *root;
    const int order;   // max children per internal node
    const int maxKeys; // order - 1 (max keys in any node)

public:
    BPlusTree(int ord = 4) : root(nullptr), order(ord), maxKeys(ord - 1) {}

    // ─── SEARCH ──────────────────────────────────────────────
    bool search(const Key &key, Value &value) const
    {
        Node *cur = root;
        while (cur)
        {
            int i = 0;
            if (cur->leaf)
            {
                while (i < (int)cur->keys.size() && key > cur->keys[i])
                    ++i;
                if (i < (int)cur->keys.size() && cur->keys[i] == key)
                {
                    value = cur->values[i];
                    return true;
                }
                return false;
            }

            while (i < (int)cur->keys.size() && key >= cur->keys[i])
                ++i;

            cur = cur->child[i];
        }

        return false;
    }

    void countLeaves()
    {
        if (!root)
        {
            cout << "Total Leaves: 0";
            return;
        }
        int leafCount = 0;
        Node *temp = root;
        while (temp->leaf == false)
        {
            temp = temp->child[0];
        }
        while (temp)
        {
            leafCount++;
            temp = temp->next;
        }
        cout << "Total Leaves: " << leafCount;
    }

    void rangeSearch(const Key &low, const Key &high) const
    {
        if (!root)
            return;

        Node *cur = root;

        while (!cur->leaf)
        {
            int i = 0;
            while (i < (int)cur->keys.size() && low >= cur->keys[i])
                ++i;
            cur = cur->child[i];
        }

        while (cur)
        {
            for (int i = 0; i < (int)cur->keys.size(); i++)
            {
                if (cur->keys[i] >= low && cur->keys[i] <= high)
                    cout << "(" << cur->keys[i] << " -> " << cur->values[i] << ") ";

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
    void insert(const Key &key, const Value &value)
    {
        if (!root)
        {
            root = new Node(true);
            root->keys.push_back(key);
            root->values.push_back(value);
            return;
        }

        // Pitfall: split a FULL root before descending (root has no parent)
        // Pitfall: trigger is == maxKeys, not > maxKeys
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
        int j = 0;

        while (!level.empty())
        {
            cout << j++;
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
    void insertNonFull(Node *node, const Key &key, const Value &value)
    {
        if (node->leaf)
        {
            int i = 0;
            while (i < (int)node->keys.size() && key > node->keys[i])
                ++i;

            if (i < (int)node->keys.size() && node->keys[i] == key)
            {
                node->values[i] = value;
                return;
            }

            node->keys.insert(node->keys.begin() + i, key);
            node->values.insert(node->values.begin() + i, value);
            return;
        }

        int i = 0;
        while (i < (int)node->keys.size() && key >= node->keys[i])
            ++i;

        if ((int)node->child[i]->keys.size() == maxKeys)
        {
            splitChild(node, i);
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

            int mid = child->keys.size() / 2; // maxKeys divided by 2 is safer mid (for odd orders)

            sibling->keys.assign(child->keys.begin() + mid, child->keys.end());
            child->keys.erase(child->keys.begin() + mid, child->keys.end());

            sibling->values.assign(child->values.begin() + mid, child->values.end());
            child->values.erase(child->values.begin() + mid, child->values.end());

            sibling->next = child->next;
            child->next = sibling;

            parent->keys.insert(parent->keys.begin() + idx, sibling->keys.front());
        }
        else
        {
            // ── Internal split: PUSH-UP ──────────────────────────

            int mid = child->keys.size() / 2; // = 1 for order-4 (maxKeys=3)

            Key promo = child->keys[mid];

            sibling->keys.assign(child->keys.begin() + mid + 1, child->keys.end());
            child->keys.erase(child->keys.begin() + mid, child->keys.end());

            sibling->child.assign(child->child.begin() + mid + 1, child->child.end());
            child->child.erase(child->child.begin() + mid + 1, child->child.end());

            parent->keys.insert(parent->keys.begin() + idx, promo);
        }

        // Wire the new sibling into the parent's child list
        parent->child.insert(parent->child.begin() + idx + 1, sibling);
    }
};

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    string path = "data/dict-2.json";
    int order = 64;

    if (argc >= 2)
    {
        path = argv[1];
    }
    else
    {
        std::string source_file = __FILE__;
        size_t pos = source_file.find_last_of("/\\");
        if (pos != std::string::npos)
        {
            path = source_file.substr(0, pos) + "/data/dict.json";
        }
        cout << "No dictionary path provided. Defaulting to: " << path << endl;
    }

    if (argc >= 3)
    {
        order = std::stoi(argv[2]);
    }

    cout << "Using B+ Tree order: " << order << "\n";

    BPlusTree<string, string> dictionary(order);
    vector<pair<string, string>> entries;

    if (!load_dict_json(path, entries))
    {
        cout << "Failed to load dict.json file: " << path << endl;
        return 1;
    }

    auto start_insert = std::chrono::high_resolution_clock::now();
    for (auto &entry : entries)
        dictionary.insert(entry.first, entry.second);
    auto end_insert = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> insert_ms = end_insert - start_insert;

    cout << "English-Bangla dictionary loaded from file: " << path << endl;
    // cout << "Loaded " << entries.size() << " entries." << endl;
    cout << "Insertion time: " << insert_ms.count() << " ms\n";

    // cout << "Leaf list (key, value) in sorted order:\n";
    // dictionary.printLeaves();

    string query = "saber";
    string translation;

    auto start_search = std::chrono::high_resolution_clock::now();
    bool found = dictionary.search(query, translation);
    auto end_search = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> search_ms = end_search - start_search;

    if (found)
    {
        cout << "Search result: " << query << " -> " << translation << "\n";
    }
    else
    {
        cout << "Word not found: " << query << "\n";
    }

    cout << "Search time: " << search_ms.count() << " ms\n";

    return 0;
}
