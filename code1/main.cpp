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

struct VectorHasher {
    size_t operator()(const std::vector<long>& v) const {
        size_t seed = 0;
        for (auto& i : v) {
            seed ^= std::hash<long>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
        return seed;
    }
};

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

// Generalized idxR for large orders
int idxR(int self, const vector<TreeNode*>& neighbors, long int order) {
    int count = 0;
    for (long int i = 1; i < neighbors.size(); ++i) { // Skip parent (neighbors[0])
        if (neighbors[i] && neighbors[i]->data == 1) {
            count++;
        }
    }
    // Use a larger rule space: map count to 0–15 (2^4 rules)
    vector<long int> tempV(4, 0); // Use 4 bits for larger rule space
    to_binary(tempV, count % 16); // Modulo to limit count for large orders
    return (tempV[0] * 8 + tempV[1] * 4 + self * 2 + tempV[2] * 1) % 16; // Return index 0–15
}

void makeiCR(vector<long int>& ic, TreeNode* header, const vector<long int>& rule, vector<long int>& F, long int V, long int order) {
    ic.clear();
    queue<TreeNode*> nodeQueue;
    ic.push_back(rule[idxR(header->data, header->neighbors, order)]);
    for (long int i = 1; i < header->neighbors.size(); ++i) {
        if (header->neighbors[i]) {
            nodeQueue.push(header->neighbors[i]);
        }
    }

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        ic.push_back(rule[idxR(current->data, current->neighbors, order)]);
        for (long int i = 1; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i]) {
                nodeQueue.push(current->neighbors[i]);
            }
        }
    }
    F[toDeci(ic, V)] = 1;
}

int verificationOfConfigurationReduced(vector<long int>& temp, unordered_set<vector<long int>, VectorHasher>& fC) {
    return fC.insert(temp).second;
}

int calLoop(vector<int>& t) {
    int count = 1;
    int n = t.size() - 1;
    for (int j = n - 1; j > -1; j--) {
        if (t[j] == t[n]) return count;
        count++;
    }
    return count;
}

void CAonCT_reduced_rules(long int vertices, int rule, TreeNode* tree, vector<int>& lV, long int order) {
    lV[0] = INT_MAX; lV[1] = 0; lV[2] = INT_MIN;
    const long int maxConfigs = 10000; // Limit to 10,000 configurations
    vector<long int> tempV(vertices, 0);
    vector<long int> RULE(16, 0); // Increased rule size to 2^4
    to_binary(RULE, rule % 65536); // Map rule to 0–65535 for larger space
    unordered_set<vector<long int>, VectorHasher> fCr;
    vector<long int> flag(min(static_cast<long int>(1e6), static_cast<long int>(pow(2, min(vertices, 20L)))), 0);
    vector<int> confsDerived;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<long int> dist(0, pow(2, min(vertices, 20L)) - 1);

    for (long int i = 0; i < maxConfigs; ++i) {
        long int config = dist(gen); // Random configuration
        to_binary(tempV, config);
        vector<long int> iCr(tempV);
        long int idR = toDeci(iCr, vertices);
        if (flag[idR % flag.size()] != 0) continue;
        confsDerived.push_back(idR);
        while (verificationOfConfigurationReduced(iCr, fCr)) {
            insertDataIntoTree(tree, iCr);
            makeiCR(iCr, tree, RULE, flag, vertices, order);
            idR = toDeci(iCr, vertices);
            confsDerived.push_back(idR);
            if (flag.size() > idR) flag[idR] = 1; // Mark only if within bounds
        }
        int looplength = calLoop(confsDerived);
        if (looplength < lV[0]) lV[0] = looplength;
        if (looplength > lV[2]) lV[2] = looplength;
        fCr.clear();
        confsDerived.clear();
        fill(tempV.begin(), tempV.end(), 0);
    }
}

void iCM(vector<long int>& ic, TreeNode* header, const vector<long int>& rule, long int order) {
    ic.clear();
    queue<TreeNode*> nodeQueue;
    ic.push_back(rule[idxR(header->data, header->neighbors, order)]);
    for (long int i = 1; i < header->neighbors.size(); ++i) {
        if (header->neighbors[i]) {
            nodeQueue.push(header->neighbors[i]);
        }
    }

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        ic.push_back(rule[idxR(current->data, current->neighbors, order)]);
        for (long int i = 1; i < current->neighbors.size(); ++i) {
            if (current->neighbors[i]) {
                nodeQueue.push(current->neighbors[i]);
            }
        }
    }
}

long int CA_G(const vector<long int>& Tr, long int conf, TreeNode* tree, long int vertices, long int order) {
    vector<long int> temp(vertices, 0);
    to_binary(temp, conf);
    insertDataIntoTree(tree, temp);
    iCM(temp, tree, Tr, order);
    return toDeci(temp, vertices);
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
    ofstream myfile("output.txt", ios::trunc);
    if (!myfile) {
        cerr << "File not created or did not open successfully" << endl;
        return 1;
    }
    std::unique_ptr<TreeNode> CT(createCayleyTree(height, order, V));
    if (!CT) {
        cerr << "Cayley tree creation failed" << endl;
        return 1;
    }
    vector<int> lengthVector(3);
    myfile << "RULE\tMIN cl\tMAX cl" << endl;
    cout << "RULE\tMIN cl\tMAX cl" << endl;
    for (int rule = 0; rule < 256; rule++) {
        CAonCT_reduced_rules(V, rule, CT.get(), lengthVector, order);
        myfile << rule << '\t' << lengthVector[0] << '\t' << lengthVector[2] << '\n';
        cout << rule << '\t' << lengthVector[0] << '\t' << lengthVector[2] << endl;
    }
    myfile.close();
    return 0;
}
