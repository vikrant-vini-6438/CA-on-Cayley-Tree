#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <memory>
#include "./cayley_tree.h"

using namespace std;

long int calculateVertexCount(int height, int order) {
    long int result = 1;
    for (int i = 0; i < height; ++i) {
        result += (order + 1) * pow(order, i);
    }
    return result;
}

TreeNode* createCayleyTree(long int height, long int v) {
    unique_ptr<TreeNode> root(new TreeNode);

    if (height == 0) {
        return root.release();
    }

    root->neighborOne = new TreeNode;
    root->neighborTwo = new TreeNode;
    root->neighborThree = new TreeNode;
    root->neighborOne->neighborOne = root.get();
    root->neighborTwo->neighborOne = root.get();
    root->neighborThree->neighborOne = root.get();

    queue<TreeNode*> nodeQueue;
    nodeQueue.push(root->neighborOne);
    nodeQueue.push(root->neighborTwo);
    nodeQueue.push(root->neighborThree);

    long long verticesLeft = v - 4; // Optimized vertex count calculation
    while (verticesLeft > 0) {
        TreeNode* current = nodeQueue.front();
        current->neighborTwo = new TreeNode;
        current->neighborTwo->neighborOne = current;
        current->neighborThree = new TreeNode;
        current->neighborThree->neighborOne = current;
        nodeQueue.push(current->neighborTwo);
        nodeQueue.push(current->neighborThree);
        verticesLeft-=2;
        nodeQueue.pop();
    }

    return root.release();
}

void insertDataIntoTree(TreeNode* head, const vector<long int>& data) {
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(head);
    int index = 0;

    TreeNode* current = nodeQueue.front();

    current->data = data[index++];
    nodeQueue.push(current->neighborOne);
    nodeQueue.push(current->neighborTwo);
    nodeQueue.push(current->neighborThree);

    nodeQueue.pop();
    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        current->data = data[index++];

        if (current->neighborTwo != nullptr) {
            nodeQueue.push(current->neighborTwo);
            nodeQueue.push(current->neighborThree);
        }
    }
}

void printTree(TreeNode* head) {
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(head);

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        // ... (print node data as desired)

        if (current->neighborTwo != nullptr) {
            nodeQueue.push(current->neighborTwo);
            nodeQueue.push(current->neighborThree);
        }
    }
}

void deleteTree(TreeNode* head) {
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(head);

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        if (current->neighborTwo != nullptr) {
            nodeQueue.push(current->neighborTwo);
            nodeQueue.push(current->neighborThree);
        }
        delete current;
    }
}