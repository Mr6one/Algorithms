#include <iostream>
#include <vector>

using namespace std;

long CountingOfPyramids(int n){
    vector<long> value(n+1);
    value[0]=1;
    for (int i=1; i<=n; ++i){
        value[i]=0;
    }
    for (int i=n; i>=1; --i){
        for (int j=1; j<=n; ++j){
            if (j-i>=0){
                value[j]=value[j-i]+value[j];
            }
        }
    }
    return value[n];
}

int main()
{
    int n;
    cin>>n;
    cout<<CountingOfPyramids(n);
    return 0;
}
