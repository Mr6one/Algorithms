#include <iostream>
#include <vector>
#include <map>
#pragma GCC optimize("Ofast,unroll-all-loops")

using namespace std;

const int INF = 1000000;

struct Node{
    Node(int position, int length): position(position), length(length){}
    Node(){}

    int position;
    int length;
};

class SuffixTree{
public:
    SuffixTree(int n): sz(1), suffixTree(n), nodeInfo(n), link(n), suf1(n, 0), suf2(n, 0), currentSuffixLength(0), currentSuffixPosition(0), currentNode(0){
        nodeInfo[0] = Node(0, INF);
    }

    void AddSuffix(int, const string&);
    void Go(const string&);
    void CoutSufTree(int, const string& str1, const string& str2);
    void CountSufTree(int, int k, int&, const string&, const string&);
    int sz;
private:
    vector<map<int, int>> suffixTree;
    vector<Node> nodeInfo;
    vector<int> link;
    vector<int> suf1;
    vector<int> suf2;
    int currentSuffixLength;
    int currentSuffixPosition;
    int currentNode;
};

void SuffixTree::Go(const string& str){
    while (currentSuffixLength - currentSuffixPosition > nodeInfo[suffixTree[currentNode][str[currentSuffixPosition]]].length){
        int tmp = currentSuffixPosition;
        currentSuffixPosition += nodeInfo[suffixTree[currentNode][str[currentSuffixPosition]]].length;
        currentNode = suffixTree[currentNode][str[tmp]];
    }
}

void SuffixTree::AddSuffix(int symbol, const string& str){
    ++currentSuffixLength;
    int lastNode = 0;
    while (currentSuffixPosition < currentSuffixLength){
        Go(str);
        int nextNode = suffixTree[currentNode][str[currentSuffixPosition]];
        int sym = str[nodeInfo[nextNode].position + currentSuffixLength - currentSuffixPosition - 1];
        if (nextNode == 0){
            suffixTree[currentNode][str[currentSuffixPosition]] = sz;
            nodeInfo[sz] = Node(currentSuffixPosition, INF);
            ++sz;

            link[lastNode] = currentNode;
            lastNode = nextNode;
        } else if (sym == symbol){
            link[lastNode] = currentNode;
            return;
        } else {
            suffixTree[sz][symbol] = sz + 1;
            suffixTree[sz][sym] = nextNode;
            nodeInfo[sz] = Node(nodeInfo[nextNode].position, currentSuffixLength - currentSuffixPosition - 1);
            ++sz;

            nodeInfo[sz] = Node(currentSuffixLength - 1, INF);
            ++sz;

            suffixTree[currentNode][str[currentSuffixPosition]] = sz - 2;
            int newLength;
            if (nodeInfo[nextNode].length == INF){
                newLength = INF;
            } else {
                newLength = nodeInfo[nextNode].length - (currentSuffixLength - currentSuffixPosition - 1);
            }
            nodeInfo[nextNode] = Node(nodeInfo[nextNode].position + currentSuffixLength - currentSuffixPosition - 1, newLength);

            link[lastNode] = sz - 2;
            lastNode = sz - 2;
        }
        if (currentNode == 0){
            ++currentSuffixPosition;
        } else {
            currentNode = link[currentNode];
        }
    }
}

void SuffixTree::CoutSufTree(int k, const string& str1, const string& str2){
    if (suffixTree[k].size() != 0){
        for (auto j = suffixTree[k].begin(); j != suffixTree[k].end(); ++j){
            if (nodeInfo[j->second].length == INF && [j->second].position < str1.size()){
                suf1[k] = 1;
            }
        }
    }
}

void SuffixTree::CoutSufTree(int pos, int k, int& counter, const string& str1, const string& str2){
    if (k == counter){
        return;
    }
    if (suffixTree[pos].size() != 0){
        for (auto j = suffixTree[pos].begin(); j != suffixTree[pos].end(); ++j){
            if (suf1[j->second] == 1 && suf2[j->second] == 1){
                ++counter;
                CoutSufTree(j->second, counter, devider, str1, str2);
            }
        }
    }
}

int main()
{
    string str1, str2;
    cin >> str1 >> str2;
    str1 += '$';
    str2 += '#';
    string str = str1 + str2;
    SuffixTree suffixTree((int)(str.size() * 2));
    for (size_t i = 0; i < str.size(); ++i){
        suffixTree.AddSuffix(str[i], str);
    }
    int counter = 0;
    int k;
    suffixTree.CoutSufTree(0, counter, str1.size(), str1, str2);
    return 0;
}
