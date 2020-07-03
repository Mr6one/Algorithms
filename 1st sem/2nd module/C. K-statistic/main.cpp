#include <cstdlib>
#include <iostream>
#include <ctime>
#include <vector>

using namespace std;

int Partition(vector<int>& a, int left, int right){
    int i=right;
    int j=right;
    srand(time(0));
    int random =left+ rand() % (right-left);
    int pivot=a[random];
    swap(a[random], a[left]);
    while (j>0){
        if(a[j]<=pivot){
            j--;
        } else {
            swap(a[i], a[j]);
            i--;
            j--;
        }
    }
    swap(a[i], a[left]);
    return i;
}

int KStatistic(vector<int>& a, int left, int right, int k){
    while (left<right){
        int pivot=Partition(a,left,right);
        if (pivot==k){
            return k;
        } else if (k<pivot){
            right=pivot-1;
        } else {
            left=pivot+1;
        }
    }
    return right;
}

int main()
{
    int n;
    cin>>n;
    vector<int> a;
    int k;
    cin>>k;
    int x;
    for (int i=0; i<n; i++){
        cin>>x;
        a.push_back(x);
    }
    k=KStatistic(a, 0, n-1, k);
        cout<<a[k];
    return 0;
}
