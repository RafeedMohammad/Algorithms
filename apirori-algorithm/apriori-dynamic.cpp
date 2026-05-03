#include <iostream>
#include <vector>
#include <set>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

// Function Prototypes

// reads n transactions
vector<set<string>> readTransactions(int n);

// prints all transactions
void printTransactions(const vector<set<string>> &transactions);

// goes through all transactions and calculates frequency of each items
map<string, int> countSingleItemSupport(const vector<set<string>> &transactions);

// prints frequency of each count
void printSupportCount(const map<string, int> &supportCount);

// filters items based on minimum support (minSupport) - set of sets
set<set<string>> generateL1(const map<string, int> &supportCount, int minSupport);

// prints any level of frequent itemsets
void printItemsets(const set<set<string>> &L);

/*
    Takes previous frequent itemsets(Lk-1)
    Joins them to form k-item candidates (Ck)
*/
set<set<string>> generateCandidates(const set<set<string>> &prevL, int k);

/*
    Checks each transaction
    Joins them to form k-item candidates (Ck)
*/
map<set<string>, int> countCandidateSupport(
    const vector<set<string>> &transactions,
    const set<set<string>> &candidates);

void printCandidateSupport(const map<set<string>, int> &candidateSupport);

/*
    Removes itemsets whose support < minSupport
    Keeps only frequent itemsets (Lk)
*/
set<set<string>> pruneCandidates(
    const map<set<string>, int> &candidateSupport,
    int minSupport);

// Generates all possible subsets of an itemset
vector<set<string>> generateSubsets(const set<string> &itemset);

/*
Global Support Storage
    Stores support for every frequent itemset
    Used later for confidence calculation
*/
map<set<string>, int> allSupport;

// to generate association rules
/*
    1. Generate all subsets
    2. Split into:
        - Left Side (X)
        - Right Side (Y = L-X)
    3. Compute confidence
        confidence = support(L) / support(X)
    4. if confidence >= minConfidence: keep
*/
void generateAssociationRules(int totalTransactions, double minConfidence);

int main()
{
    int n;
    cout << "Enter number of transactions: ";
    cin >> n;
    cin.ignore();

    vector<set<string>> transactions = readTransactions(n);
    printTransactions(transactions);

    // Step 1: Count 1-item supports
    map<string, int> supportCount = countSingleItemSupport(transactions);
    printSupportCount(supportCount);

    for (auto &entry : supportCount)
    {
        set<string> s;
        s.insert(entry.first);
        allSupport[s] = entry.second;
    }

    int minSupport;
    cout << "\nEnter minimum support: ";
    cin >> minSupport;

    // Step 2: Generate L1
    set<set<string>> L = generateL1(supportCount, minSupport);

    cout << "\nFrequent 1-itemsets (L1):\n";
    printItemsets(L);

    // Step 3 onwards: Loop for C2, C3, C4... until stop
    int k = 1;

    while (!L.empty())
    {
        int nextK = k + 1;

        // Generate candidates
        set<set<string>> C = generateCandidates(L, nextK);

        if (C.empty())
            break;

        cout << "\nCandidate " << nextK << "-itemsets (C" << nextK << "):\n";
        printItemsets(C);

        // Count support
        map<set<string>, int> candSupport = countCandidateSupport(transactions, C);

        for (auto &entry : candSupport)
        {
            allSupport[entry.first] = entry.second;
        }

        cout << "\nSupport count of C" << nextK << " candidates:\n";
        printCandidateSupport(candSupport);

        // Prune to get frequent itemsets
        set<set<string>> Lnew = pruneCandidates(candSupport, minSupport);

        if (Lnew.empty())
            break;

        cout << "\nFrequent " << nextK << "-itemsets (L" << nextK << "):\n";
        printItemsets(Lnew);

        // Move forward
        L = Lnew;
        k = nextK;

        // Subset Testing
        //  set<string> test = {"A", "B", "C"};
        //  vector<set<string>> subs = generateSubsets(test);

        // cout << "\nSubsets of {A,B,C}:\n";
        // for (auto &s : subs)
        // {
        //     cout << "{ ";
        //     for (auto &x : s)
        //         cout << x << " ";
        //     cout << "}\n";
        // }
    }

    double minConfidence;
    cout << "\nEnter minimum confidence (0 to 1): ";
    cin >> minConfidence;

    generateAssociationRules(n, minConfidence);

    return 0;
}

// ------------------ Functions ------------------

// Read transactions
vector<set<string>> readTransactions(int n)
{
    vector<set<string>> transactions;
    cout << "Enter each transaction (space separated items):\n";

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

// Print transactions
void printTransactions(const vector<set<string>> &transactions)
{
    cout << "\nStored transactions:\n";

    for (int i = 0; i < transactions.size(); i++)
    {
        cout << "T" << i + 1 << ": { ";
        for (auto item : transactions[i])
        {
            cout << item << " ";
        }
        cout << "}\n";
    }
}

// Count support for single items
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

// Print support counts
void printSupportCount(const map<string, int> &supportCount)
{
    cout << "\nSupport count of 1-itemsets:\n";
    for (auto &entry : supportCount)
    {
        cout << entry.first << " -> " << entry.second << endl;
    }
}

// Generate frequent 1-itemsets (L1)
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

// Print itemsets
void printItemsets(const set<set<string>> &L)
{
    for (auto &itemset : L)
    {
        cout << "{ ";
        for (auto &item : itemset)
        {
            cout << item << " ";
        }
        cout << "}\n";
    }
}

// Generate candidates (general)
set<set<string>> generateCandidates(const set<set<string>> &prevL, int k)
{
    set<set<string>> candidates;
    vector<set<string>> prevList(prevL.begin(), prevL.end());

    for (int i = 0; i < prevList.size(); i++)
    {
        for (int j = i + 1; j < prevList.size(); j++)
        {
            set<string> candidate;

            set_union(prevList[i].begin(), prevList[i].end(),
                      prevList[j].begin(), prevList[j].end(),
                      inserter(candidate, candidate.begin()));

            if (candidate.size() == k)
            {
                candidates.insert(candidate);
            }
        }
    }

    return candidates;
}

// Count support of candidates
map<set<string>, int> countCandidateSupport(
    const vector<set<string>> &transactions,
    const set<set<string>> &candidates)
{
    map<set<string>, int> candidateSupport;

    for (auto &cand : candidates)
    {
        int count = 0;

        for (auto &trans : transactions)
        {
            if (includes(trans.begin(), trans.end(),
                         cand.begin(), cand.end()))
            {
                count++;
            }
        }

        candidateSupport[cand] = count;
    }

    return candidateSupport;
}

// Print candidate supports
void printCandidateSupport(const map<set<string>, int> &candidateSupport)
{
    for (auto &entry : candidateSupport)
    {
        cout << "{ ";
        for (auto &item : entry.first)
        {
            cout << item << " ";
        }
        cout << "} -> " << entry.second << endl;
    }
}

// Prune candidates -> Frequent itemsets
set<set<string>> pruneCandidates(
    const map<set<string>, int> &candidateSupport,
    int minSupport)
{
    set<set<string>> frequent;

    for (auto &entry : candidateSupport)
    {
        if (entry.second >= minSupport)
        {
            frequent.insert(entry.first);
        }
    }

    return frequent;
}

vector<set<string>> generateSubsets(const set<string> &itemset)
{
    vector<string> items(itemset.begin(), itemset.end());
    int n = items.size();

    vector<set<string>> subsets;

    // masks from 1 to (2^n - 2)
    for (int mask = 1; mask < (1 << n) - 1; mask++)
    {
        set<string> subset;

        for (int i = 0; i < n; i++)
        {
            if (mask & (1 << i))
            {
                subset.insert(items[i]);
            }
        }

        subsets.push_back(subset);
    }

    return subsets;
}

void generateAssociationRules(int totalTransactions, double minConfidence)
{
    cout << "\n========== ASSOCIATION RULES ==========\n";

    for (auto &entry : allSupport)
    {
        set<string> itemset = entry.first;

        if (itemset.size() < 2)
            continue;

        int supXY = entry.second;

        vector<set<string>> subsets = generateSubsets(itemset);

        for (auto &X : subsets)
        {
            set<string> Y;

            set_difference(itemset.begin(), itemset.end(),
                           X.begin(), X.end(),
                           inserter(Y, Y.begin()));

            // X and Y must exist in support table
            if (allSupport.find(X) == allSupport.end())
                continue;
            if (allSupport.find(Y) == allSupport.end())
                continue;

            int supX = allSupport[X];
            int supY = allSupport[Y];

            double confidence = (double)supXY / supX;
            double support = (double)supXY / totalTransactions;
            double lift = confidence / ((double)supY / totalTransactions);

            if (confidence >= minConfidence)
            {
                cout << "{ ";
                for (auto &x : X)
                    cout << x << " ";
                cout << "} -> { ";
                for (auto &y : Y)
                    cout << y << " ";
                cout << "}";

                cout << "  Support: " << support;
                cout << "  Confidence: " << confidence;
                cout << "  Lift: " << lift << endl;
            }
        }
    }
}