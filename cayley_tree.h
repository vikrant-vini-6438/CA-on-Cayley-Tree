#ifndef __cayley_tree_h
#define __cayley_tree_h

#include<iostream>
struct treeNode{
	treeNode* parent = nullptr;
	treeNode* childOne = nullptr;
	treeNode* childTwo = nullptr;
};

treeNode* cayleyTree(int h, int o, long int v);
void insertDataCayleyTree(treeNode* head, vector<long int> ar);
void printCayleyTree(treeNode* head);
void printCTArr(treeNode* head);
void deleteCayleyTree(treeNode* head);

#endif
