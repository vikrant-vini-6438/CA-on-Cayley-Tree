#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_set>
#include <fstream>
#include <climits>
#include <memory>
#include <random>
#include "cayley_tree.h"
using namespace std;



void to_binary(vector<long int>& ar, long int M) {
    int idx = 0;
    while (M && idx < ar.size()) {
        ar[idx++] = (M & 1);
        M >>= 1;
    }
}

long int toDeci(const vector<long int>& T, long int v) {
    long int val = 0;
    for (long int i = 0; i < v; i++) {
        val += pow(2, i) * T[i];
    }
    return val;
}

int idxR(int self, const vector<TreeNode*>& neighbors, long int order) {
    int count = 0;
    for (long int i = 1; i < neighbors.size(); ++i) {
        if (neighbors[i] &&  neighbors[i]->data == 1) {
            count++;
        }
    }
    vector<long int> temp(2, 0);
    to_binary(temp, count % 4);
    return (temp[0] * 1 + self * 2 + temp[1] * 4) % 8;
}

void CA(vector<long int>& ic, TreeNode* header, const vector<long int>& rule, long int V, long int order,const long int vert) {
   ic.clear(); 
   ic.resize(vert);
   fill(ic.begin(), ic.end(), 0);
   int idx  = 0;
    queue<TreeNode*> nodeQueue;
    //ic.push_back(rule[idxR(header->data, header->neighbors, order)]);
    ic[idx++] = rule[idxR(header->data, header->neighbors, order)];

    for (long int i = 1; i < header->neighbors.size(); ++i) {
        if (header->neighbors[i]) {
            nodeQueue.push(header->neighbors[i]);
        }
    }

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        //ic.push_back(rule[idxR(current->data, current->neighbors, order)]);
	ic[idx++] = rule[idxR(current->data, current->neighbors, order)];
        for (long int i = 1; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i]) {
                nodeQueue.push(current->neighbors[i]);
            }
        }
    }
}

int cycleCheck(const long int vertices, const int rule, TreeNode* tree, const long int order) {
    long int i = 0, loopCount = 0;
    vector<long int> RULE(8, 0);
    to_binary(RULE, rule % 256);
    long long totalIdx = pow(2, vertices);
    vector<long int> flag(totalIdx, 0);
    vector<long int> nextConf(vertices, 0);
    vector<long int> data(vertices, 0);
    while (i < totalIdx) {
	    
        if (flag[i] == 1) {
            i++;
            continue;
        } else {
		to_binary(nextConf, i);
            while (1) {

                flag[i] = 1;
                insertDataIntoTree(tree, nextConf);
                CA(nextConf, tree, RULE, vertices, order,vertices);
		i = toDeci(nextConf, vertices);
		
                if (flag[i] == 1) {
                    break;
                }
                //data = nextConf;
		fill(nextConf.begin(), nextConf.end(), 0);
            }
        }
        i = 0;loopCount++;
    }
    return loopCount;
}

int main() {
    long int order, height;
    cout << "Order: ";
    cin >> order;
    cout << "Height: ";
    cin >> height;
    long int V = calculateVertexCount(height, order);
    cout << "Total vertices: " << V << endl;
    if (V > 20) {
        cerr << "Warning: Vertex count " << V << " is large. Sampling 10,000 configurations." << endl;
    }
    std::unique_ptr<TreeNode> CT(createCayleyTree(height, order, V));
    if (!CT) {
        cerr << "Cayley tree creation failed" << endl;
        return 1;
    }
    int rule;
    cout << "rule: ";
    cin >> rule;
    cout << "cycle for rule:" << rule << "::" << cycleCheck(V, rule, CT.get(), order);
    return 0;
}
