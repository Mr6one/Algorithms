#include <iostream>
#include <vector>
#include <string>

using namespace std;

const int K = 256;

void CountingSort(vector<string>& a, int left, int right, vector<int>& c, int current_r){
    char j;

    for (int i=right; i<left; i++){
        if (current_r<a[i].size()){
            j=a[i][current_r];
        } else {
            j='\0';
        }
        c[j]++;
    }
    int sum=right;
    for (int i=0; i<K+1; i++){
        int temp=c[i];
        c[i]=sum;
        sum=sum+temp;
    }

    vector<string> b(left-right);
    for (int i=right; i<left; i++){
        if (current_r<a[i].size()){
            j=a[i][current_r];
        } else {
            j='\0';
        }
        b[c[j]-right]=a[i];
        c[j]++;
    }

    for (int i=right; i<left; i++){
        a[i]=b[i-right];
    }
}

void MSDSort(vector<string>& a, int left, int right, int current_length, int max_length){
    if (left-right<=1){
        return;
    }
    vector<int> c(K+1);

    c[K]=left;
    for (int i=0; i<K; i++){
        c[i]=0;
    }

    CountingSort(a, left, right, c, current_length);

    if (current_length<max_length){
        for(int i = 0; i < K; i++){
            MSDSort(a, c[i+1], c[i], current_length+1, max_length);
        }
    }
}

int main()
{
    vector<string> a;
    string str;
    int max_length=0;
    for (;;){
        getline(cin, str);
        if (str.length()>max_length){
            max_length=str.length();
        }
        if (str.empty()){
            break;
        }
        a.push_back(str);
    }

    MSDSort(a, a.size(), 0, 0, max_length);

    for (int i=0; i<a.size(); i++){
        cout<<a[i]<<endl;
    }
    return 0;
}
