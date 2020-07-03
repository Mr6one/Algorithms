#include <cstdlib>
#include <iostream>
#include <vector>
#include <cstdio>
#include <stack>

using namespace std;

void InsertionSort(vector<int>& a, int left, int right){
    for (int i=left+1; i<=right; i++){
        int elem=a[i];
        int j=i-1;
        while (a[j]>elem && j>=left){
                a[j+1]=a[j];
                j--;
        }
        a[j+1]=elem;
    }
}

int MediansOfThree(vector<int>& a, int left, int right){
    int mid=(left+right)/2;
    if (a[left]<=a[mid]){
        if (a[mid]<=a[right]){
            return mid;
        } else {
            if (a[left]<=a[right]){
                return right;
            } else {
                return left;
            }
        }
    } else {
        if (a[mid]<=a[right]){
        if (a[left]<=a[right]){
            return left;
        }
        else {
            return right;
        }
        }
        else {
            return mid;
        }
    }
}

int partition(vector<int>& a, int left, int right){
    int i=left;
    int j=right-1;
    int pivotIndex=MediansOfThree(a,left,right);
    int pivot=a[pivotIndex];
    swap(a[pivotIndex], a[right]);
    while (i<=j){
        while (a[i]<=pivot && i<right){
            i++;
        }
        while (a[j]>pivot && j>=0){
            j--;
        }
        if (i<j){
            swap(a[i], a[j]);
            i++;
            j--;
        }
    }
    swap(a[i], a[right]);
    return i;
}

void Quicksort(vector<int>& a, int left, int right){
    stack<int>b;
    b.push(0);
    b.push(0);
    while (!b.empty()){
        while (right-left+1>40){
            int pivot=partition(a,left,right);
                b.push(pivot+1);
                b.push(right);
                right=pivot-1;
        }
        InsertionSort(a, left, right);
        if (!b.empty()){
            right=b.top();
            b.pop();
            left=b.top();
            b.pop();
        }
    }
}

int main()
{
    vector<int> a;
    char c=getchar_unlocked();
    while (c!=EOF){
        int x=0;
        while (isdigit(c)){
            x=10*x+static_cast<int>(c-'0');
            c=getchar_unlocked();
        }
        a.push_back(x);
        c=getchar_unlocked();
    }
    Quicksort(a,0,a.size()-1);
    for (int i = 9; i < a.size(); i+=10) {
        cout<<a[i]<<' ';
    }
    return 0;
}
