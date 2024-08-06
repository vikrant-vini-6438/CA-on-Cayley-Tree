#include<iostream>
#include "cayley_tree.h"
#include<cmath>
using namespace std;


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

//creation of Cayley Tree dynamically @ each node with dynamic memory allocation 
treeNode *cayleyTree(int h, int o, long int v)
{
    treeNode *root = new treeNode;

    if (h == 0)
    {
        return root;
    }

    root->neighborOne = new treeNode;
    root->neighborTwo = new treeNode;
    root->neighborThree = new treeNode;
    root->neighborOne->neighborOne = root;
    root->neighborTwo->neighborOne = root;
    root->neighborThree->neighborOne = root;
    v -= 4;                                                                 // less by 4 since 4 vertices are already created as root and its three neighbour vertices
    vector<treeNode *> queue{root->neighborOne, root->neighborTwo, root->neighborThree}; // use of queue for performing bfs
    while (v > 0)
    { // iterate till all vertices are assigned to their respective parent and children
        queue[0]->neighborTwo = new treeNode;
        queue[0]->neighborTwo->neighborOne = queue[0];
        queue[0]->neighborThree = new treeNode;
        queue[0]->neighborThree->neighborOne = queue[0];
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        v -= 2; // less by 2 since every time 2 vertices are being created
        queue.erase(queue.begin());
    }
    return root;
}

//insertion of data into Cayley Tree 
void insertDataCayleyTree(treeNode *head, vector<long int> ar)
{
    vector<treeNode *> queue; // for performing bfs over tree
    queue.push_back(head);
    long int *idx = new long int;
    *idx = 0;
    queue.push_back(queue[0]->neighborOne);
    while (queue[0] != nullptr)
    {
        queue[0]->data = ar[(*idx)++];
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        queue.erase(queue.begin());
    }
    delete idx;
}

//Cayley Tree printing as tabular view which one belong to which node in any form either parent or child
void printCayleyTree(treeNode *head)
{
    cout << "present-node | parent | child-one | child-two" << endl;
    vector<treeNode *> queue;
    queue.push_back(head);
    queue.push_back(queue[0]->neighborOne);
    while (queue[0] != nullptr)
    {
        // for leaf nodes:
        if (queue[0]->neighborTwo == nullptr)
        {
            cout << "leaf node: " << queue[0]->data;
        }
        // for internal nodes
        else
        {
            cout << queue[0]->data << "\t\t";
            cout << queue[0]->neighborOne->data << "\t\t";
            cout << queue[0]->neighborTwo->data << "\t\t";
            cout << queue[0]->neighborThree->data << "\t\t";
        }
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        queue.erase(queue.begin());
        cout << endl;
    }
}

//printing of Cayley Tree as an array
void printCTArr(treeNode *head)
{
    vector<treeNode *> queue;
    queue.push_back(head);
    queue.push_back(head->neighborOne);
    while (queue[0] != nullptr)
    {
        cout << queue[0]->data << ' ';
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        queue.erase(queue.begin());
    }
    cout << endl;
}

//deletion of dynamically created Cayley Tree
void deleteCayleyTree(treeNode *head)
{
    vector<treeNode *> tempo;
    treeNode *temp = head;
    tempo.push_back(head);
    tempo.push_back(head->neighborOne);
    long int idx = 0;
    while (temp->neighborTwo != nullptr)
    {
        tempo.push_back(temp->neighborTwo);
        tempo.push_back(temp->neighborThree);
        temp = tempo[++idx];
    }
    idx = tempo.size();
    while (idx > 0)
    {
        temp = tempo[--idx];
        delete temp;
    }
}

