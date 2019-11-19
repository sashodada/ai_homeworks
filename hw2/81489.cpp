#include <iostream>
#include <cmath>
#include <stack>
#include <functional>
#include <queue>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

int manhattanDist(int value, int row, int col) {
    if (!value) {
        value = 9;
    }
    --value;
    int distRow = value / 3;
    int distCol = value % 3;
    return abs(row - distRow) + abs(col - distCol);
}

int distToGoal(int value, int row, int col) {
    return manhattanDist(value, row, col);
}

bool moveLeft(int mat[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 1; j < 3; ++j) {
            if (!mat[i][j]) {
                swap(mat[i][j], mat[i][j - 1]);
                return true;
            }
        }
    }
    return false;
}

bool moveUp(int mat[3][3]) {
    for (int i = 1; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!mat[i][j]) {
                swap(mat[i][j], mat[i - 1][j]);
                return true;
            }
        }
    }
    return false;
}

bool moveRight(int mat[3][3]) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 2; ++j) {
            if (!mat[i][j]) {
                swap(mat[i][j], mat[i][j + 1]);
                return true;
            }
        }
    }
    return false;
}

bool moveDown(int mat[3][3]) {
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (!mat[i][j]) {
                swap(mat[i][j], mat[i + 1][j]);
                return true;
            }
        }
    }
    return false;
}

class State {
    public:
        int level;
        int state[3][3];

        State(int level, int state[3][3]): level(level) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    this->state[i][j] = state[i][j];
                }
            }
        }

        State& operator = (State other) {
            if (this == &other) {
                return *this;
            }
            this->level = other.level;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    this->state[i][j] = other.state[i][j];
                }
            }
        }

        bool operator == (State other) {
            bool equal = true;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    if (this->state[i][j] != other.state[i][j]) {
                        equal = false;
                    }
                }
            }
            return equal;
        }

        operator int() {
            return level + dist();
        }

        int dist() {
            int res = 0;
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    res += distToGoal(this->state[i][j], i, j);
                }
            }
            return res;
        }
};

void solve(int mat[3][3]) {
    priority_queue<State, vector<State>, greater<int>> myQueue;
    vector<State> used;
    myQueue.push(State(0, mat));

    while (!myQueue.empty()) {
        State current = myQueue.top();
        myQueue.pop();
        used.push_back(current);
        if (current.dist() == 0) {
            cout << current.level << endl;
            return;
        }
        if (moveDown(current.state)) {
            State newOne(current.level + 1, current.state);
            if (find(used.begin(), used.end(), newOne) == used.end()) {
                myQueue.push(newOne);
            }
            moveUp(current.state);
        }
        if (moveUp(current.state)) {
            State newOne(current.level + 1, current.state);
            if (find(used.begin(), used.end(), newOne) == used.end()) {
                myQueue.push(newOne);
            }
            moveDown(current.state);
        }
        if (moveLeft(current.state)) {
            State newOne(current.level + 1, current.state);
            if (find(used.begin(), used.end(), newOne) == used.end()) {
                myQueue.push(newOne);
            }
            moveRight(current.state);
        }
        if (moveRight(current.state)) {
            State newOne(current.level + 1, current.state);
            if (find(used.begin(), used.end(), newOne) == used.end()) {
                myQueue.push(newOne);
            }
            moveLeft(current.state);
        }
    }
    cout << "No found solution\n";
}

int main()
{
    int mat[3][3] = {
        {6, 5, 3},
        {2, 4, 8},
        {7, 0, 1}
    };

    solve(mat);
    return 0;
}
