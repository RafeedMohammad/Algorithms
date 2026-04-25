#include <iostream>
#include <vector>
using namespace std;

struct BPlusNode
{
    bool isLeaf;
    vector<int> keys;
    vector<BPlusNode> children;
    // for internal nodes
    BPlusNode *next;
    // leaf nodes
    BPlusNode(bool leaf) : isLeaf(leaf), next(nullptr) {}
};


