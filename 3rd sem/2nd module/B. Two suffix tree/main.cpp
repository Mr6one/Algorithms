#include <iostream>
#include <stdio.h>
#include <climits>
#include <vector>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::map;

const size_t INF = INT_MAX;

struct Node{
    Node(int position, int length): position(position), length(length){}
    Node(){}

    int position;
    int length;
};

class SuffixTree{
public:
    SuffixTree(int n): suffixTree(n), nodeInfo(n), link(n), sz(1), currentSuffixLength(0), currentSuffixPosition(0), currentNode(0){
        nodeInfo[0] = Node(0, INF);
    }

    void AddSuffix(int, const string&);
    size_t GetSize();
    void CoutSufTree(int, size_t&, const string& str1, const string& str2);
private:
    vector<map<int, int>> suffixTree;
    vector<Node> nodeInfo;
    vector<int> link;
    size_t sz;
    int currentSuffixLength;
    int currentSuffixPosition;
    int currentNode;

    void TransitionToNextNode(const string&);
};

size_t SuffixTree::GetSize(){
    return sz;
}

void SuffixTree::TransitionToNextNode(const string& str){
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
        TransitionToNextNode(str);

        int nextNode = suffixTree[currentNode][str[currentSuffixPosition]];
        if (nextNode == 0){
            suffixTree[currentNode][str[currentSuffixPosition]] = sz;
            nodeInfo[sz] = Node(currentSuffixPosition, INF);
            ++sz;

            link[lastNode] = currentNode;
            lastNode = nextNode;
        } else {
            int sym = str[nodeInfo[nextNode].position + currentSuffixLength - currentSuffixPosition - 1];
            if (sym == symbol){
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
        }

        if (currentNode == 0){
            ++currentSuffixPosition;
        } else {
            currentNode = link[currentNode];
        }
    }
}

void SuffixTree::CoutSufTree(int k, size_t& counter, const string& str1, const string& str2){
    size_t tmp = counter;
    if (suffixTree[k].size() != 0){
        for (auto j = suffixTree[k].begin(); j != suffixTree[k].end(); ++j){
            printf("%d ", tmp);
            if (nodeInfo[j->second].position < str1.size()){
                printf("0 ");
                printf("%d ", nodeInfo[j->second].position);
                if (nodeInfo[j->second].length == INF){
                    printf("%d\n", str1.size());
                } else if (nodeInfo[j->second].position + nodeInfo[j->second].length <= str1.size()) {
                    printf("%d\n", nodeInfo[j->second].position + nodeInfo[j->second].length);
                } else {
                    printf("%d\n", str1.size());
                }
            } else {
                printf("1 ");
                printf("%d ", nodeInfo[j->second].position - str1.size());
                if (nodeInfo[j->second].length == INF){
                    printf("%d\n", str2.size());
                } else {
                    printf("%d\n", nodeInfo[j->second].position + nodeInfo[j->second].length - str1.size());
                }
            }
            ++counter;
            CoutSufTree(j->second, counter, str1, str2);
        }
    }
}

int main()
{
    string str1, str2;
    cin >> str1 >> str2;
    string str = str1 + str2;
    SuffixTree suffixTree(str.size() * 2);
    for (size_t i = 0; i < str.size(); ++i){
        suffixTree.AddSuffix(str[i], str);
    }
    cout << suffixTree.GetSize() << endl;
    size_t counter = 0;
    suffixTree.CoutSufTree(0, counter, str1, str2);
    return 0;
}
