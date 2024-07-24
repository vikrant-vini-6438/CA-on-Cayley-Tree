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
        // for leaf nodes:
        if (queue[0]->childOne == nullptr)
        {
            cout << "leaf node: "<< queue[0]->data;
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
void printCTSecond(treeNode *head)
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
/*
int idx(int n, int p)
{
    return ((n * 2) + (p * 1));
}
*/

int idxN4(int self, int p, int c1, int c2){
	return (self*8 + p*4 + (c1+c1) + c2); //self x 2^3 + p x 2^2 + c1 x 2^1 + c2 x 2^0
}
/*
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
*/
// for final set of configurations
int verificationOfConfiguration(vector<long int> temp, vector<vector<long int>> &fC)
{

    int j = fC.size();
    for (long i = 0; i < j; i++)
    {
        if (temp == fC[i])
        {
             cout << "returning 0" << endl;
            return 0;
        }
    }
    fC.push_back(temp);
     cout << "returning 1" << endl;
    return 1;
}
/*
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
		
		//configHeight++;
            break;
        }
        configHeight++;
        // printCTSecond(head);
    }
    return configHeight;
}
*/
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
/*
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
*/
void makeiCN4(vector<long int>& ic, treeNode* header,vector<long int> rule){
	cout<<"makeiCn4"<<endl;
	ic.clear();
	vector<treeNode* > queue;
	int m = idxN4(header->data, header->parent->data, header->childOne->data, header->childTwo->data);
	cout<<m<<endl;
	ic.push_back(rule[idxN4(header->data, header->parent->data, header->childOne->data, header->childTwo->data)]);
	queue.push_back(header->parent);
	queue.push_back(header->childOne);
	queue.push_back(header->childTwo);
	while(queue[0] != nullptr){
		ic.push_back(rule[idxN4(queue[0]->data, queue[0]->parent->data, queue[0]->childOne->data, queue[0]->childTwo->data)]);
		queue.push_back(queue[0]->childOne);
		queue.push_back(queue[0]->childTwo);
	}
	cout<<"makeic n4"<<endl;
}
void CA_at_n_equals_4(treeNode* head,int rule,vector<long int> arr){
	vector<long int> RM(16, 0);
	transitionRule(rule, RM);
	for(long int m : RM){cout<<m;}
	vector<long int> iC(arr);
	cout<<endl;
	for(long int m: iC){cout<<m;}
	vector<vector<long int>> fC;
	while ( verificationOfConfiguration(iC, fC)){
		makeiCN4(iC, head, RM);
		insertDataCayleyTree(head,iC);
		printCTSecond(head);
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
                                                 // printCayleyTree(CT);                         // printing of tree in tabular format
    cout << "initial config:\n";
    printCTSecond(CT); // printign of tree in array format
    cout << endl;
    vector<vector<int>> T(16,vector<int>(3));
    // for (int rule = 0; rule < 16; rule++)
    // {	int max = 1, avg = 1, res = 0;
    //     for (int j = 0; j < 10; j++)
    //     {
    //         res = CAonCT(CT, arr, V, rule);
    //         if (max < res)
    //         {
    //             max = res;
    //         }
	//     avg = (avg+res);
	    
    //     }
    //     T[rule][0] = 1;
    //     T[rule][1] = avg/10;
    //  V:\CA-on-Cayley-Tree   T[rule][2] = max;
    // } // performing CA over Cayley Tree for m finite configurations
   int rule;cin>>rule;

   // CAonCT(CT,arr, V, rule);
   // makeTable(T, CT, arr, V);
   // cout << "RULE\t" << "minH\t" << "avgH\t" << "maxH\n";
  //  for (int i = 0; i < 16; i++)
  /*  {
        cout << i << '\t';
        for (int j = 0; j < 3; j++)
        {
            cout << T[i][j] << '\t';
        }
        cout << endl;
    }*/
    cout<<"\nca ct n 4:"<<endl;
    CA_at_n_equals_4(CT,rule,arr);
   // makeTable(T, CT, arr, V);
    deleteCayleyTree(CT); // deletion of dynamically created tree
    return 0;
}
