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
};
