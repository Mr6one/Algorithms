#ifndef BIGINTEGER_H_INCLUDED
#define BIGINTEGER_H_INCLUDED

#include <iostream>
#include <vector>

using namespace std;

class BigInteger{
public:
    BigInteger() {};
    BigInteger(int);
    BigInteger(const string&);

    BigInteger operator-()const;
    BigInteger& operator-=(const BigInteger&);
    BigInteger& operator+=(const BigInteger&);
    friend BigInteger operator+(const BigInteger&, const BigInteger&);
    friend BigInteger operator-(const BigInteger&, const BigInteger&);

    BigInteger& operator/=(const BigInteger&);
    BigInteger& operator%=(const BigInteger&);
    friend BigInteger operator/(const BigInteger&, const BigInteger&);
    friend BigInteger operator%(const BigInteger&, const BigInteger&);

    BigInteger& operator*=(const BigInteger&);
    friend BigInteger operator*(const BigInteger&, const BigInteger&);

    BigInteger& operator--();
    BigInteger& operator++();
    BigInteger operator--(int);
    BigInteger operator++(int);

    bool operator<=(const BigInteger&) const;
    bool operator>=(const BigInteger&) const;
    bool operator>(const BigInteger&) const;
    bool operator<(const BigInteger&) const;
    bool operator!=(const BigInteger&) const;
    bool operator==(const BigInteger&) const;

    friend istream& operator>>(istream& input, BigInteger&);
    friend ostream& operator<<(ostream& output, const BigInteger&);

    explicit operator bool();
    string  toString() const;
private:
    vector<int> number;
    bool isNegative;

    BigInteger Sum(BigInteger&, const BigInteger&);
    BigInteger Differnce(BigInteger&, const BigInteger&);
    vector<int> Mul(const vector<int>&, const vector<int>&);
    BigInteger Div(const BigInteger&, const BigInteger&, bool);
    bool AbsLessCompare(const BigInteger&) const;
    BigInteger Abs(const BigInteger&);
    void RemoveLeadingZeros();
    void Normalize();
};

BigInteger::BigInteger (int num) {
    number.clear();
    if (num < 0) {
        isNegative = true;
        num = -num;
    } else if (num == 0) {
        number.push_back(0);
        isNegative = false;
    } else {
        isNegative = false;
    }
    while (num != 0) {
        number.push_back(num % 10);
        num /= 10;
    }
}

BigInteger::BigInteger(const string& str) {
    if (str.empty()) {
        isNegative = false;
        number.push_back(0);
        return;
    }
    number.clear();
    for (size_t i = str.size() - 1; i >= 1; --i) {
        number.push_back(str[i] - '0');
    }
    if (str[0] == '-') {
        isNegative = true;
    } else {
        number.push_back(str[0] - '0');
        isNegative = false;
    }
    if (number.size() == 1 && number[0] == 0) {
        isNegative = false;
    }
}

istream& operator>>(istream& input, BigInteger& bigInt){
    string str;
    input >> str;
    bigInt = BigInteger(str);
    bigInt.RemoveLeadingZeros();
    return input;
}

ostream& operator<<(ostream& output, const BigInteger& bigInt){
    string str = bigInt.toString();
    output << str;
    return output;
}

string BigInteger::toString() const {
    string str = "";
    if (isNegative) {
        str += '-';
    }

    for (ssize_t i = number.size() - 1; i >= 0; --i) {
        str += number[i] + '0';
    }

    if (str.size() == 0 || (str.size() == 1 && str[0] == '-')) {
        str = '0';
    }
    return str;
}

BigInteger BigInteger::operator-() const{
    BigInteger b = *this;
    b.isNegative = !isNegative;
    if (b.number.size() == 1 && b.number[0] == 0) {
        b.isNegative = false;
    }
    return b;
}

bool BigInteger::operator<(const BigInteger& bigInt) const {
    if (isNegative != bigInt.isNegative) {
        return isNegative;
    } else {
        if (isNegative) {
            return bigInt.AbsLessCompare(*this);
        } else {
            return AbsLessCompare(bigInt);
        }
    }
}

bool BigInteger::operator>(const BigInteger& bigInt) const {
    return -*this < -bigInt;
}

bool BigInteger::operator!=(const BigInteger& bigInt) const {
    return (*this < bigInt || *this > bigInt);
}

bool BigInteger::operator==(const BigInteger& bigInt) const {
    return !(*this != bigInt);
}

bool BigInteger::operator<=(const BigInteger& bigInt) const {
    return (*this < bigInt || *this == bigInt);
}

bool BigInteger::operator>=(const BigInteger& bigInt) const {
    return (*this > bigInt || *this == bigInt);
}

BigInteger::operator bool(){
    return *this != 0;
}

BigInteger& BigInteger::operator+=(const BigInteger& bigInt) {
    if (!(isNegative ^ bigInt.isNegative)) {
        *this = Sum(*this, bigInt);
        return *this;
    }
    BigInteger b = bigInt;
    if (isNegative) {
        if (Abs(*this) < bigInt) {
            *this = Differnce(b, *this);
            isNegative = false;
        } else {
            *this = Differnce(*this, b);
        }
    } else {
        if (*this < Abs(bigInt)) {
            *this = Differnce(b, *this);
            isNegative = true;
        } else {
            *this = Differnce(*this, b);
        }
    }
    return *this;
}

BigInteger& BigInteger::operator++() {
    *this += 1;
    return *this;
}

BigInteger BigInteger::operator++(int) {
    BigInteger b = *this;
    *this += 1;
    return b;
}

BigInteger operator+(const BigInteger& first, const BigInteger& second) {
    BigInteger b = first;
    b += second;
    return b;
}

BigInteger& BigInteger::operator-=(const BigInteger& bigInt) {
    *this += -bigInt;
    return *this;
}

BigInteger& BigInteger::operator--() {
    *this -= 1;
    return *this;
}

BigInteger BigInteger::operator--(int) {
    BigInteger b = *this;
    *this -= 1;
    return b;
}

BigInteger operator-(const BigInteger& first, const BigInteger& second) {
    BigInteger b = first;
    b -= second;
    return b;
}

BigInteger& BigInteger::operator*=(const BigInteger& bigInt) {
    size_t maxSize = max(number.size(), bigInt.number.size());
    size_t k = 1;
    while (k < maxSize) {
        k <<= 1;
    }
    BigInteger b = bigInt;
    number.resize(k);
    b.number.resize(k);
    number = Mul(number, b.number);
    Normalize();
    RemoveLeadingZeros();
    if ((isNegative && !bigInt.isNegative) || (!isNegative && bigInt.isNegative)) {
        isNegative = true;
    } else if (isNegative && bigInt.isNegative) {
        isNegative = false;
    }
    return *this;
}

BigInteger operator*(const BigInteger& first, const BigInteger& second) {
    BigInteger b = first;
    b *= second;
    return b;
}

BigInteger& BigInteger::operator/=(const BigInteger& bigInt) {
    if (bigInt == 0) {
        exit(0);
    }
    *this = Div(*this, bigInt, true);
    return *this;
}

BigInteger operator/(const BigInteger& first, const BigInteger& second) {
    BigInteger b = first;
    b /= second;
    return b;
}

BigInteger& BigInteger::operator%=(const BigInteger& bigInt) {
    *this = Div(*this, bigInt, false);
    return *this;
}

BigInteger operator%(const BigInteger& first, const BigInteger& second) {
    BigInteger b = first;
    b %= second;
    return b;
}

BigInteger BigInteger::Sum(BigInteger& first, const BigInteger& second) {
    int carry = 0;
    for (size_t i = 0; i < max(first.number.size(), second.number.size()) || carry; ++i) {
        if (i == first.number.size()) {
            first.number.push_back(0);
        }
        if (i < second.number.size()) {
            first.number[i] += (second.number[i] + carry);
        } else {
            first.number[i] += carry;
        }
        carry = first.number[i] / 10;
        first.number[i] %= 10;
    }
    first.RemoveLeadingZeros();
    return first;
}


BigInteger BigInteger::Differnce(BigInteger& first, const BigInteger& second){
    int carry = 0;
    for (size_t i = 0; i < max(first.number.size(), second.number.size()) || carry; ++i) {
        if (i < second.number.size()) {
            first.number[i] -= (second.number[i] + carry);
        } else {
            first.number[i] -= carry;
        }
        if (first.number[i] < 0) {
            carry = 1;
            first.number[i] += 10;
        } else {
            carry = 0;
        }
    }
    first.RemoveLeadingZeros();
    return first;
}

vector<int> BigInteger::Mul(const vector<int>& first, const vector<int>& second) {
    if (first.size() <= 1) {
        return {first[0] * second[0], 0};
    }
    size_t mid = first.size() / 2;
    vector<int> leftHalfFirst {first.begin(), first.begin() + mid};
    vector<int> rightHalfFirst {first.begin() + mid, first.end()};
    vector<int> leftHalfSecond {second.begin(), second.begin() + mid};
    vector<int> rightHalfSecond {second.begin() + mid, second.end()};

    vector<int> tmp1 = Mul(rightHalfFirst, rightHalfSecond);
    vector<int> tmp2 = Mul(leftHalfFirst, leftHalfSecond);
    vector<int> sumFirst(mid);
    vector<int> sumSecond(mid);
    for (size_t i = 0; i < mid; ++i) {
        sumFirst[i] = leftHalfFirst[i] + rightHalfFirst[i];
        sumSecond[i] = leftHalfSecond[i] + rightHalfSecond[i];
    }
    vector<int> tmp3 = Mul(sumFirst, sumSecond);
    vector<int> res(first.size() * 2, 0);
    for (size_t i = 0; i < first.size(); ++i) {
        res[i] = tmp2[i];
    }
    for (size_t i = first.size(); i < first.size() * 2; ++i) {
        res[i] = tmp1[i - first.size()];
    }
    for (size_t i = mid; i < mid + first.size() ; ++i) {
        res[i] += (tmp3[i - mid] - tmp2[i - mid] - tmp1[i - mid]);
    }
    return res;
}

BigInteger BigInteger::Div(const BigInteger& first, const BigInteger& second, bool flag) {
    BigInteger result;
    result.isNegative = false;
    result.number.resize(first.number.size());
    BigInteger rest;
    rest.isNegative = false;
    for (ssize_t i = first.number.size() - 1; i >= 0; --i) {
        rest.number.insert(rest.number.begin(), first.number[i]);
        rest.RemoveLeadingZeros();
        size_t left = 0, right = 9;
        int num = 0;
        while (left <= right) {
            size_t mid = (left + right) / 2;
            BigInteger tmp = mid * Abs(second);
            if (tmp <= rest) {
                num = mid;
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        result.number[i] = num;
        rest -= num * Abs(second);
    }
    if (flag) {
        if ((first.isNegative && !second.isNegative) || (!first.isNegative && second.isNegative)) {
            result.isNegative = true;
        } else if (first.isNegative && second.isNegative) {
            result.isNegative = false;
        }
        result.RemoveLeadingZeros();
        return result;
    } else {
        rest.RemoveLeadingZeros();
        rest.isNegative = first.isNegative;
        return rest;
    }
}

void BigInteger::RemoveLeadingZeros() {
    while (number.back() == 0 && number.size() > 1) {
        number.pop_back();
    }
    if (number.size() == 1 && number[0] == 0) {
        isNegative = false;
    }
}

bool BigInteger::AbsLessCompare(const BigInteger& bigInt) const {
    if (number.size() != bigInt.number.size()) {
        return number.size() < bigInt.number.size();
    } else {
        for (ssize_t i = number.size() - 1; i >= 0; --i) {
            if (number[i] < bigInt.number[i]) {
                return true;
            }
            if (number[i] > bigInt.number[i]) {
                return false;
            }
        }
    }
    return false;
}

void BigInteger::Normalize() {
    number.push_back(0);
    for (size_t i = 0; i < number.size() - 1; ++i) {
        number[i + 1] += number[i] / 10;
        number[i] %= 10;
    }
}

BigInteger BigInteger::Abs(const BigInteger& bigInt){
    BigInteger b = bigInt;
    b.isNegative = false;
    return b;
}

#endif // BIGINTEGER_H_INCLUDED
