#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

const size_t alphabetSize = 26;

void CountingSort(const string& str, vector<int>& sufArray){
    vector<int> positions(alphabetSize, 0);
    for (size_t i = 0; i < str.size(); ++i){
        ++positions[str[i] - 'a'];
    }
    int startPosition = 0;
    for (size_t i = 0; i < alphabetSize; ++i){
        int tmp;
        tmp = positions[i];
        positions[i] = startPosition;
        startPosition += tmp;
    }

    for (size_t i = 0; i < str.size(); ++i){
        sufArray[positions[str[i] - 'a'] + 1] = i;
        ++positions[str[i] - 'a'];
    }
    sufArray[0] = str.size();
}

void Initialization(const string& str, vector<int>& sufArray, vector<int>& classes){
    CountingSort(str, sufArray);

    int classNumber = 1;
    classes[sufArray[0]] = 1;
    for (size_t i = 1; i < sufArray.size(); ++i){
        if (str[sufArray[i]] != str[sufArray[i - 1]]){
            ++classNumber;
        }
        classes[sufArray[i]] = classNumber;
    }
}

void Conversion(vector<int>& sufArray, vector<int>& classes, size_t currentLength){
    vector<int> tmpSufArray(sufArray.size());
    for (size_t i = 0; i < sufArray.size(); ++i){
        tmpSufArray[i] = (sufArray[i] - currentLength) % classes.size();
    }
    vector<int> positions(classes.size() + 1, 0);
    for (size_t i = 0; i < classes.size(); ++i){
        ++positions[classes[i]];
    }
    int startPosition = 0;
    for (size_t i = 0; i < classes.size(); ++i){
        int tmp;
        tmp = positions[i];
        positions[i] = startPosition;
        startPosition += tmp;
    }
    for (size_t i = 0; i < classes.size(); ++i){
        sufArray[positions[classes[tmpSufArray[i]]]] = tmpSufArray[i];
        ++positions[classes[tmpSufArray[i]]];
    }
    vector<int> tmpClasses(classes.size());
    int classNumber = 1;
    tmpClasses[sufArray[0]] = classNumber;
    for (size_t i = 1; i < classes.size(); ++i){
        int firstPartFirstSubstring = classes[sufArray[i - 1]];
        int secondPartFirstSubstring = classes[(sufArray[i - 1] + currentLength) % classes.size()];
        int firstPartSecondSubstring = classes[sufArray[i]];
        int secondPartSecondSubstring = classes[(sufArray[i] + currentLength) % classes.size()];
        if (firstPartFirstSubstring != firstPartSecondSubstring || secondPartFirstSubstring != secondPartSecondSubstring){
            ++classNumber;
        }
        tmpClasses[sufArray[i]] = classNumber;
    }
    classes = move(tmpClasses);
}

void BuildLCP(const string& str, vector<int>& sufArray){
    vector<int> inverseArray(sufArray.size());
    for (size_t i = 0; i < sufArray.size(); ++i){
        inverseArray[sufArray[i]] = i;
    }

    vector<int> lcp(sufArray.size());
    int length = 0;
    while (str[length % str.size()] == str[(sufArray[inverseArray[0] + 1] + length) % str.size()]){
        ++length;
    }
    lcp[inverseArray[0]] = length;
    for (size_t i = 1; i < str.size(); ++i){
        length = max(0, lcp[inverseArray[i - 1]] - 1);
        if (inverseArray[i] == sufArray.size() - 1){
            lcp[inverseArray[i]] = -1;
            continue;
        }
        int currentSuffix = sufArray[inverseArray[i] + 1];
        while (str[(currentSuffix + length) % str.size()] != str[(i + length) % str.size()]){
            ++length;
        }
        lcp[inverseArray[i]] = length;
    }
    for (int i: lcp){
        cout<<i<<endl;
    }
}

void BuildSufArray(const string& str){
    vector<int> sufArray(str.size() + 1);
    vector<int> classes(str.size() + 1);
    Initialization(str, sufArray, classes);

    size_t currentLength = 1;
    while (currentLength < str.size()){
        Conversion(sufArray, classes, currentLength);
        currentLength *= 2;
    }

    BuildLCP(str, sufArray);
}

int main()
{
    string str;
    cin>>str;
    BuildSufArray(str);
    return 0;
}
