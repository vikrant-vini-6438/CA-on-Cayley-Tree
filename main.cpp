#include<iostream>
#include<cmath>
#include<vector>
using namespace std;


struct treeNode{
    treeNode* parent = nullptr;
    treeNode* childOne = nullptr;
    treeNode* childTwo = nullptr;
    int data;
};
typedef struct treeNode treeNode;
int noOfVertices(int height, int order){
    int result = 1;
    for(int i = 0;i<height;i++){
        result += (order+1)*pow((order),i);
    }
    return result;
}
treeNode *cayleyTree(int h, int o, int v){
    treeNode* root = new treeNode;

    if (h == 0){return root;}

    root->parent = new treeNode;
    root->childOne = new treeNode;
    root->childTwo = new treeNode;
    root->parent->parent = root;
    root->childOne->parent = root;
    root->childTwo->parent = root;
    v -= 4;
    vector<treeNode*> queue{root->parent,root->childOne,root->childTwo};
    while(v>0){
        queue[0]->childOne = new treeNode;
        queue[0]->childOne->parent = queue[0];
        queue[0]->childTwo = new treeNode;
        queue[0]->childTwo->parent = queue[0];
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        v-= 2;
//        for(treeNode* i:queue){cout<<i->parent->childOne->data;}
        queue.erase(queue.begin());
//        for(int i = 0;i<queue.size();i++){cout<<i;}
    }
    return root;
}
void insertDataCayleyTree(treeNode* head, int * ar){
    vector<treeNode*> queue;
    queue.push_back(head);
    int idx = 0;
//    queue[0]->data = ar[idx++];
    queue.push_back(queue[0]->parent);
//    queue.push_back(queue[0]->childOne);
//    queue.push_back(queue[0]->childTwo);
//    queue.erase(queue.begin());
//    while(queue.size() and idx < V){
    while(queue[0] != nullptr){
        queue[0]->data = ar[idx++];
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
//        cout<<queue.size();
        queue.erase(queue.begin());
//        cout<<queue.size();
    }
//    return nullptr;
}

void printCayleyTree(treeNode* head){
    cout<<"present-node | parent | child-one | child-two"<<endl;
    vector<treeNode*>queue;
    queue.push_back(head);
    queue.push_back(queue[0]->parent);
    while(queue[0] != nullptr){
        //for leaf nodes:
        if(queue[0]->childOne == nullptr){
            cout<<"leaf node: "<<queue[0]->data;
        }
        //for internal nodes
        else {
            cout << queue[0]->data << "\t\t";
            cout << queue[0]->parent->data << "\t\t";
            cout << queue[0]->childOne->data << "\t\t";
            cout << queue[0]->childTwo->data << "\t\t";
        }
        queue.push_back(queue[0]->childOne);
        queue.push_back(queue[0]->childTwo);
        queue.erase(queue.begin());
        cout<<endl;
    }
}

void deleteCayleyTree(treeNode* head){
    vector<treeNode*> tempo;
    treeNode* temp = head;
    tempo.push_back(head);
    tempo.push_back(head->parent);
    int idx = 0;
    while(temp->childOne != nullptr){
        tempo.push_back(temp->childOne);
        tempo.push_back(temp->childTwo);
        temp = tempo[++idx];
    }
    idx = tempo.size();
    while(idx > 0){
        temp = tempo[--idx];
//        cout<<idx;
        delete temp;
    }
}

int main(){
    int order = 2,height;
    cout<<"height:";
    cin>>height;
    int V = noOfVertices(height, order);
//    cout<<V;
    int arr[V];
    for(int i = 0;i<V;i++){
        arr[i] = i;
    }

    treeNode* CT = cayleyTree(height, order,V);
    insertDataCayleyTree(CT, arr);
    printCayleyTree(CT);
//    cout<<CT->data;
    deleteCayleyTree(CT);
//    cout<<CT->data;
    return 0;
}



//#include<iostream>
//using namespace std;
//
//int main(){
//    int n=4;
//    for(int i = 0;i<n;i++){
//        //for spaces
//        for(int m = 0;m<n-i-1;m++){
//            cout<<"  ";
//        }
//        for (int m = 1;m<=i+1;m++){
//            cout<<m;
//        }
//        for(int m = i;m>=1;m--){
//            cout<<m;
//        }
//        cout<<endl;
//    }
//    return 0;
//}
