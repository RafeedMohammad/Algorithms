/*

    3. a) Implement Hashing using the following hash function:
    i. Mid-square method
    ii. Folding method


    b) Implement the Chaining technique to solve the collision for the above.

*/

#include <iostream>
using namespace std;

struct Node
{
    int key;
    Node *next;
};

class HashTable
{
private:
    int size;
    Node **table;

public:
    HashTable(int s)
    {
        size = s;
        table = new Node *[size];

        for (int i = 0; i < size; i++)
        {
            table[i] = NULL;
        }
    }

    // --------Mid Square Method-----------
    int midSquareHash(int key)
    {
        long long sq = 1LL * key * key;
        string s = to_string(sq);

        int len = s.length();
        int mid = len / 2;

        string midDigits;

        if (len >= 2)
        {
            if (mid == 0)
            {
                midDigits = 5;
            }

            else
            {
                midDigits = s.substr(mid - 1, 2);
            }
        }

        else
        {
            midDigits = 5;
        }

        int midVal = stoi(midDigits);
        return midVal % size;
    }
};
