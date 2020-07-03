#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>

using namespace std;

const int num = 4;
const int maxDeapth = 80;

const unsigned long long aim=1147797409030816545;
const unsigned long long firstLastMove=1311768467463003900;
const unsigned long long secondLastMove=1311768467463790095;

const unsigned long long Masks[] = {
        0xFFFFFFFFFFFFFFF0, 0xFFFFFFFFFFFFFF0F, 0xFFFFFFFFFFFFF0FF,
        0xFFFFFFFFFFFF0FFF, 0xFFFFFFFFFFF0FFFF, 0xFFFFFFFFFF0FFFFF,
        0xFFFFFFFFF0FFFFFF, 0xFFFFFFFF0FFFFFFF, 0xFFFFFFF0FFFFFFFF,
        0xFFFFFF0FFFFFFFFF, 0xFFFFF0FFFFFFFFFF, 0xFFFF0FFFFFFFFFFF,
        0xFFF0FFFFFFFFFFFF, 0xFF0FFFFFFFFFFFFF, 0xF0FFFFFFFFFFFFFF,
        0x0FFFFFFFFFFFFFFF
};

class State{
public:
    State(vector<int>& configuration_, int nullPos): nullPos(nullPos){
        for (size_t i=0; i<configuration_.size(); ++i){
            configuration=(configuration&Masks[i])|(static_cast<unsigned long long>(configuration_[i]))<<(i*4);
        }
    }
    unsigned long long GetConfiguration()const;
    int GetNullPos()const;
    bool IsRightMoveAvailable()const;
    bool IsLeftMoveAvailable()const;
    bool IsUpMoveAvailable()const;
    bool IsDownMoveAvailable()const;

private:
    unsigned long long configuration=0;
    int nullPos;
};

unsigned long long State::GetConfiguration()const{
    return configuration;
}

int State::GetNullPos()const{
    return nullPos;
}

bool State::IsRightMoveAvailable()const{
    return nullPos % num != num-1;
}

bool State::IsLeftMoveAvailable()const{
    return nullPos % num != 0;
}

bool State::IsUpMoveAvailable()const{
    return nullPos >= num;
}

bool State::IsDownMoveAvailable()const{
    return nullPos <= num*(num-1)-1;
}

bool Search(const State& state, int deapthSearch, int currentDeapth, string& ans, unordered_set<unsigned long long>& visited, int& newDeapth);

void SetElem(unsigned long long& configuration, int elem, int pos){
    configuration = (configuration&Masks[pos])|(static_cast<unsigned long long>(elem))<<(pos*4);
}

unsigned long long NewConfiguration(unsigned long long configuration, int firstPosition, int secondPosition){
    int firstElem = (configuration>>(firstPosition*num))&15;
    int secondElem = (configuration>>(secondPosition*num))&15;
    SetElem(configuration, secondElem, firstPosition);
    SetElem(configuration, firstElem, secondPosition);
    return configuration;
}

bool InversionCheking(unsigned long long configuration, int nullPos){
    int inversionNum=0;
    for (int i=0; i<num*num; ++i){
        for (int j=i+1; j<num*num; ++j){
            if ((int)((configuration>>(i*num))&15)>(int)((configuration>>(j*num))&15) && (int)((configuration>>(j*num))&15)!=0){
                ++inversionNum;
            }
        }
    }
    if ((inversionNum+nullPos/num%2+1)%2==0){
        return true;
    } else {
        return false;
    }
}

void ManhattanDistance(unsigned long long configuration, int& heuristic){
    for (int i=0; i<num*num; ++i){
        if (((configuration>>(i*num))&15)==0){
            continue;
        }
        heuristic+=(abs ( ((((int)((configuration>>(i*num))&15)-1))>>2)-(i>>2)) + abs( ((int)((configuration>>(i*4))&15)-1)%num-i%num));
    }
}

void LastMove(unsigned long long configuration, int& heuristic){
    if (configuration==firstLastMove || configuration==secondLastMove){
        heuristic+=2;
    }
}

void LinearConflict(unsigned long long configuration, int& heuristic, int nullPos){
    for (int i=0; i<num; i++){
        for (int j=0; j<num; j++){
            for (int k=j+1; k<num; k++) {
                int firstPos=num*i+j;
                int secondPos=num*i+k;
                int firstElement=((configuration>>(firstPos*num))&15)-1;
                int secondElement=((configuration>>(secondPos*num))&15)-1;
                if (firstPos!=nullPos && secondPos!=nullPos && firstElement/num==i && secondElement/num==i && firstElement>secondElement) {
                    heuristic+=2;
                }
            }
        }
    }
    for (int i=0; i<num; i++){
        for (int j=0; j<num; j++){
            for (int k=i+1; k<num; k++) {
                int firstPos=num*i+j;
                int secondPos=num*k+j;
                int firstElement=((configuration>>(firstPos*num))&15)-1;
                int secondElement=((configuration>>(secondPos*num))&15)-1;
                if (firstPos!=nullPos && secondPos!=nullPos && firstElement%num==j && secondElement%num==j && firstElement>secondElement) {
                    heuristic+=2;
                }
            }
        }
    }
}

int Estimation(unsigned long long configuration, int nullPos){
    int heuristic=0;

    ManhattanDistance(configuration, heuristic);
    LastMove(configuration, heuristic);
    LinearConflict(configuration, heuristic, nullPos);

    return 1.1*heuristic;
}

vector<int> TransformationToVector(unsigned long long configuration){
    vector<int> configuration_;
    for (int i=0; i<num*num; ++i){
        configuration_.push_back((configuration>>(i*num))&15);
    }
    return configuration_;
}

State Up(unsigned long long configuration, int nullPos){
    unsigned long long newConfiguration=NewConfiguration(configuration, nullPos, nullPos-num);
    vector<int> configuration_=TransformationToVector(newConfiguration);
    State state(configuration_, nullPos-num);
    return state;
}

State Left(unsigned long long configuration, int nullPos){
    unsigned long long newConfiguration=NewConfiguration(configuration, nullPos, nullPos-1);
    vector<int> configuration_=TransformationToVector(newConfiguration);
    State state(configuration_, nullPos-1);
    return state;
}

State Down(unsigned long long configuration, int nullPos){
    unsigned long long newConfiguration=NewConfiguration(configuration, nullPos, nullPos+num);
    vector<int> configuration_=TransformationToVector(newConfiguration);
    State state(configuration_, nullPos+num);
    return state;
}

State Right(unsigned long long configuration, int nullPos){
    unsigned long long newConfiguration=NewConfiguration(configuration, nullPos, nullPos+1);
    vector<int> configuration_=TransformationToVector(newConfiguration);
    State state(configuration_, nullPos+1);
    return state;
}

bool CheckingForSolution(const State& state){
    if (state.GetConfiguration()==aim){
        return true;
    } else {
        return false;
    }
}

bool Recalculation(const State& state, int deapthSearch, int currentDeapth, string& ans,
                         unordered_set<unsigned long long>& visited, int& newDeapth){
    if (CheckingForSolution(state)){
        return true;
    }

    if (visited.find(state.GetConfiguration())==visited.end()){
        int f=currentDeapth+Estimation(state.GetConfiguration(), state.GetNullPos());
        if (f<=deapthSearch){
            visited.emplace(state.GetConfiguration());
            if (Search(state, deapthSearch, currentDeapth+1, ans, visited, newDeapth)){
                return true;
            }
            visited.erase(state.GetConfiguration());
        } else {
            newDeapth=min(newDeapth, f);
        }
    }
    return false;
}

bool SetAnswer(State& state, int deapthSearch, int currentDeapth, string& ans, unordered_set<unsigned long long>& visited, int& newDeapth, char nextMove){
    ans+=nextMove;
    if (Recalculation(state, deapthSearch, currentDeapth, ans, visited, newDeapth)){
        return true;
    } else {
        ans.pop_back();
    }
    return false;
}

bool Search(const State& state, int deapthSearch, int currentDeapth, string& ans,
            unordered_set<unsigned long long>& visited, int& newDeapth){
    if (state.IsUpMoveAvailable()){
        State newState=Up(state.GetConfiguration(), state.GetNullPos());
        if (SetAnswer(newState, deapthSearch, currentDeapth, ans, visited, newDeapth, 'D')){
            return true;
        }
    }
    if (state.IsDownMoveAvailable()){
        State newState=Down(state.GetConfiguration(), state.GetNullPos());
        if (SetAnswer(newState, deapthSearch, currentDeapth, ans, visited, newDeapth, 'U')){
            return true;
        }
    }
    if (state.IsLeftMoveAvailable()){
        State newState=Left(state.GetConfiguration(), state.GetNullPos());
        if (SetAnswer(newState, deapthSearch, currentDeapth, ans, visited, newDeapth, 'R')){
            return true;
        }
    }
    if (state.IsRightMoveAvailable()){
        State newState=Right(state.GetConfiguration(), state.GetNullPos());
        if (SetAnswer(newState, deapthSearch, currentDeapth, ans, visited, newDeapth, 'L')){
            return true;
        }
    }
    return false;
}

void MinMovementsSearch(const State& state, string& ans){
    int deapthSearch=Estimation(state.GetConfiguration(), state.GetNullPos());
    int newDeapth;
    unordered_set<unsigned long long> visited;
    while (true){
        newDeapth=maxDeapth;
        visited.emplace(state.GetConfiguration());
        if (Search(state, deapthSearch, 0, ans, visited, newDeapth)){
            break;
        }
        deapthSearch=newDeapth;
        visited.clear();
    }
}

int main()
{
    int nullPos;
    vector<int> configuration;
    for (int i=0; i<num*num; ++i){
        int number;
        cin>>number;
        if (number==0){
            nullPos=i;
        }
        configuration.push_back(number);
    }
    State state(configuration, nullPos);
    if (!InversionCheking(state.GetConfiguration(), state.GetNullPos())){
        cout<<-1;
        return 0;
    }

    string ans="";
    MinMovementsSearch(state, ans);
    cout<<ans.size()<<endl;
    cout<<ans;
    return 0;
}
