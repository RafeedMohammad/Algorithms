#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class DataNode
{
private:
    vector<T> keys;
    int maxCapacity;

public:
    DataNode(int capacity) : maxCapacity(capacity)
    {
        keys.reserve(maxCapacity);
    }

    void insert(T key)
    {
        if (keys.size() < maxCapacity)
        {
            keys.push_back(key);
            cout << "Inserted: " << key << endl;
        }

        else
        {
            cout << "Node is full" << endl;
        }
    }

    void display()
    {
        cout << "[";
        for (const auto &k : keys)
        {
            cout << k << " ";
        }

        cout << "]" << endl;
    }
};

int main()
{
    DataNode<int> intNode(3);
    intNode.insert(10);
    intNode.insert(20);
    intNode.display();

    DataNode<string> nameNode(2);
    nameNode.insert("Dhaka");
    nameNode.insert("Chattagram");
    nameNode.display();

    return 0;
}
