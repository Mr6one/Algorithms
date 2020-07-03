#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::string;

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

int main()
{
    vector<int> zFunction;
    int value;
    while (cin >> value){
        zFunction.push_back(value);
    }
    zFunction[0] = 0;
    cout<<StringByZFunction(zFunction);
    return 0;
}
