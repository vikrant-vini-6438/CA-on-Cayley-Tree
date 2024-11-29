#ifndef CAYLEY_TREE_H
#define CAYLEY_TREE_H
#include<iostream>

#include <vector>

struct TreeNode {
    TreeNode* neighborOne = nullptr;
    TreeNode* neighborTwo = nullptr;
    TreeNode* neighborThree = nullptr;
    int data = -1;
};

long int calculateVertexCount(int height, int order);
TreeNode* createCayleyTree(long int height, long int v);
void insertDataIntoTree(TreeNode* head, const std::vector<long int>& data);
void printTree(TreeNode* head);
void deleteTree(TreeNode* head);

#endif // CAYLEY_TREE_H



//#ifndef CAYLEY_TREE_H
//#define CAYLEY_TREE_H
//
//#include <vector>
//
//// Forward declaration of treeNode struct
//struct treeNode {
//    treeNode* neighborOne = nullptr;
//    treeNode* neighborTwo = nullptr;
//    treeNode* neighborThree = nullptr;
//    int data = -1;
//};
//
//// Function to calculate the total number of vertices based on the height and order of the tree
//long int noOfVertices(int height, int order);
//
//// Function to create a Cayley Tree dynamically
//treeNode* cayleyTree(int h, int o, long int v);
//
//// Function to insert data into the Cayley Tree
//void insertDataCayleyTree(treeNode* head, std::vector<long int> ar);
//
//// Function to print the Cayley Tree in a tabular view
//void printCayleyTree(treeNode* head);
//
//// Function to print the Cayley Tree as an array
//void printCTArr(treeNode* head);
//
//// Function to delete the dynamically created Cayley Tree
//void deleteCayleyTree(treeNode* head);
//
//#endif // CAYLEY_TREE_H




//#include<iostream>
//#include<vector>
//
//
//#ifndef __cayley_tree_h
//#define __cayley_tree_h
//
//struct treeNode{
//    treeNode* neighborOne = nullptr;
//    treeNode* neighborTwo = nullptr;
//    treeNode* neighborThree = nullptr;
//    int data = -1;
//};
//long int noOfVertices(int height, int order);
//treeNode* cayleyTree(int h, int o, long int v);
//void insertDataCayleyTree(treeNode* head, std::vector<long int> ar);
//void printCayleyTree(treeNode* head);
//void printCTArr(treeNode* head);
//void deleteCayleyTree(treeNode* head);
//
//#endif

