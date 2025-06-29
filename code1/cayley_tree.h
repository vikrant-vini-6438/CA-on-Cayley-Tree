#ifndef CAYLEY_TREE_H
#define CAYLEY_TREE_H
#include <vector>

struct TreeNode {
    std::vector<TreeNode*> neighbors; // Dynamic list of neighbors
    int data = -1;
};

long int calculateVertexCount(int height, int order);
TreeNode* createCayleyTree(long int height, long int order, long int v);
void insertDataIntoTree(TreeNode* head, const std::vector<long int>& data);
void printTree(TreeNode* head);
void deleteTree(TreeNode* head);

#endif // CAYLEY_TREE_H
