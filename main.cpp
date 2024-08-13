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
void makexx(int *arr, int m){
	for(int i = 0;i<4;i++){
		arr[i] = (m&1);
		m >>= 1;
	}
}
void transitionRule(int R, vector<long int> &RM)
{
    int idx = 0;
    while (R)
    {
        RM[idx++] = R % 2;
        R /= 2;
    }
    
    for(int i = 0;i<idx;i++){
	    int* arr = new int[4]{0}; 
	    if(RM[i] == 1){
		makexx(arr, i);	
		}

	    }
	    delete [] arr;
    }
}
//for n = 2
int idx(int n, int p)
{
	return ((n * 2) + (p * 1));
}
//for n = 4
int idxN4(int self, int p, int N1, int N2)
{
    return (self * 8 + p * 4 + (N1 + N1) + N2); // self x 2^3 + p x 2^2 + N1 x 2^1 + N2 x 2^0
}
//for n = 2
void makeiC(vector<long int> &iC, treeNode *headerCt, vector<long int> &R)
{
    vector<treeNode *> queue;
    iC.clear();

    // data to configuration array
   //here parent refers to the first child of considered root
     // (not a real parent of root, real parent of root doesn't exist)
    iC.push_back(R[idx(headerCt->data, 0)]); // added 0 becaue root has no parent so parent value will be 0
    queue.push_back(headerCt->neighborOne);
    queue.push_back(headerCt->neighborTwo);
    queue.push_back(headerCt->neighborThree);

    while (queue[0] != nullptr)
    {
        iC.push_back(R[idx(queue[0]->data, queue[0]->neighborOne->data)]);
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        queue.erase(queue.begin());
    }
}

//for n = 4
void makeiCN4(vector<long int> &ic, treeNode *header, vector<long int> rule)
{
    ic.clear();
    vector<treeNode *> queue;
    ic.push_back(rule[idxN4(header->data, header->neighborOne->data, header->neighborTwo->data, header->neighborThree->data)]);
    queue.push_back(header->neighborOne);
    queue.push_back(header->neighborTwo);
    queue.push_back(header->neighborThree);
    while (queue[0] != nullptr)
    {
        if (queue[0]->neighborTwo == nullptr)
        {
            ic.push_back(rule[idxN4(queue[0]->data, queue[0]->neighborOne->data, 0, 0)]);
        }
        else
        {
            ic.push_back(rule[idxN4(queue[0]->data, queue[0]->neighborOne->data, queue[0]->neighborTwo->data, queue[0]->neighborThree->data)]);
        }
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
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
//execution of CA @ n = 2
int CAonCT(treeNode *head, vector<long int> ar, long int R)
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

//tabular analysis of CA 
void makeTable(vector<vector<int>>& Table, treeNode* CT, vector<long int>arr,long int V)
{
 for (int rule = 0; rule < 16; rule++)
    {	int max = 1, avg = 1, res = 0;
        for (int j = 0; j < 10; j++)
        {
            res = CAonCT(CT, arr, rule);
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

//execution of CA @ n = 4
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
    insertDataCayleyTree(CT, arr);               // data insertion into tree
    cout << "initial config:\n";
    printCTArr(CT); // printing of tree in array format
    cout << endl;
    vector<vector<int>> T(16, vector<int>(3));
   // int rr;cout<<"rule for n = 2: ";cin>>rr;
   // CAonCT(CT, arr,rr);

    int rule;
    cin >> rule;
    cout << "\nca ct n 4:" << endl;
    CA_at_n_equals_4(CT, rule, arr);
    // makeTable(T, CT, arr, V);
    deleteCayleyTree(CT); // deletion of dynamically created tree
    return 0;
}
