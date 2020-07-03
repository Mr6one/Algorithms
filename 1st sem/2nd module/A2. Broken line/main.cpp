#include <iostream>
#include <vector>

using namespace std;

struct Point{
    int x;
    int y;
    bool operator>(const Point& point){
        return x==point.x && y>point.y || x>point.x;
	}
};

void Insertion_Sort(vector<Point>& point,int N){
    struct Point current;
	int i;
    for (int j=1; j<N; j++){
        current=point[j];
        for (i=j-1; i>=0; i--){
            if (point[i]>current){
                point[i+1]=point[i];
            } else {
                break;
            }
        }
        point[i+1]=current;
    }
}

int main()
{
    int N;
    cin>>N;
    vector<Point> point(N);
    for (int i=0; i<N; i++){
        cin>>point[i].x;
        cin>>point[i].y;
    }
    cout<<endl;
    Insertion_Sort(point, N);
    for (int i=0; i<N; i++){
        cout<<point[i].x<<' ';
        cout<<point[i].y<<endl;
    }
    return 0;
}
