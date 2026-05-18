/*

    3. a) Implement Hashing using the following hash function:
    i. Mid-square method
    ii. Folding method


    b) Implement the Chaining technique to solve the collision for the above.

*/

#include <iostream>
using namespace std;

enum HashMethod
{
    MID_SQUARE,
    FOLDING
};

struct Node
{
    int key;
    Node *next;
    Node(int k) : key(k), next(nullptr) {}
};

class HashTable
{
private:
    int size;
    int foldSize;
    HashMethod method;
    Node **table; // table - pointer to first element of array, each array elements - Node*

    // --------Mid Square Method-----------
    int midSquareHash(int key)
    {
        long long sq = 1LL * key * key; // 1LL - to avoid overflow
        string s = to_string(sq);
        int len = s.length();

        int start = (len - 2) / 2; // mid position

        string midDigits;
        if (len >= 2)
        {
            midDigits = s.substr(start, 2);
        }
        else
        {
            midDigits = s;
        }

        return stoi(midDigits) % size; // stoi converts string into integer
    }

    //-----------Folding Method--------------
    int foldingHash(int key)
    {
        string s = to_string(key);
        int sum = 0, i = 0;

        while (i < (int)s.length())
        {
            sum += stoi(s.substr(i, foldSize));
            i += foldSize;
        }

        return sum % size;
    }

    //------------Method selection---------------
    int hash(int key)
    {
        return (method == MID_SQUARE)
                   ? midSquareHash(key)
                   : foldingHash(key);
    }

public:
    HashTable(int s, HashMethod m, int fs = 2)
    {
        size = s;
        method = m;
        foldSize = fs;
        table = new Node *[size];

        for (int i = 0; i < size; i++)
        {
            table[i] = nullptr;
        }
    }

    ~HashTable()
    {
        for (int i = 0; i < size; i++)
        {
            Node *curr = table[i];
            while (curr)
            {
                Node *tmp = curr;
                curr = curr->next;
                delete tmp;
            }
        }

        delete[] table;
    }

    void insert(int key)
    {
        int idx = hash(key);
        Node *newNode = new Node(key);
        newNode->next = table[idx];
        table[idx] = newNode;
        cout << "inserted " << key << " -> index " << idx << endl;
    }

    bool search(int key)
    {
        int idx = hash(key);
        Node *curr = table[idx];
        while (curr)
        {
            if (curr->key == key)
            {
                return true;
            }
            curr = curr->next;
        }
        return false;
    }

    bool deleteKey(int key)
    {
        int idx = hash(key);
        Node *curr = table[idx];
        Node *prev = nullptr;

        while (curr)
        {
            if (curr->key == key)
            {
                if (prev == nullptr)
                {
                    table[idx] = curr->next;
                }

                else
                {
                    prev->next = curr->next;
                }

                delete curr;
                cout << "Deleted " << key << " from index " << idx << endl;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }

        return 0;
    }

    void display()
    {
        for (int i = 0; i < size; i++)
        {
            cout << " [" << i << "] -> ";
            Node *curr = table[i];
            while (curr)
            {
                cout << curr->key << "->";
                curr = curr->next;
            }

            cout << "NULL" << endl;
        }
    }

    void searchAndPrint(int key)
    {
        cout << "Search " << key << ": " << (search(key) ? "found" : "not found") << endl;
    }
};

int main()
{
    int keys[] = {79, 25, 36, 123, 456, 79};
    cout << "=== Mid-Square Hashing (size=10) ===\n";
    HashTable ms(10, MID_SQUARE);
    for (int k : keys)
        ms.insert(k);
    cout << "\nHash Table:\n";
    ms.display();

    // cout << "\nSearch 36: " << (ms.search(36) ? "found" : "not found") << endl;
    // cout << "Search 123: " << (ms.search(123) ? "found" : "not found") << endl;

    ms.searchAndPrint(36);
    ms.searchAndPrint(125);
    cout << endl;

    ms.deleteKey(456);
    cout << "After deletion operation" << endl;
    ms.display();
    cout << endl;

    cout << "=== Folding Method (size=10, foldSize=2) ===\n";
    HashTable f1(10, FOLDING, 2);
    for (int k : keys)
    {
        f1.insert(k);
    }

    cout << "\nHash Table: " << endl;
    f1.display();

    return 0;
}
