#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "cayley_tree.h"

using namespace std;

// a node structure
typedef struct treeNode treeNode;


/*
int rule(int parent, int present)
{
    return ((parent + present) % 2);
}
*/
void transitionRule(int R, vector<long int> &RM)
{
    int idx = 0;
    while (R)
    {
        RM[idx++] = R % 2;
        R /= 2;
    }
}

int idxN4(int self, int p, int c1, int c2)
{
    return (self * 8 + p * 4 + (c1 + c1) + c2); // self x 2^3 + p x 2^2 + c1 x 2^1 + c2 x 2^0
}

void makeiC(vector<long int> &iC, treeNode *headerCt, vector<long int> &R)
{
    vector<treeNode *> queue;
    iC.clear();

    // data to configuration array
   //here parent refers to the first child of considered root
     // (not a real parent of root, real parent of root doesn't exist)
    iC.push_back(R[idx(headerCt->data, 0)]); // added 0 becaue root has no parent so parent value will be 0
    queue.push_back(headerCt->parent);
    queue.push_back(headerCt->childOne);
    queue.push_back(headerCt->childTwo);

    while (queue[0] != nullptr)
    {
        iC.push_back(R[idx(queue[0]->data, queue[0]->parent->data)]);
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        queue.erase(queue.begin());
    }
}

// for final set of configurations
int verificationOfConfiguration(vector<long int> temp, vector<vector<long int>> &fC)
{

    int j = fC.size();
    for (long i = 0; i < j; i++)
    {
        if (temp == fC[i])
        {
            return 0;
        }
    }
    fC.push_back(temp);
    return 1;
}

int CAonCT(treeNode *head, vector<long int> ar, long int n, long int R)
{
    vector<long int> iC(ar);
    vector<vector<long int>> fC;
    vector<long int> RULE(4, 0);
    transitionRule(R, RULE);
    int configHeight = 0;
    while (1)
    {
        makeiC(iC, head, RULE);
        insertDataCayleyTree(head, iC);
        if (verificationOfConfiguration(iC, fC) == 0)
        {
            break;
        }
        configHeight++;
        // printCTSecond(head);
    }
    return configHeight;
}

void deleteCayleyTree(treeNode *head)
{
    vector<treeNode *> tempo;
    treeNode *temp = head;
    tempo.push_back(head);
    tempo.push_back(head->parent);
    long int idx = 0;
    while (temp->childOne != nullptr)
    {
        tempo.push_back(temp->childOne);
        tempo.push_back(temp->childTwo);
        temp = tempo[++idx];
    }
    idx = tempo.size();
    while (idx > 0)
    {
        temp = tempo[--idx];
        delete temp;
    }
}

void makeTable(vector<vector<int>>& Table, treeNode* CT, vector<long int>arr,long int V)
{
 for (int rule = 0; rule < 16; rule++)
    {	int max = 1, avg = 1, res = 0;
        for (int j = 0; j < 10; j++)
        {
            res = CAonCT(CT, arr, V, rule);
            if (max < res)
            {
                max = res;
            }
        avg = (avg+res);

        }
        Table[rule][0] = 1;
        Table[rule][1] = avg/10;
        Table[rule][2] = max;
    }
}

void makeiCN4(vector<long int> &ic, treeNode *header, vector<long int> rule)
{
    ic.clear();
    vector<treeNode *> queue;
    ic.push_back(rule[idxN4(header->data, header->parent->data, header->childOne->data, header->childTwo->data)]);
    queue.push_back(header->parent);
    queue.push_back(header->childOne);
    queue.push_back(header->childTwo);
    while (queue[0] != nullptr)
    {
        if (queue[0]->childOne == nullptr)
        {
            ic.push_back(rule[idxN4(queue[0]->data, queue[0]->parent->data, 0, 0)]);
        }
        else
        {
            ic.push_back(rule[idxN4(queue[0]->data, queue[0]->parent->data, queue[0]->childOne->data, queue[0]->childTwo->data)]);
        }
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        queue.erase(queue.begin());
    }
}
void CA_at_n_equals_4(treeNode *head, int rule, vector<long int> arr)
{
    vector<long int> RM(16, 0);
    transitionRule(rule, RM);
    vector<long int> iC(arr);
    vector<vector<long int>> fC;
    int count = 1;
    while (1)
    {

        makeiCN4(iC, head, RM);
        if (!verificationOfConfiguration(iC, fC))
        {
            break;
        }
        insertDataCayleyTree(head, iC);
       cout << "conf-" << ++count << ':';
       printCTArr(head);
    }
}
int main()
{
    int order = 2, height;
    cin >> height;
    // cin >> rule;
    long int V = noOfVertices(height, order);
    cout << V << endl;
    long int arr1[V];
    srand(time(nullptr));
    for (long int i = 0; i < V; i++)
    {
        arr1[i] = rand() & 1;
    }
    vector<long int> arr(arr1, arr1 + V);
    treeNode *CT = cayleyTree(height, order, V); // tree formation
    insertDataCayleyTree(CT, arr);               // data insertion into treeV:\CA-on-Cayley-Tree
    cout << "initial config:\n";
    printCTArr(CT); // printign of tree in array format
    cout << endl;
    vector<vector<int>> T(16, vector<int>(3));
    int rule;
    cin >> rule;
    cout << "\nca ct n 4:" << endl;
    CA_at_n_equals_4(CT, rule, arr);
    // makeTable(T, CT, arr, V);
    deleteCayleyTree(CT); // deletion of dynamically created tree
    return 0;
}
