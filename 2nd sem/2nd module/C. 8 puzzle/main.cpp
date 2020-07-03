#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <unordered_map>

using namespace std;

const int num = 3;
const int aim = 87654321;

class State{
public:
    State(vector<vector<int>>& configuration_, int nullPos): nullPos(nullPos){
        configuration=0;
        for (int i=num-1; i>=0; --i){
            for (int j=num-1; j>=0; --j){
                configuration=configuration*10+configuration_[i][j];
            }
        }
    }

    int GetElement(int, int)const;
    int GetConfiguration()const;
    int GetNullPos()const;
    vector<vector<int>> TransformationFromNumberToVector()const;
private:
    int configuration;
    int nullPos;
};

int State::GetElement(int i, int j)const{
    int positionInNumber=i*num+j;
    return (configuration/int((pow(10, positionInNumber))))%10;
}

int State::GetConfiguration()const{
    return configuration;
}

int State::GetNullPos()const{
    return nullPos;
}

vector<vector<int>> State::TransformationFromNumberToVector()const{
    vector<vector<int>> configiration_;
    int number=configuration;
    for (int i=0; i<num; ++i){
        vector<int> line;
        for (int j=0; j<num; ++j){
            line.push_back(number%10);
            number=number/10;
        }
        configiration_.push_back(line);
    }
    return configiration_;
}

State NewConfiguration(const State& state, int x_first_elem, int y_first_elem, int x_second_elem, int y_second_elem){
    vector<vector<int>> conf=state.TransformationFromNumberToVector();
    swap(conf[x_first_elem][y_first_elem], conf[x_second_elem][y_second_elem]);
    State newState(conf, num*x_second_elem+y_second_elem);
    return newState;
}

bool InversionCheking(const State& state){
    int inversionNum=0;
    for (int i=0; i<num; ++i){
        for (int j=0; j<num; ++j){
            for (int k=j+i*num; k<num*num; ++k){
                if (state.GetElement(k/num, k%num)!=0){
                    if (state.GetElement(i, j)>state.GetElement(k/num, k%num)){
                        ++inversionNum;
                    }
                }
            }
        }
    }
    if (inversionNum%2==0){
        return true;
    } else {
        return false;
    }
}

void GenerateNewStates(vector<pair<State, char>>& numbers, const State& state){
    int i=state.GetNullPos()/num;
    int j=state.GetNullPos()%num;
    if (i-1>=0){
        numbers.push_back(make_pair(NewConfiguration(state, i, j, i-1, j), 'U'));
    }
    if (j-1>=0){
        numbers.push_back(make_pair(NewConfiguration(state, i, j, i, j-1), 'L'));
    }
    if (i+1<num){
        numbers.push_back(make_pair(NewConfiguration(state, i, j, i+1, j), 'D'));
    }
    if (j+1<num){
        numbers.push_back(make_pair(NewConfiguration(state, i, j, i, j+1), 'R'));
    }
}

void FindPath(const State& state, unordered_map<int, pair<char, int>>& history){
    queue<State> myQueue;
    myQueue.push(state);
    while(!myQueue.empty()){
        State nextStates=myQueue.front();
        if (nextStates.GetConfiguration()==aim){
            break;
        }
        myQueue.pop();

        vector<pair<State, char>> nextConfigurations;
        GenerateNewStates(nextConfigurations, nextStates);
        for (size_t i=0; i<nextConfigurations.size(); ++i){
            if (history.find(nextConfigurations[i].first.GetConfiguration())==history.end()){
                history.emplace(nextConfigurations[i].first.GetConfiguration(), make_pair(nextConfigurations[i].second, nextStates.GetConfiguration()));
                myQueue.push(nextConfigurations[i].first);
            }
        }
    }
}

State Input(){
    vector<vector<int>> conf;
    int nullPos;
    for (int i=0; i<num; ++i){
        vector<int> line;
        for (int j=0; j<num; ++j){
            int number;
            cin>>number;
            if (number==0){
                    nullPos=i*num+j;
            }
            line.push_back(number);
        }
        conf.push_back(line);
    }
    State state(conf, nullPos);
    return state;
}

void Output(const State& configuration, unordered_map<int, pair<char, int>>& parents){
    vector<char> path;
    path.push_back(parents[aim].first);
    for (int j=parents[aim].second; j!=configuration.GetConfiguration(); j=parents[j].second){
        path.push_back(parents[j].first);
    }
    cout<<path.size()<<endl;
    for (int i=path.size()-1; i>=0; --i){
        cout<<path[i];
    }
}

int main()
{
    State configuration=Input();
    if (!InversionCheking(configuration)){
        cout<<-1;
        return 0;
    }
    unordered_map<int, pair<char, int>> parents;
    FindPath(configuration, parents);
    Output(configuration, parents);
    return 0;
}
