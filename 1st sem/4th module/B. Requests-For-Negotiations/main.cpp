#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Term{
    int inTime;
    int outTime;
};

int CountMeetings(vector<Term>& term){
    int meetingsCounter=1;
    int last=0;
    for (size_t i=1; i<term.size(); ++i){
        if (term[i].inTime>=term[last].outTime){
            ++meetingsCounter;
            last=i;
        }
    }
    return meetingsCounter;
}

bool operator <(Term& term1, Term& term2){
    return term1.outTime<term2.outTime;
}

istream& operator >>(istream& in, Term& terms){
    in>>terms.inTime;
    in>>terms.outTime;
    return in;
}

int main()
{
    vector<Term> terms;
    while (!cin.eof()){
        Term term;
        cin>>term;
        terms.push_back(term);
    }
    sort (terms.begin(), terms.end());
    cout<<CountMeetings(terms);
    return 0;
}
