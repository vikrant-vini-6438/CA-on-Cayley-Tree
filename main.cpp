#include <iostream>
#include <cmath>
#include <climits>
#include <vector>
//#include <cstdlib>
//#include <ctime>
#include "./cayley_tree.h"
//#include <fstream>
using namespace std;

// a node structure
typedef struct treeNode treeNode;


/*
int rule(int parent, int present)
{
    return ((parent + present) % 2);
}
*/
//for n = 2
void to_binary(vector<long int>&ar,int M){

	int idx = 0;
	while (M)
	{
		ar[idx++] = (M&1);
		M >>= 1;
	}
}
long int toDeci(vector<long int> T,long int v){
	long int val = 0;
	for(long int i = 0; i< v; i++){
		val += pow(2,i) * T[i];
	}
	return val;
}
int idx(int n, int p)
{
	return ((n * 2) + (p * 1));
}
//for n = 4
int idxN4(int self, int p, int N1, int N2)
{
    return (self * 8 + p * 4 + (N1 + N1) + N2); // self x 2^3 + p x 2^2 + N1 x 2^1 + N2 x 2^0
}
int idxR(int self, int neighborOne, int neighborTwo, int neighborThree){
	int *arr= new int[3]{neighborOne, neighborTwo, neighborThree};
	int *count = new int(0);
	for(int i = 0;i<3;i++){
		if (arr[i] == 1) (*count)++;
	}
	vector<long int> tempV{0, 0};
	to_binary(tempV, *count);
	delete count;
	delete [] arr;
	return (tempV[0] * 4 + self * 2 + tempV[1] * 1);
//	return (self * 4 + tempV[1] * 2 + tempV[0] * 1);
}
void shiftTransitionRule(vector<long int>&arr){
	int temp = 0;
	int tempArr[4];
	int counter = -1, idx,counterr = -1;
	
	while(++counter < 16)
	{
		if(arr[counter] == 1){
			//cout<<"\ncounter: "<<counter<<endl;
			counterr = counter;
			idx = 0;
			tempArr[0]=0,tempArr[1]=0,tempArr[2]=0,tempArr[3]=0;
			while(counterr){
				tempArr[idx++] = (counterr&1);
				counterr >>= 1;
			}
			//cout<<endl;
			//for (int i = 0;i<4;i++) cout<<"arr ele: "<<tempArr[i]<<endl;
			for(int i = 0;i<3;i++)//its already known the number of rotations
			{
				arr[idxN4(tempArr[3], tempArr[2], tempArr[1], tempArr[0])] = 1;

				temp = tempArr[0];
				tempArr[0] = tempArr[1];
				tempArr[1] = tempArr[2];
				tempArr[2] = temp;	
			//for(int i = 0;i<4;i++){cout<<"\nchecking in loop: "<<tempArr[i];}
			//cout<<endl;
			}
			//cout<<endl;
			//for(int i = 0;i<4;i++){cout<<"\nchecking: "<<tempArr[i];}
				//cout<<endl;
		}
	}
}

void transitionRule(int R, vector<long int> &RM)
{
	to_binary(RM, R);
	cout<<"RULE: ";
	for(long int ele :RM){cout<<ele<<' ';}
	shiftTransitionRule(RM);
	cout<<"\nafter shifting: ";
	for(long int ele: RM){cout<<ele<<' ';}
	cout<<endl;
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
void makeiCN4(vector<long int> &ic, treeNode *header, vector<long int> rule,vector<long int>& F,long int V)
{
    ic.clear();
    vector<treeNode *> queue;
   ic.push_back(rule[idxN4(header->data, header->neighborOne->data, header->neighborTwo->data, header->neighborThree->data)]);

//    ic.push_back(rule[idxN4(header->neighborOne->data, header->data, header->neighborTwo->data, header->neighborThree->data)]);
    queue.push_back(header->neighborOne);
    queue.push_back(header->neighborTwo);
    queue.push_back(header->neighborThree);
    while (queue[0] != nullptr)
    {
        if (queue[0]->neighborTwo == nullptr)
        {

            ic.push_back(rule[idxN4( queue[0]->data,queue[0]->neighborOne->data, 0, 0)]);
           // ic.push_back(rule[idxN4( queue[0]->neighborOne->data,queue[0]->data, 0, 0)]);
        }
        else
        {
            ic.push_back(rule[idxN4(queue[0]->data,queue[0]->neighborOne->data,queue[0]->neighborTwo->data, queue[0]->neighborThree->data)]);
		
           // ic.push_back(rule[idxN4(queue[0]->neighborOne->data,queue[0]->data,queue[0]->neighborTwo->data, queue[0]->neighborThree->data)]);
        }
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        queue.erase(queue.begin());
    }

	F[toDeci(ic, V)]=1;
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
//making an configuration for the implementation of transition rule, this configuration is derived form the tree
void makeiCR(vector<long int> &ic, treeNode *header, vector<long int> rule,vector<long int>& F,long int V)
{
    ic.clear();//if there is/may be any already derived configuration then clear 
    vector<treeNode *> queue;
    ic.push_back(rule[idxR(header->data, header->neighborOne->data, header->neighborTwo->data, header->neighborThree->data)]);
    queue.push_back(header->neighborOne);
    queue.push_back(header->neighborTwo);
    queue.push_back(header->neighborThree);
    while (queue[0] != nullptr)
    {
        if (queue[0]->neighborTwo == nullptr)
        {
            ic.push_back(rule[idxR(queue[0]->data, queue[0]->neighborOne->data, 0, 0)]);
        }
        else
        {
            ic.push_back(rule[idxR(queue[0]->data, queue[0]->neighborOne->data, queue[0]->neighborTwo->data, queue[0]->neighborThree->data)]);
        }
        queue.push_back(queue[0]->neighborTwo);
        queue.push_back(queue[0]->neighborThree);
        queue.erase(queue.begin());
    }
	F[toDeci(ic, V)]=1;

}

//verification if there is any configuration repeating itself
int verificationOfConfigurationReduced(vector<long int> temp, vector<vector<long int>> &fC,long int *tempC,long int n)
{
					
    int j = fC.size();
    for (long i = 0; i < j; i++)
    {
        if (temp == fC[i]) //comparision with list of already created configurations
        {	*tempC = toDeci(temp, n);
            return 0; //if such repeating configuration is found to be there in the list
        }
    }
    fC.push_back(temp);//adding dissimilar configuration to the list
    return 1;//no match found
}
int calLoop(vector<int> t){
	int count = 1;
	int n = t.size()-1;
	for(int j = n-1;j> -1;j--){
		if (t[j] == t[n]) return count;
		count++;
	}
	return count;
}
void CAonCT_reduced_rules(long int vertices, int rule,treeNode* tree,vector<int>&lV){
	lV[0] = INT_MAX;lV[1] = 0; lV[2] = INT_MIN;
	int totalIdx = pow(2, vertices); //how much configurations should be there for consideration of transition rule
	vector<vector<long int>> confs(totalIdx); //original matrix of configurations which contains a empty list which is further updated with its binary values related to each of its index 
	vector<long int> tempV(vertices,0);
	vector<long int> RULE(8,0);//rule matrix :: 8-bit binary form of RULE 
	to_binary(RULE, rule);
	vector<vector<long int>>fCr; //checking for cycle of similar configurations after applying transition rule everytime 
	vector<long int> flag(totalIdx,0);
	for(int i = 0;i<totalIdx;i++){
		to_binary(tempV, i);
		confs[i] = tempV; //assigning each index of conf with a totalIdx-bit binary form of its index 
		fill(tempV.begin(), tempV.end(), 0); //resetting temporary vector again to its default values '0'
	}
//	vector<int> reversibleRule(totalIdx);//vector for checking if reversiblity of rule occurs :: if at any point of time after iteration any off bit is found to be there in this list then rule is said to be as not reversible
	long int *tempRev = new long int;
	vector<int> confsDerived;
	vector<int> lenConfs;
	int * looplength = new int(0);
	for(vector<long int> tt: confs){
		vector<long int>iCr(tt);
	//	long int idR = toDeci(iCr, vertices);
		long int *idR = new long int(toDeci(iCr, vertices));
		if(flag[*idR] != 0) continue; 
		confsDerived.push_back(*idR);
		//cout<<*idR<<"->";
	//	int count = 1;
		while(verificationOfConfigurationReduced(iCr, fCr,tempRev,vertices)){

			insertDataCayleyTree(tree, iCr);
			makeiCR(iCr, tree, RULE,flag,vertices);
			*idR = toDeci(iCr, vertices);
		//	cout<<toDeci(iCr, vertices)<<"->";
			confsDerived.push_back(*idR);
			//cout<<*idR<<"->";
/*			count++;
			if (count == 2){
				cout<<endl;
				cout<<toDeci(iCr, vertices)<< ' ';
				count = 1;
			}*/
			
			
		}
	/*	if(idR != *tempRev) {
			//cout<<'\n'<<rule<<" :: non reversible"<<endl;
			return;
		}
	*/	
		(*looplength) = calLoop(confsDerived);
		//cout<<"\tloop length: "<<(*looplength);
		lenConfs.push_back((*looplength));
		//cout<<endl;
		fCr.clear();
		delete idR;
		confsDerived.clear();
	}
	*looplength = 0;
	for( int i: lenConfs){
		if (i < lV[0]) lV[0] = i;
		if (i > lV[2]) lV[2] = i;
		(*looplength) += i;	
	}
	lV[1] = (*looplength)/(totalIdx);
	delete looplength;
	delete tempRev;
	return;
}	

int main()
{
	int order = 2, height;
	cin >> height;
	long int V = noOfVertices(height, order);
	cout << V << endl;

	treeNode *CT = cayleyTree(height, order, V); // tree formation
//	int rule;
//	cin >> rule;

	vector<int> lengthVector(3);
	cout<<"RULE"<<"\tMIN cl"<<"\tAVG cl"<<"\tMAX cl"<<endl;
	for(int rule = 0;rule<256;rule++){
		CAonCT_reduced_rules(V, rule, CT,lengthVector);
		cout<<rule<<'\t'<<lengthVector[0]<<'\t'<<lengthVector[1]<<'\t'<<lengthVector[2]<<endl;
	}
	deleteCayleyTree(CT);
}
