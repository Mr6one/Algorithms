#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int PrefixFunction(string& str, int previousValue, int i, vector<int>& prefixFunction){
    while ((str[previousValue] != str[i]) && (previousValue > 0)){
        previousValue = prefixFunction[previousValue - 1];
    }
    if (str[previousValue] == str[i]){
        return ++previousValue;
    } else {
        return 0;
    }
}

void StringMatching(string& str, vector<int>& prefixFunction, int sz){
    int currentValue = 0;
    for (size_t i = sz; i < str.length(); ++i){
        currentValue = PrefixFunction(str, currentValue, i, prefixFunction);
        if (currentValue == sz - 1){
           cout<<i - currentValue - sz + 1<<' ';
        }
    }
}

void StringMatching(string& str, string& pattern){
    vector<int> prefixFunction(pattern.length());
    prefixFunction[0] = 0;
    for (size_t i = 1; i < pattern.length(); ++i){
        prefixFunction[i] = PrefixFunction(pattern, prefixFunction[i - 1], i, prefixFunction);
    }
    string newStr = pattern + "#" + str;
    StringMatching(newStr, prefixFunction, pattern.length() + 1);
}

int main()
{
    string pattern;
    cin>>pattern;
    string str;
    cin>>str;
    StringMatching(str, pattern);
    return 0;
}
