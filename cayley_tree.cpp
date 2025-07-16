#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <memory>
#include "./cayley_tree.h"

using namespace std;

long int calculateVertexCount(int height, int order) {
    long int result = 1; // Root node
    for (int i = 0; i < height; ++i) {
        result += (order + 1) * pow(order, i);
    }
    return result;
}

TreeNode* createCayleyTree(long int height, long int order, long int v) {
    unique_ptr<TreeNode> root(new TreeNode);
    root->neighbors.resize(order + 1, nullptr); // Root has order + 1 neighbors

    if (height == 0) {
        return root.release();
    }

    // Initialize neighbors of the root
    for (long int i = 0; i < order + 1; ++i) {
        root->neighbors[i] = new TreeNode;
        root->neighbors[i]->neighbors.resize(order + 1, nullptr);
        root->neighbors[i]->neighbors[0] = root.get(); // Connect back to parent
    }

    queue<TreeNode*> nodeQueue;
    for (long int i = 0; i < order + 1; ++i) {
        nodeQueue.push(root->neighbors[i]);
    }

    long long verticesLeft = v - (order + 2); // Root + its neighbors
    while (verticesLeft > 0 && !nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        // Add neighbors (except the parent link)
        for (long int i = 1; i < order + 1 && verticesLeft > 0; ++i) {
            if (current->neighbors[i] == nullptr) {
                current->neighbors[i] = new TreeNode;
                current->neighbors[i]->neighbors.resize(order + 1, nullptr);
                current->neighbors[i]->neighbors[0] = current; // Connect back to parent
                nodeQueue.push(current->neighbors[i]);
                verticesLeft--;
            }
        }
    }

    return root.release();
}

void insertDataIntoTree(TreeNode* head, const vector<long int>& data) {
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(head);
    size_t index = 0;
    if (head->neighbors[0] != nullptr) {
	    nodeQueue.push(head->neighbors[0]);
    }
    while (!nodeQueue.empty() && index < data.size()) {
    	TreeNode* current = nodeQueue.front();
        nodeQueue.pop();
        current->data = data[index++];

        // Add non-null neighbors (except parent) to the queue
        for (size_t i = 1; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i] != nullptr) {
                nodeQueue.push(current->neighbors[i]);
            }
        }
    }
}

void printTree(TreeNode* head) {
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(head);
    nodeQueue.push(head->neighbors[0]);

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        cout << "Node data: " << current->data << ", Neighbors: ";
        for (size_t i = 0; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i] != nullptr) {
                cout << current->neighbors[i]->data << " ";
            } else {
                cout << "null ";
            }
        }
        cout << endl;

        // Add non-null neighbors (except parent) to the queue
        for (size_t i = 1; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i] != nullptr) {
                nodeQueue.push(current->neighbors[i]);
            }
        }
    }
}

void deleteTree(TreeNode* head) {
    queue<TreeNode*> nodeQueue;
    nodeQueue.push(head);

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        // Add non-null neighbors (except parent) to the queue
        for (size_t i = 1; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i] != nullptr) {
                nodeQueue.push(current->neighbors[i]);
            }
        }
        delete current;
    }
}
