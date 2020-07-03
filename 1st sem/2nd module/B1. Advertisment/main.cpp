#include <iostream>
#include <vector>

using namespace std;

struct Term{
    int in;
    int out;
    bool operator>(Term& term){
    if ((in==term.in && out>term.out) || in>term.in){
        return true;
    } else {
        return false;
    }
}
};

class Heap {
public:
    Heap(int n){
        term.resize(n);
        size=n;
    }
    ~Heap(){}

    int current_last;
    void AddElement(int, int, int);
    void SiftDown(int);
    void BuildHeap();
    void Swap(int);
    int SizeReturn();
    int TermInReturn(int);
    int TermOutReturn(int);

private:
    vector<Term> term;
    int size;
};

int Heap::SizeReturn(){
    return size;
}

int Heap::TermInReturn(int i){
    return term[i].in;
}

int Heap::TermOutReturn(int i){
    return term[i].out;
}

void Heap::AddElement(int in, int out, int i){
    term[i].in=in;
    term[i].out=out;
}

void Heap::SiftDown(int i){
    int left,right,top;
    int j=i;
    while (j<=current_last/2-1){
        left=2*j+1;
        right=2*j+2;
        top=j;
        if (term[left]>term[top] && left<current_last){
            top=left;
        }

        if (term[right]>term[top] && right<current_last){
            top=right;
        }

        if (top==j){
            break;
        } else {
            swap(term[top], term[j]);
            j=top;
        }

    }
}

void Heap::BuildHeap(){
    for (int i=size/2-1; i>=0; i--){
        SiftDown(i);
    }
}

void Heap::Swap(int current_last){
    Term temp=term[0];
    term[0]=term[current_last-1];
    term[current_last-1]=temp;
}

int MinAd(Heap& heap){
    int firstAd=-1;
    int lastAd=-1;
    int countAd=0;
    for (int i=0; i<heap.SizeReturn(); i++){
        if (heap.TermInReturn(i)>lastAd){
            countAd+=2;
            firstAd=heap.TermOutReturn(i)-1;
            lastAd=heap.TermOutReturn(i);
        } else if (heap.TermInReturn(i)==lastAd){
            countAd++;
            firstAd=lastAd;
            lastAd=heap.TermOutReturn(i);
        } else if (heap.TermInReturn(i)>firstAd && heap.TermOutReturn(i)>lastAd){
            countAd++;
            firstAd=lastAd;
            lastAd=heap.TermOutReturn(i);
        } else if (heap.TermInReturn(i)>firstAd && heap.TermOutReturn(i)==lastAd){
            countAd++;
            firstAd=heap.TermOutReturn(i)-1;
        } else if (heap.TermInReturn(i)>firstAd && heap.TermOutReturn(i)<lastAd){
            countAd+=2;
            firstAd=heap.TermOutReturn(i)-1;
            lastAd=heap.TermOutReturn(i);
        }
    }
    return countAd;
}



int main()
{
    int n;
    cin>>n;
    Heap heap(n);
    heap.current_last=n;
    int in,out;
    for (int i=0; i<n; i++){
        cin>>in>>out;
        heap.AddElement(in, out, i);
    }
    heap.BuildHeap();
    while (heap.current_last>1){
        heap.Swap(heap.current_last);
        heap.current_last--;
        heap.SiftDown(0);
    }
    int min=MinAd(heap);
    cout<<min;
    return 0;
}
