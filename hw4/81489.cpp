#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef pair<int, int> alphaBeta;
int max(int x, alphaBeta y)
{
    return max(x, max(y.first, y.second));
}
int min(int x, alphaBeta y)
{
    return min(x, min(y.first, y.second));
}

const char PLAYER_ONE = 'X';
const char PLAYER_TWO = 'O';
const char EMPTY_TILE = '_';

class Board
{
private:
    void copyBoard(const char source[3][3])
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                board[i][j] = source[i][j];
            }
        }
    }

    void initBoard()
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                board[i][j] = EMPTY_TILE;
            }
        }
    }

public:
    char board[3][3];
    Board() { this->initBoard(); }
    Board(const Board &other) { this->copyBoard(other.board); }
    Board(const char otherBoard[3][3]) { this->copyBoard(otherBoard); }
    Board &operator=(const Board &other)
    {
        this->copyBoard(other.board);
        return *this;
    }

    bool isTerminal()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (board[i][0] == board[i][1] &&
                board[i][1] == board[i][2] &&
                (board[i][2] == PLAYER_ONE || board[i][2] == PLAYER_TWO))
            {
                return true;
            }

            if (board[0][i] == board[1][i] &&
                board[1][i] == board[2][i] &&
                (board[2][i] == PLAYER_ONE || board[2][i] == PLAYER_TWO))
            {
                return true;
            }
        }

        if (board[0][0] == board[1][1] &&
            board[1][1] == board[2][2] &&
            (board[2][2] == PLAYER_ONE || board[2][2] == PLAYER_TWO))
        {
            return true;
        }

        if (board[2][0] == board[1][1] &&
            board[1][1] == board[0][2] &&
            (board[0][2] == PLAYER_ONE || board[0][2] == PLAYER_TWO))
        {
            return true;
        }

        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                if (board[i][j] == EMPTY_TILE)
                {
                    return false;
                }
            }
        }

        return true;
    }

    const char getWinner()
    {
        for (int i = 0; i < 3; ++i)
        {
            if (board[i][0] == board[i][1] &&
                board[i][1] == board[i][2] &&
                board[i][2] != EMPTY_TILE)
            {
                return board[i][0];
            }

            if (board[0][i] == board[1][i] &&
                board[1][i] == board[2][i] &&
                board[2][i] != EMPTY_TILE)
            {
                return board[0][i];
            }
        }

        if (board[0][0] == board[1][1] &&
            board[1][1] == board[2][2] &&
            board[2][2] != EMPTY_TILE)
        {
            return board[1][1];
        }

        if (board[2][0] == board[1][1] &&
            board[1][1] == board[0][2] &&
            board[0][2] != EMPTY_TILE)
        {
            return board[1][1];
        }

        return EMPTY_TILE;
    }

    bool checkInput(int x, int y)
    {

        if (x < 0 || x > 2 || y < 0 || y > 2)
        {
            cout << "Coordinates should be between 1 and 3!\n";
            return false;
        }
        if (board[x][y] != EMPTY_TILE)
        {
            cout << "Cell already taken!\n";
            return false;
        }

        return true;
    }

    void setField(int x, int y, const char symbol)
    {
        board[x][y] = symbol;
    }

    const char getField(int x, int y)
    {
        return board[x][y];
    }

    friend ostream &operator<<(ostream &os, const Board &item)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                os << item.board[i][j] << " ";
            }
            os << endl;
        }
    }
};

vector<Board> generateStates(Board board, const char symbol = PLAYER_TWO)
{
    vector<Board> children;

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board.getField(i, j) == EMPTY_TILE)
            {
                Board child(board);
                child.setField(i, j, symbol);
                children.push_back(child);
            }
        }
    }

    return children;
}

int play(Board board, alphaBeta currentAlphaBeta, const bool isMin)
{
    if (board.isTerminal())
    {
        return board.getWinner() == EMPTY_TILE ? 0 : board.getWinner() == PLAYER_TWO ? 1 : -1;
    }

    vector<Board> children = generateStates(board, isMin ? PLAYER_ONE : PLAYER_TWO);
    int size = children.size();
    int returnValue = isMin ? 10 : -10;
    for (int i = 0; i < size; ++i)
    {
        int result = play(children[i], currentAlphaBeta, !isMin);
        if (isMin)
        {
            currentAlphaBeta.second = min(currentAlphaBeta.second, result);
            returnValue = min(returnValue, result);
        }
        else
        {
            currentAlphaBeta.first = max(currentAlphaBeta.first, result);
            returnValue = max(returnValue, result);
        }

        if (currentAlphaBeta.first >= currentAlphaBeta.second)
        {
            break;
        }
    }

    return returnValue;
}

void play(Board &board)
{
    alphaBeta source(INT32_MIN, INT32_MAX);

    vector<Board> states = generateStates(board);
    int size = states.size();

    Board nextMove;

    for (int i = 0; i < size; ++i)
    {
        int result = play(states[i], source, true);
        if (source.first < result)
        {
            nextMove = states[i];
            source.first = result;
        }
    }

    board = nextMove;
}

int main()
{
    Board board;
    cout << board;

    int x, y;

    while (!board.isTerminal())
    {
        cout << "Input position to play: ";
        cin >> x >> y;
        --x;
        --y;

        if (!board.checkInput(x, y) && !board.isTerminal())
        {
            continue;
        }

        board.setField(x, y, PLAYER_ONE);
        cout << board << endl;
        if (board.isTerminal())
        {
            break;
        }
        cout << "Now, computer's turn:\n";
        play(board);
        cout << board << endl;
    }

    char winner = board.getWinner();

    if (winner == EMPTY_TILE)
    {
        cout << "Draw!\n";
    }
    else
    {
        cout << "The winner is player " << winner << "!\n";
    }

    return 0;
}