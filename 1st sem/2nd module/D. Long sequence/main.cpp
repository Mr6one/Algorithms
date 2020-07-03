#include <iostream>
#include <vector>

using namespace std;

void Merge (vector<int>& a, int left, int right){
    int mid=(left+right)/2;
    vector<int> b(right-left+1);
    int i=left;
    int j=mid+1;
    int step=0;
    while (i<=mid && j<=right){
        if (a[i]<=a[j]){
            b[step]=a[i];
            i++;
        } else {
            b[step]=a[j];
            j
            ++;
        }
        step++;
    }
    if (i>mid){
        for (; j<=right; j++){
            b[step]=a[j];
            step++;
        }
    }
    if (j>right){
        for (; i<=mid; i++){
            b[step]=a[i];
            step++;
        }
    }
    for (i=0; i<step; i++){
        a[left+i]=b[i];
    }
}

void MergeSort(vector<int>& a, int left, int right){
    if (left>=right){
        return;
    }
    int mid=(left+right)/2;
    MergeSort(a, left, mid);
    MergeSort(a, mid+1, right);
    Merge(a, left, right);
}

int main()
{
    int n;
    cin>>n;
    int k;
    cin>>k;
    vector<int> a(2*k);
    for (int i=0; i<2*k; i++){
        cin>>a[i];
    }
    MergeSort(a, 0, 2*k-1);
    for (int i=0; i<(n-2*k)/k; i++){
        for (int j=k; j<2*k; j++){
            cin>>a[j];
        }
        MergeSort(a, 0, 2*k-1);
    }

    for (int i=k; i<k+n%k; i++){
        cin>>a[i];
    }
    MergeSort(a, 0, k+n%k-1);
    for (int i=0; i<k; i++){
        cout<<a[i]<<' ';
    }
    return 0;
}
