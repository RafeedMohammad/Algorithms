/*
Basic terminologies:
Support: The frequency of an itemset in the dataset
Confidence: The probability that an item appears in a transaction given the presence of another item
Lift: The strength of the association, indicating whether items are bought together more often than expected


Apriori Algorithm Steps
 1. Generate candidate item sets - Identifying individual items and counting their occurrences to determine frequent items
 2. Prune item sets - items that appear less than the minimum support threshold are removed
 3. Form frequent item sets - generates larger itemsets by combining frequent smaller itemsets, iterating until no more frequent itemsets can be found
 4. Derive association rules - extracts rules based on confidence and lift values to determine relationships
*/

#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <map>
using namespace std;

// Defined Interfaces
vector<set<string>> readTransactions(int n);
void printTransactions(const vector<set<string>> &transactions);

map<string, int> countSingleItemSupport(const vector<set<string>> &transactions);
void printSupportCount(const map<string, int> &supportCount);

set<set<string>> generateL1(const map<string, int> &supportCount, int minSupport);
void printFrequentItemsets(const set<set<string>> &L);

int main()
{
    int n; // no. of transactions
    cout << "Enter number of transactions:";
    cin >> n;
    cin.ignore(); // clear newline

    vector<set<string>> transactions = readTransactions(n);

    printTransactions(transactions);

    map<string, int> supportCount = countSingleItemSupport(transactions);
    printSupportCount(supportCount);

    int minSupport;
    cout << "Enter minimum support: ";
    cin >> minSupport;

    set<set<string>> L1 = generateL1(supportCount, minSupport);

    cout << "\nFrequent 1-itemsets (L1):" << endl;
    printFrequentItemsets(L1);

    return 0;
}

// Function to read transactions
vector<set<string>> readTransactions(int n)
{
    vector<set<string>> transactions;
    cout << "Enter each transactions (space separated items): " << endl;

    for (int i = 0; i < n; i++)
    {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string item;
        set<string> trans;

        while (ss >> item)
        {
            trans.insert(item);
        }

        transactions.push_back(trans);
    }

    return transactions;
}

// Function to print transactions
void printTransactions(const vector<set<string>> &transactions)
{
    cout << "Stored transactions:" << endl;

    for (int i = 0; i < transactions.size(); i++)
    {
        cout << "T" << i + 1 << ": { ";
        for (auto item : transactions[i])
        {
            cout << item << " ";
        }
        cout << "}";

        cout << endl;
    }
}

map<string, int> countSingleItemSupport(const vector<set<string>> &transactions)
{
    map<string, int> supportCount;

    for (auto &trans : transactions)
    {
        for (auto &item : trans)
        {
            supportCount[item]++;
        }
    }

    return supportCount;
}

// Function to print supoort counts
void printSupportCount(const map<string, int> &supportCount)
{
    cout << "\nSupport count of 1-itemsets:" << endl;

    for (auto &entry : supportCount)
    {
        cout << entry.first << " -> " << entry.second << endl;
    }
}

set<set<string>> generateL1(const map<string, int> &supportCount, int minSupport)
{
    set<set<string>> L1;

    for (auto &entry : supportCount)
    {
        if (entry.second >= minSupport)
        {
            set<string> itemset;
            itemset.insert(entry.first);
            L1.insert(itemset);
        }
    }

    return L1;
}

void printFrequentItemsets(const set<set<string>> &L)
{
    for (auto &itemset : L)
    {
        cout << "{ ";
        for (auto &item : itemset)
        {
            cout << item << " }";
        }

        cout << endl;
    }
}