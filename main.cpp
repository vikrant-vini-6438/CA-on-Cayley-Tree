#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
using namespace std;

// a node structure
struct treeNode
{
    treeNode *parent = nullptr;
    treeNode *childOne = nullptr;
    treeNode *childTwo = nullptr;
    int data;
};
typedef struct treeNode treeNode;

// calculation of the total number of vertices this tree can have based on the height and order of the tree
long int noOfVertices(int height, int order)
{
    int result = 1;
    for (int i = 0; i < height; i++)
    {
        result += (order + 1) * pow((order), i);
    }
    return result;
}

treeNode *cayleyTree(int h, int o, long int v)
{
    treeNode *root = new treeNode;

    if (h == 0)
    {
        return root;
    }

    root->parent = new treeNode;
    root->childOne = new treeNode;
    root->childTwo = new treeNode;
    root->parent->parent = root;
    root->childOne->parent = root;
    root->childTwo->parent = root;
    v -= 4;                                                                 // less by 4 since 4 vertices are already created as root and its three neighbour vertices
    vector<treeNode *> queue{root->parent, root->childOne, root->childTwo}; // use of queue for performing bfs
    while (v > 0)
    { // iterate till all vertices are assigned to their respective parent and children
        queue[0]->childOne = new treeNode;
        queue[0]->childOne->parent = queue[0];
        queue[0]->childTwo = new treeNode;
        queue[0]->childTwo->parent = queue[0];
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        v -= 2; // less by 2 since every time 2 vertices are being created
        queue.erase(queue.begin());
    }
    return root;
}
void insertDataCayleyTree(treeNode *head, vector<long int> ar)
{
    vector<treeNode *> queue; // for performing bfs over tree
    queue.push_back(head);
    long int *idx = new long int;
    *idx = 0;
    queue.push_back(queue[0]->parent);
    while (queue[0] != nullptr)
    {
        queue[0]->data = ar[(*idx)++];
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        queue.erase(queue.begin());
    }
    delete idx;
}

void printCayleyTree(treeNode *head)
{
    cout << "present-node | parent | child-one | child-two" << endl;
    vector<treeNode *> queue;
    queue.push_back(head);
    queue.push_back(queue[0]->parent);
    while (queue[0] != nullptr)
    {
        // for leaf nodes:V:\CA-on-Cayley-Tree
        if (queue[0]->childOne == nullptr)
        {
            cout << "leaf node: " << queue[0]->data;
        }
        // for internal nodes
        else
        {
            cout << queue[0]->data << "\t\t";
            cout << queue[0]->parent->data << "\t\t";
            cout << queue[0]->childOne->data << "\t\t";
            cout << queue[0]->childTwo->data << "\t\t";
        }
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        queue.erase(queue.begin());
        cout << endl;
    }
}
void printCT_second(treeNode *head)
{
    vector<treeNode *> queue;
    queue.push_back(head);
    queue.push_back(head->parent);
    while (queue[0] != nullptr)
    {
        cout << queue[0]->data << ' ';
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        queue.erase(queue.begin());
    }
    cout << endl;
}

int rule(int parent, int present)
{
    return ((parent + present) % 2);
}

void transitionRule(int R, vector<long int> &RM)
{
    int idx = 0;
    while (R)
    {
        RM[idx++] = R % 2;
        R /= 2;
    }
}
int idx(int n, int p)
{
    return ((n * 2) + (p * 1));
}
void makeiC(vector<long int> &iC, long int SIZE, treeNode *headerCt, vector<long int> &R)
{
    treeNode *temp = headerCt;
    vector<treeNode *> queue;
    iC.clear();

    // data to configuration array
    /*here parent refers to the first child of considered root
      (not a real parent of root, real parent of root doesn't exist) */
    iC.push_back(R[idx(temp->data, 0)]); // added 0 becaue root has no parent so parent value will be 0
    queue.push_back(temp->parent);
    queue.push_back(temp->childOne);
    queue.push_back(temp->childTwo);

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
            // cout << "returning 0" << endl;
            return 0;
        }
    }
    fC.push_back(temp);
    // cout << "returning 1" << endl;
    return 1;
}
void CAonCT(treeNode *head, vector<long int> ar, long int n, long int R)
{
    vector<long int> iC(ar);
    vector<vector<long int>> fC;
    // cout<<"ca on ct"<<endl;
    vector<long int> RULE(4, 0);
    transitionRule(R, RULE);
    while (1)
    {
        makeiC(iC, n, head, RULE);
        insertDataCayleyTree(head, iC);
        if (verificationOfConfiguration(iC, fC) == 0)
        {
            cout << "\nfinal config: " << endl;

            printCT_second(head);
            break;
        }
        printCT_second(head);
    }
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

int main()
{
    int order = 2, height, rule;
    cin >> height;
    cin >> rule;
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
// printCayleyTree(CT);                         // printing of tree in tabular format
    cout << "initial config:\n";
    printCT_second(CT); // printign of tree in array format
    cout << endl;
    cout << "intermediate configs:\n";
    CAonCT(CT, arr, V, rule); // performing CA over Cayley Tree for m finite configurations
    deleteCayleyTree(CT);     // deletion of dynamically created tree
    return 0;
}
