#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::string;

//Строит минимально-лексикографическую строку по префикс функции
string StringByPrefixFunction(vector<int>& prefixFunction){
    string str = "a";
    for (size_t i = 1; i < prefixFunction.size(); ++i){
        if (prefixFunction[i] != 0){
            str += str[prefixFunction[i] - 1];
        } else {
            int k = prefixFunction[i - 1];
            vector<bool> usedLetters(26);
            while (k > 0){
                usedLetters[str[k] - 'a'] = true;
                k = prefixFunction[k - 1];
            }
            int j = 1;
            char nextLetter = 'b';
            while (usedLetters[j]){
                ++j;
                ++nextLetter;
            }
            str += nextLetter;
        }
    }
    return str;
}

//Строит минимально-лексикографическую строку по z-фукции
string StringByZFunction(vector<int>& zFunction){
    string str = "a";
    size_t currentIndex = 1;
    vector<int> usedIndex;
    while (currentIndex < zFunction.size()){
        if (zFunction[currentIndex] != 0){
            int remainingZBlockLength = zFunction[currentIndex];
            size_t index = 0;
            while (remainingZBlockLength > 0){
                if (zFunction[currentIndex] > remainingZBlockLength){
                    remainingZBlockLength = zFunction[currentIndex];
                    index = 0;
                }
                if (zFunction[currentIndex] == remainingZBlockLength){
                    usedIndex.push_back(zFunction[currentIndex]);
                }

                str += str[index];
                --remainingZBlockLength;
                ++currentIndex;
                ++index;
            }
        } else {
            vector<bool> usedLetters(26);
            for (int index: usedIndex){
                usedLetters[str[index] - 'a'] = true;
            }
            usedIndex.clear();
            int j = 1;
            char nextLetter = 'b';
            while (usedLetters[j]){
                ++j;
                ++nextLetter;
            }
            str += nextLetter;

            ++currentIndex;

        }
    }
    return str;
}

//Строит префик функцию по строке
vector<int> BuildingPrefixFunction(string str){
    vector<int> prefixFunction(str.length());
    prefixFunction[0] = 0;
    for (size_t i = 1; i < str.length(); ++i){
        int previousValue = prefixFunction[i - 1];
        while ((str[previousValue] != str[i]) && (previousValue > 0)){
            previousValue = prefixFunction[previousValue - 1];
        }
        if (str[previousValue] == str[i]){
            prefixFunction[i] = previousValue + 1;
        } else {
            prefixFunction[i] = 0;
        }
    }
    return prefixFunction;
}

//Строит z-функцию по строке
vector<int> BuildingZFunction(string str){
    vector<int> zFunction(str.length());
    size_t left = 0;
    size_t right = 0;
    for (size_t i = 0; i < str.length(); ++i){
        if (i > right){
            zFunction[i] = 0;
            while (i + zFunction[i] < str.length() && str[zFunction[i]] == str[i + zFunction[i]]){
                ++zFunction[i];
            }
            left = i;
            right = i + zFunction[i] - 1;
        } else {
           if (i + zFunction[i - left] <= right){
                zFunction[i] = zFunction[i - left];
           } else {
                size_t j = 1;
                while (right + j < str.length() && str[right - i + j] == str[right + j]){
                    ++j;
                }
                left = i;
                right = right + j - 1;
                zFunction[i] = right - left + 1;
            }
        }
    }
    return zFunction;
}

//Строит префикс функцию по z
vector<int> BuildingPrefixFunctionByZ(vector<int>& zFunction){
    return BuildingPrefixFunction(StringByZFunction(zFunction));
}

//Строит z функцию по префикс
vector<int> BuildingZFunctionByPrefix(vector<int>& prefixFunction){
    return BuildingZFunction(StringByPrefixFunction(prefixFunction));
}

void VectorCout(vector<int>& arr){
    for (int i: arr){
        cout<<i<<' ';
    }
    cout<<'\n';
}

int main()
{
    string str;
    cin>>str;

    vector<int> zFunction = BuildingZFunction(str);
    VectorCout(zFunction);

    vector<int> prefixFunction = BuildingPrefixFunction(str);
    VectorCout(prefixFunction);

    cout<<StringByZFunction(zFunction)<<'\n';

    cout<<StringByPrefixFunction(prefixFunction)<<'\n';

    zFunction = BuildingZFunctionByPrefix(prefixFunction);
    VectorCout(zFunction);

    prefixFunction = BuildingPrefixFunctionByZ(zFunction);
    VectorCout(prefixFunction);

    return 0;
}
