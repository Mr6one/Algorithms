#include <iostream>
#include <vector>

using namespace std;

struct Node{
    Node(int element){
        sum=maxSum=maxPref=maxSuf=element;
    }
    Node(){}
    int sum;
    int maxSum;
    int maxPref;
    int maxSuf;
};

class SegmentTree{
public:
    explicit SegmentTree(int n, vector<int>& _array): segmentTree(n, 0){
        SegmentTreeBuild(_array, 0, 0, _array.size()-1);
    }

    Node Query(int vert, int left, int right, int leftPointer, int rightPointer)const;
private:
    vector<Node> segmentTree;
    void SegmentTreeBuild(vector<int>& _array, int vert, int left, int right);
};

Node FindMax(const Node& leftNode, const Node& rightNode);

void SegmentTree::SegmentTreeBuild(vector<int>& _array, int vert, int left, int right){
    if (left==right){
        segmentTree[vert]=Node(_array[left]);
    } else {
        int mid=(left+right)/2;
        SegmentTreeBuild(_array, 2*vert+1, left, mid);
        SegmentTreeBuild( _array, 2*vert+2, mid+1, right);
        segmentTree[vert]=FindMax(segmentTree[2*vert+1], segmentTree[2*vert+2]);
    }
}

Node SegmentTree::Query(int vert, int left, int right, int leftPointer, int rightPointer)const{
    if (leftPointer==left && rightPointer==right){
        return segmentTree[vert];
    } else {
        int mid=(leftPointer+rightPointer)/2;
        if (right<=mid){
            return Query(vert*2+1, left, right, leftPointer, mid);
        }
        if (left>mid){
            return Query(vert*2+2, left, right, mid+1, rightPointer);
        }
        return FindMax(Query(vert*2+1, left, mid, leftPointer, mid), Query(vert*2+2, mid+1, right, mid+1, rightPointer));
    }
}

Node FindMax(const Node& leftNode, const Node& rightNode){
    Node newNode;
    newNode.sum=leftNode.sum+rightNode.sum;
    newNode.maxPref=max(leftNode.maxPref, leftNode.sum+rightNode.maxPref);
    newNode.maxSuf=max(rightNode.maxSuf, rightNode.sum+leftNode.maxSuf);
    newNode.maxSum=max(max(leftNode.maxSum, rightNode.maxSum), leftNode.maxSuf+rightNode.maxPref);

    return newNode;
}

int main()
{
    int n;
    cin>>n;
    while (!cin.eof()){
        int m;
        cin>>m;
        vector<int> _array;
        for (int i=0; i<n; ++i){
            int element;
            cin>>element;
            _array.push_back(element);
        }
        SegmentTree segmentTree(4*n, _array);
        for (int i=0; i<m; ++i){
            int left, right;
            cin>>left>>right;
            cout<<segmentTree.Query(0, left-1, right-1, 0, _array.size()-1).maxSum<<endl;
        }
    	cin>>n;
    }
    return 0;
}
