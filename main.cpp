#include <iostream>
#include <cmath>
#include <vector>
#include <queue>
#include <unordered_set>
#include <fstream>
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


void to_binary(vector<long int>& ar, int M) {
    int idx = 0;
    while (M) {
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

int idxR(int self, int neighborOne, int neighborTwo, int neighborThree) {
    int arr[3] = {neighborOne, neighborTwo, neighborThree};
    int count = 0;
    for (int i = 0; i < 3; i++) {
        if (arr[i] == 1) count++;
    }
    vector<long int> tempV(2, 0);
    to_binary(tempV, count);
    return (tempV[0] * 4 + self * 2 + tempV[1] * 1);
}
void makeiCR(vector<long int>& ic, TreeNode* header, const vector<long int>& rule, vector<long int>& F, long int V) {
    ic.clear();
    queue<TreeNode*> nodeQueue;
    ic.push_back(rule[idxR(header->data, header->neighborOne->data, header->neighborTwo->data, header->neighborThree->data)]);
    nodeQueue.push(header->neighborOne);
    nodeQueue.push(header->neighborTwo);
    nodeQueue.push(header->neighborThree);

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        if (current->neighborTwo == nullptr) {
            ic.push_back(rule[idxR(current->data, current->neighborOne->data, 0, 0)]);
        } else {
            ic.push_back(rule[idxR(current->data, current->neighborOne->data, current->neighborTwo->data, current->neighborThree->data)]);
            nodeQueue.push(current->neighborTwo);
            nodeQueue.push(current->neighborThree);
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

void CAonCT_reduced_rules(long int vertices, int rule, TreeNode* tree, vector<int>& lV) {
    lV[0] = INT_MAX; lV[1] = 0; lV[2] = INT_MIN;
    int totalIdx = pow(2, vertices);
    vector<vector<long int>> confs(totalIdx);
    vector<long int> tempV(vertices, 0);
    vector<long int> RULE(8, 0);
    to_binary(RULE, rule);
    unordered_set<vector<long int>, VectorHasher> fCr;
    vector<long int> flag(totalIdx, 0);
    for (int i = 0; i < totalIdx; i++) {
        to_binary(tempV, i);
        confs[i] = tempV;
        fill(tempV.begin(), tempV.end(), 0);
    }
    vector<int> confsDerived;
   // int m = 0;
    int looplength = 0;
    for (const auto& tt : confs) {
        vector<long int> iCr(tt);
        long int idR = toDeci(iCr, vertices);
        if (flag[idR] != 0) continue;
        confsDerived.push_back(idR);
   //     cout << idR << "->";
        while (verificationOfConfigurationReduced(iCr, fCr)) {
          insertDataIntoTree(tree, iCr);
            makeiCR(iCr, tree, RULE, flag, vertices);
            idR = toDeci(iCr, vertices);
            confsDerived.push_back(idR);
      //      cout << idR << "->";
        }
        looplength = calLoop(confsDerived);
        if (looplength < lV[0]) lV[0] = looplength;
        if (looplength > lV[2]) lV[2] = looplength;
        // cout << endl;
        fCr.clear();
        confsDerived.clear();
    }
}
// date : 18-12-2024

int searchZero(const vector<long int>&arr){
	for(long int value : arr) {
		if (value == 0) return 1;
	}
	return 0;
}
void iCM(vector<long int>& ic, TreeNode* header, const vector<long int>& rule) {
    ic.clear();
    queue<TreeNode*> nodeQueue;
    ic.push_back(rule[idxR(header->data, header->neighborOne->data, header->neighborTwo->data, header->neighborThree->data)]);
    nodeQueue.push(header->neighborOne);
    nodeQueue.push(header->neighborTwo);
    nodeQueue.push(header->neighborThree);

    while (!nodeQueue.empty()) {
        TreeNode* current = nodeQueue.front();
        nodeQueue.pop();

        if (current->neighborTwo == nullptr) {
            ic.push_back(rule[idxR(current->data, current->neighborOne->data, 0, 0)]);
        } else {
            ic.push_back(rule[idxR(current->data, current->neighborOne->data, current->neighborTwo->data, current->neighborThree->data)]);
            nodeQueue.push(current->neighborTwo);
            nodeQueue.push(current->neighborThree);
        }
    }
}
long int CA_G(const vector<long int>&Tr, long int conf, TreeNode*tree, long int vertices) {
	vector<long int> temp (vertices, 0);
	to_binary(temp, conf);
        insertDataIntoTree(tree, temp);
	iCM(temp, tree, Tr);
	return to_deci(temp, vertices);
	
}
int main() {
    int order = 2;long int height;
    cout << "height: ";
    cin >> height;
    long int V = calculateVertexCount(height, order);
    cout << "total vertices: " << V << endl;
    ofstream myfile("output.txt", ios::trunc);
    if (!myfile) {
        cerr << "file not created or did not open successfully" << endl;
        return 1;
    }
    unique_ptr<TreeNode> CT(createCayleyTree(height, V)); // Correct number of arguments
    if (!CT) {
        cerr << "Cayley tree creation failed" << endl;
        return 1;
    }
//int rule = 125;
    vector<int> lengthVector(3);
    myfile << "RULE\tMIN cl\tMAX cl" << endl;
    cout << "RULE\tMIN cl\tMAX cl" << endl;
    for (int rule = 0; rule < 256; rule++) {
        CAonCT_reduced_rules(V, rule, CT.get(), lengthVector);
        myfile << rule << '\t' << lengthVector[0] << '\t' << lengthVector[2] << '\n';
        cout << rule << '\t' << lengthVector[0] << '\t' << lengthVector[2] << endl;
    }
    myfile.close();
    return 0;
}
