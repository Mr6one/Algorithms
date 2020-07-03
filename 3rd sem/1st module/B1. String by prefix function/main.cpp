#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::string;

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

int main()
{
    vector<int> prefixFunction;
    int value;
    while (cin >> value){
        prefixFunction.push_back(value);
    }
    cout<<StringByPrefixFunction(prefixFunction);
    return 0;
}
