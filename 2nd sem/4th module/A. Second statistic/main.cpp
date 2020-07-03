#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>

using namespace std;

pair<int, int> GetTwoMinElements(pair<int, int> firstPair, pair<int, int> secondPair){
    if (firstPair.first == firstPair.second && secondPair.first == secondPair.second && firstPair.second == secondPair.first){
        return firstPair;
    }

    int firstElem;
    int secondElem;
    if (firstPair.first < secondPair.first){
        firstElem=firstPair.first;
        if (firstPair.second < secondPair.first && firstPair.second != firstElem){
            secondElem=firstPair.second;
        } else if (firstPair.second > secondPair.first && secondPair.first != firstElem){
            secondElem=secondPair.first;
        } else {
            secondElem=secondPair.first;
        }
    } else if (firstPair.first > secondPair.first) {
        firstElem=secondPair.first;
        if (secondPair.second < firstPair.first && secondPair.second != firstElem){
            secondElem=secondPair.second;
        } else if (secondPair.second > firstPair.first && firstPair.first != firstElem){
            secondElem=firstPair.first;
        } else {
            secondElem=firstPair.first;
        }
    } else {
        firstElem=secondPair.first;
        if (firstPair.second < secondPair.second  && firstPair.second != firstElem){
            secondElem=firstPair.second;
        } else {
            secondElem=secondPair.second;
        }
    }

    return make_pair(firstElem, secondElem);
}

class SparseTable{
public:
    SparseTable(int n, vector<int>& _array);

    int GetSecondStatistic(int left, int right)const;
private:
    int log_2;
    vector<vector<pair<int, int>>> sparseTable;
};

SparseTable::SparseTable(int n, vector<int>& _array){
    log_2 = static_cast<int>(log2(n));
    sparseTable.resize(log_2+1);
    for (int i=0; i<=log_2; ++i){
        sparseTable[i].resize(n+1);
    }
    for (size_t i=0; i<=_array.size(); ++i){
        sparseTable[0][i] =make_pair(_array[i], _array[i]);
    }
    for (int i=1; i<=log_2; ++i){
        for (int j=0; j<=n-static_cast<int>(pow(2, i-1)); ++j){
            sparseTable[i][j]=GetTwoMinElements(sparseTable[i-1][j], sparseTable[i-1][j+static_cast<int>(pow(2, i-1))]);
        }
    }
}

int SparseTable::GetSecondStatistic(int left, int right)const{
    int lenth=right-left+1;
    pair<int, int> ans=GetTwoMinElements(sparseTable[static_cast<int>(log2(lenth))][left],
                                         sparseTable[static_cast<int>(log2(lenth))][right-int(pow(2, static_cast<int>(log2(lenth))))+1]);

    return ans.second;
}

int main()
{
    int n, m;
    cin>>n>>m;
    vector<int> _array;
    for (int i=0; i<n; ++i){
        int element;
        cin>>element;
        _array.push_back(element);
    }
    SparseTable sparseTable(n, _array);
    for (int i=0; i<m; ++i){
        int left, right;
        cin>>left>>right;
        cout<<sparseTable.GetSecondStatistic(left-1, right-1)<<endl;
    }
    return 0;
}
