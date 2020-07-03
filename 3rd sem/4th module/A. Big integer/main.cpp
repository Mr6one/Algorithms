#include "biginteger.h"

int main()
{
    BigInteger b1;
    cin >> b1;
    string str;
    cin >> str;
    BigInteger b = str;
    cout << b1 + b << endl;
    cout << b1 - b << endl;
    cout << b1 * b << endl;
    cout << b1 / b << endl;
    cout << b1 % b << endl;
    if (!b1) {
        cout << "lol" << endl;
    }
    return 0;
}
