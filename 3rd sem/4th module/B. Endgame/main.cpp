#include <iostream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max() / 2;

const int maxStateNumber = 9000;

const pair<int, int> whiteKingPos = make_pair(3, 3);

const vector<pair<int, int>> directions = {
    make_pair(0, 1),
    make_pair(0, -1),
    make_pair(1, 0),
    make_pair(-1, 0),
    make_pair(1, 1),
    make_pair(1, -1),
    make_pair(-1, -1),
    make_pair(-1, 1)
};

struct State{
    State(const pair<int, int>&, const pair<int, int>&, bool);
    State(int);
    pair<int, int> whiteQueenPos;
    pair<int, int> blackKingPos;
    bool whiteTurn;

    int IntConversion() const;
};

State::State(const pair<int, int>& whiteQueenPos, const pair<int, int>& blackKingPos, bool whiteTurn):
    whiteQueenPos(whiteQueenPos), blackKingPos(blackKingPos), whiteTurn(whiteTurn){}

int State::IntConversion() const {
    int st;
    if (whiteTurn) {
        st = 1;
    } else {
        st = 0;
    }
    st <<= 3;
    st |= (whiteQueenPos.first - 1) & 7;
    st <<= 3;
    st |= (whiteQueenPos.second - 1) & 7;
    st <<= 3;
    st |= (blackKingPos.first - 1) & 7;
    st <<= 3;
    st |= (blackKingPos.second - 1) & 7;
    return st;
}

class Solver{
public:
    Solver(State&);
private:
    queue<State> moves;
    vector<int> moveNumberToMate;

    bool IsMovePossible(const pair<int, int>&);
    bool IsKingTreaten(const pair<int, int>&, const pair<int, int>&);
    bool IsQueenTreaten(const pair<int, int>&, const pair<int, int>&);
    bool IsMate(const State&);

    vector<State> FindMate();
    vector<State> QueenMoves(const State&);
    vector<State> KingMoves(const State&);

    int QueenMoveAnalysis(const State&);
    int KingMoveAnalysis(const State&);
};

Solver::Solver(State& startState):moveNumberToMate(maxStateNumber) {
    for (size_t qx = 1; qx <= 8; ++qx) {
        for (size_t qy = 1; qy <= 8; ++qy) {
            for (size_t kx = 1; kx <= 8; ++kx) {
                for (size_t ky = 1; ky <= 8; ++ky) {
                    State state(make_pair(qx, qy), make_pair(kx, ky), true);
                    moveNumberToMate[state.IntConversion()] = INF;
                    state = State(make_pair(qx, qy), make_pair(kx, ky), false);
                    moveNumberToMate[state.IntConversion()] = INF;
                }
            }
        }
    }
    vector<State> mates = FindMate();
    for (State st: mates) {
        for (State state: QueenMoves(st)) {
            moves.push(state);
        }
    }
    while (!moves.empty() && moveNumberToMate[startState.IntConversion()] == INF) {
        State newState = moves.front();
        moves.pop();
        if (newState.whiteTurn) {
            if (!IsQueenTreaten(newState.whiteQueenPos, newState.blackKingPos) && !IsKingTreaten(whiteKingPos, newState.blackKingPos)) {
                int minMoves = QueenMoveAnalysis(newState);
                if (moveNumberToMate[newState.IntConversion()] > minMoves + 1) {
                    moveNumberToMate[newState.IntConversion()] = minMoves + 1;
                    for (State nextStates: KingMoves(newState)) {
                        moves.push(nextStates);
                    }
                }
            }
        } else {
            int maxMoves = KingMoveAnalysis(newState);
            if (maxMoves != INF) {
                if (moveNumberToMate[newState.IntConversion()] == INF || moveNumberToMate[newState.IntConversion()] < maxMoves + 1) {
                    moveNumberToMate[newState.IntConversion()] = maxMoves + 1;
                    for (State nextStates: QueenMoves(newState)) {
                        moves.push(nextStates);
                    }
                }
            }
        }
    }
    cout << moveNumberToMate[startState.IntConversion()];
}

int Solver::QueenMoveAnalysis(const State& state) {
    int minMoves = INF;
    for (pair<int, int> direction: directions) {
        for (size_t i = 1; i <= 8; ++i)  {
            pair<int, int> newWhiteQueenPos = make_pair(state.whiteQueenPos.first + i * direction.first,
                                                        state.whiteQueenPos.second + i * direction.second);
            if (IsMovePossible(newWhiteQueenPos)) {
                if (newWhiteQueenPos == whiteKingPos || newWhiteQueenPos == state.blackKingPos) {
                    break;
                }
                if (!IsKingTreaten(state.blackKingPos, newWhiteQueenPos) || IsKingTreaten(whiteKingPos, newWhiteQueenPos)) {
                    State newState(newWhiteQueenPos, state.blackKingPos, false);
                    if (moveNumberToMate[newState.IntConversion()] < minMoves) {
                        minMoves = moveNumberToMate[newState.IntConversion()];
                    }
                }
            }
        }
    }
    return minMoves;
}

int Solver::KingMoveAnalysis(const State& state) {
    int maxMoves = -INF;
    for (pair<int, int> direction: directions) {
        pair<int, int> newBlackKingPos = make_pair(state.blackKingPos.first + direction.first,
                                                    state.blackKingPos.second + direction.second);
        if (IsMovePossible(newBlackKingPos) && newBlackKingPos != state.whiteQueenPos && newBlackKingPos != whiteKingPos
            && !IsQueenTreaten(state.whiteQueenPos, newBlackKingPos) && !IsKingTreaten(whiteKingPos, newBlackKingPos)) {
                State newState(state.whiteQueenPos, newBlackKingPos, true);
                if (moveNumberToMate[newState.IntConversion()] > maxMoves) {
                    maxMoves = moveNumberToMate[newState.IntConversion()];
                }
        }
    }
    return maxMoves;
}

bool Solver::IsMate(const State& state) {
    if (!IsQueenTreaten(state.whiteQueenPos, state.blackKingPos)) {
        return false;
    }
    for (pair<int, int> direction: directions) {
        pair<int, int> newBlackKingPos = make_pair(state.blackKingPos.first + direction.first,
                                                    state.blackKingPos.second + direction.second);
        if (IsMovePossible(newBlackKingPos) && !IsQueenTreaten(state.whiteQueenPos, newBlackKingPos)
            && !IsKingTreaten(whiteKingPos, newBlackKingPos)) {
                return false;
            }
    }
    return true;
}

vector<State> Solver::FindMate() {
    vector<State> mates;
    for (int kx = 1; kx <= 8; ++kx) {
        for (int ky = 1; ky <= 8; ++ky) {
            for (int qx = 1; qx <= 8; ++qx) {
                for (int qy = 1; qy <= 8; ++qy) {
                    pair<int, int> newQueenPos = make_pair(qx, qy);
                    if (newQueenPos != make_pair(kx, ky) && newQueenPos != whiteKingPos) {
                        State state(newQueenPos, make_pair(kx, ky), false);
                        if (IsMate(state)) {
                            moveNumberToMate[state.IntConversion()] = 0;
                            mates.push_back(state);
                        }
                    }
                }
            }
        }
    }
    return mates;
}

bool Solver::IsMovePossible(const pair<int, int>& position) {
    return (position.first >= 1 && position.first <= 8) && (position.second >= 1 && position.second <= 8);
}

bool Solver::IsKingTreaten(const pair<int, int>& kingPos, const pair<int, int>& pos) {
    if ((abs(kingPos.second - pos.second) == 1)) {
        return abs(kingPos.first - pos.first) <= 1;
    } else if (abs(kingPos.second - pos.second) == 0) {
        return abs(kingPos.first - pos.first) == 1;
    }
    return false;
}

bool Solver::IsQueenTreaten(const pair<int, int>& queenPos, const pair<int, int>& kingPos) {
    for (pair<int, int> direction: directions) {
        for (size_t i = 1; i <= 8; ++i) {
            pair<int, int> newWhiteQueenPos = make_pair(queenPos.first + i * direction.first,
                                                        queenPos.second + i * direction.second);
            if (IsMovePossible(newWhiteQueenPos)) {
                if (newWhiteQueenPos == whiteKingPos) {
                    break;
                }
                if (newWhiteQueenPos == kingPos) {
                    return true;
                }
            }
        }
    }
    return false;
}

vector<State> Solver::QueenMoves(const State& state) {
    vector<State> newStates;
    for (pair<int, int> direction: directions) {
        for (size_t i = 1; i <= 8; ++i) {
            pair<int, int> newWhiteQueenPos = make_pair(state.whiteQueenPos.first + i * direction.first,
                                                        state.whiteQueenPos.second + i * direction.second);
            if (IsMovePossible(newWhiteQueenPos)) {
                if (newWhiteQueenPos == whiteKingPos || newWhiteQueenPos == state.blackKingPos) {
                    break;
                }
                State newState(newWhiteQueenPos, state.blackKingPos, true);
                newStates.push_back(newState);
            }
        }
    }
    return newStates;
}

vector<State> Solver::KingMoves(const State& state) {
    vector<State> newStates;
    for (pair<int, int> direction: directions) {
        pair<int, int> newBlackKingPos = make_pair(state.blackKingPos.first + direction.first,
                                                    state.blackKingPos.second + direction.second);
        if (IsMovePossible(newBlackKingPos) && newBlackKingPos != state.whiteQueenPos && newBlackKingPos != whiteKingPos) {
                State newState(state.whiteQueenPos, newBlackKingPos, false);
                newStates.push_back(newState);
        }
    }
    return newStates;
}

int main()
{
    char qx, kx;
    int qy, ky;
    cin >> qx >> qy >> kx >> ky;
    State state(make_pair(qx - 'a' + 1, qy), make_pair(kx - 'a' + 1, ky), true);
    Solver solver(state);
    return 0;
}
