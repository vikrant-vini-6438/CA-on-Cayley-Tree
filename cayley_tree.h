#include<iostream>
#include<vector>


#ifndef __cayley_tree_h
#define __cayley_tree_h

struct treeNode{
	treeNode* parent = nullptr;
	treeNode* childOne = nullptr;
	treeNode* childTwo = nullptr;
	int data = 0;
};
long int noOfVertices(int height, int order);
treeNode* cayleyTree(int h, int o, long int v);
void insertDataCayleyTree(treeNode* head, std::vector<long int> ar);
void printCayleyTree(treeNode* head);
void printCTArr(treeNode* head);
void deleteCayleyTree(treeNode* head);

#endif
