#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::string;
using std::vector;
using std::pair;

int const alphabetSize = 26;

struct Node{
    Node(int parent, char letter): parent(parent), letter(letter), suffix(-1), isTerminated(false){
        nextNodes.resize(alphabetSize, -1);
        linking.resize(alphabetSize, -1);
    }
    Node(){};

    int parent;
    char letter;
    int suffix;
    bool isTerminated;

    vector<int> nextNodes;
    vector<int> linking;
    vector<int> position;
};

class Bor{
public:
    Bor(int n): bor(n), sz(0){
        bor[0] = Node(0, '0');
        bor[0].suffix = 0;
    }

    void AddVertex(pair<int, int>&, int, string&);
    void SearchingForMatches(string&, vector<pair<int, int>>&, string&, vector<int>&);
private:
    vector<Node> bor;
    int sz;

    int GetSuffix(int);
    int Linking(int, char);
};

void Bor::AddVertex(pair<int, int>& submaskPos, int position, string& str) {
    int currentVertex = 0;
    for (int i = submaskPos.first; i <= submaskPos.second; i++){
        char letter = str[i] - 'a';
        if (bor[currentVertex].nextNodes[letter] == -1){
            ++sz;
            bor[currentVertex].nextNodes[letter] = sz;

            bor[sz] = Node(currentVertex, letter);
        }
        currentVertex = bor[currentVertex].nextNodes[letter];
    }
    bor[currentVertex].isTerminated = true;
    bor[currentVertex].position.push_back(position);
}

int Bor::GetSuffix(int vertex){
    if (bor[vertex].suffix == -1){
        if (bor[vertex].parent == 0){
            bor[vertex].suffix = 0;
        } else {
            bor[vertex].suffix = Linking(GetSuffix(bor[vertex].parent), bor[vertex].letter);
        }
    }
    return bor[vertex].suffix;
}

int Bor::Linking(int vertex, char letter){
    if (bor[vertex].linking[letter] == -1){
        if (bor[vertex].nextNodes[letter] != -1){
            bor[vertex].linking[letter] = bor[vertex].nextNodes[letter];
        } else {
            if (vertex == 0) {
                bor[vertex].linking[letter] = 0;
            } else {
                bor[vertex].linking[letter] = Linking(GetSuffix(vertex), letter);
            }
        }
    }
    return bor[vertex].linking[letter];
}

void Bor::SearchingForMatches(string& str, vector<pair<int, int>>& masksEntry, string& pattern, vector<int>& masksPositions) {
    int vertex = 0;
    for (size_t i = 0; i < str.size(); i++){
        vertex = Linking(vertex, str[i] - 'a');
        int nextVertex = vertex;
        do {
            if (bor[nextVertex].isTerminated){
                for (int position: bor[nextVertex].position){
                    int startIndex = i - masksEntry[position].second + masksEntry[position].first;
                    if ((startIndex - masksEntry[position].first >= 0) && (startIndex - masksEntry[position].first + pattern.size() - 1 < str.size())){
                        ++masksPositions[startIndex - masksEntry[position].first];
                    }
                }
            }
            nextVertex = GetSuffix(nextVertex);
        } while (nextVertex != 0);
    }
}

void MasksEntrySearching(string& mask, vector<pair<int, int>>& masksEntry){
    pair<int, int> currentMasksEntry;
    if (mask[0] != '?'){
        currentMasksEntry.first = 0;
    }
    if (mask[1] == '?' && mask[0] != '?'){
        currentMasksEntry.second = 0;
        masksEntry.push_back(currentMasksEntry);
    }

    for (size_t i = 1; i < mask.size() - 1; i++){
        if (mask[i - 1] == '?' && mask[i] != '?'){
            currentMasksEntry.first = i;
        }
        if (mask[i + 1] == '?' && mask[i] != '?'){
            currentMasksEntry.second = i;
            masksEntry.push_back(currentMasksEntry);
        }
    }

    if (mask[mask.size() - 2] == '?' && mask[mask.size() - 1] != '?'){
        currentMasksEntry.first = static_cast<int>(mask.size() - 1);
    }
    if (mask[mask.size() - 1] != '?'){
        currentMasksEntry.second = static_cast<int>(mask.size() - 1);
        masksEntry.push_back(currentMasksEntry);
    }
}

int main() {
    string pattern, str;
    cin>>pattern>>str;

    Bor bor(pattern.size() + 1);
    vector<pair<int, int>> masksEntry;
    MasksEntrySearching(pattern, masksEntry);
    for (size_t i = 0; i < masksEntry.size(); i++){
        bor.AddVertex(masksEntry[i], i, pattern);
    }

    vector<int> masksPositions(str.size());
    bor.SearchingForMatches(str, masksEntry, pattern, masksPositions);
    for (size_t i = 0; i < masksPositions.size(); i++){
        if (masksPositions[i] == masksEntry.size()){
            std::cout<<i<<' ';
        }
    }

    return 0;
}
