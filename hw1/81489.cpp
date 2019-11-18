#include <iostream>
#include <queue>
#include <cmath>
#include <stack>
using namespace std;

const int INVALID_POSITION = 0;
const int VALID_POSITION = 1;
const int TELEPORT_POSTION = 2;

class Pair {
  public:
    int x;
    int y;

    Pair(): x(-1), y(-1) {}
    Pair(const int& x, const int& y): x(x), y(y) {}
    bool operator ==(const Pair& other)
    {
      return x == other.x && y == other.y;
    }

    bool operator !=(const Pair& other)
    {
      return !(*this == other);
    }

    friend ostream& operator << (ostream& os, Pair& pair)
    {
      os << "(" << pair.x << ", " << pair.y << ")";
      return os;
    }
};

Pair **initPredecessors(const int& fieldSize)
{
  Pair **predecessors = new Pair*[fieldSize];
  for (int i = 0; i < fieldSize; ++i)
  {
    predecessors[i] = new Pair[fieldSize];
  };
  return predecessors;
}

void freePredecessors(Pair **predecessors, const int &fieldSize)
{
  for (int i = 0; i < fieldSize; ++i)
  {
    delete[] predecessors[i];
  }
  delete[] predecessors;
}

bool isPositionValid(Pair &position, int **field, const int &fieldSize, Pair **predecessors)
{
  return position.x >= 0 && position.x < fieldSize &&
      position.y >= 0 && position.y < fieldSize &&
      predecessors[position.x][position.y] == Pair(-1, -1) &&
      field[position.x][position.y] != INVALID_POSITION;
}

void setPredecessor(Pair &position, Pair &predecessor, Pair **predecessors)
{
  predecessors[position.x][position.y] = predecessor;
}

Pair correspondingTeleport(Pair &position, int **field, const int &fieldSize)
{
  for (int i = 0; i < fieldSize; ++i)
  {
    for (int j = 0; j < fieldSize; ++j)
    {
      if (field[i][j] == TELEPORT_POSTION && !(i == position.x && j == position.y))
      {
        return Pair(i, j);
      }
    }
  }
  return Pair(-1, -1);
}

void generateNewPositions(
  Pair currentPosition,
  int **field,
  const int &fieldSize,
  queue<Pair> &positionQueue,
  Pair **predecessors)
{
  for (int i = -1; i <= 1; ++i)
  {
    for (int j = -1 + abs(i); j <= 1; j += 2)
    {
      Pair newPosition(currentPosition.x + i, currentPosition.y + j);
      if (isPositionValid(newPosition, field, fieldSize, predecessors))
      {
        setPredecessor(newPosition, currentPosition, predecessors);
        if (field[newPosition.x][newPosition.y] == TELEPORT_POSTION)
        {
          Pair teleport = correspondingTeleport(newPosition, field, fieldSize);
          setPredecessor(teleport, newPosition, predecessors);
          newPosition = teleport;
        }
        positionQueue.push(newPosition);
      }
    }
  }
}

void solve(int **field, const int &fieldSize, Pair &start, Pair &finish)
{
  Pair **predecessors = initPredecessors(fieldSize);

  queue<Pair> positionQueue;
  positionQueue.push(start);
  predecessors[start.x][start.y] = start;

  while (!positionQueue.empty() && positionQueue.front() != finish)
  {
    Pair currentPosition = positionQueue.front();
    positionQueue.pop();

    generateNewPositions(currentPosition, field, fieldSize, positionQueue, predecessors);
  }

  if (positionQueue.front() == finish)
  {
    stack<Pair> resultPath;
    Pair currentPosition = positionQueue.front();
    do
    {
      resultPath.push(currentPosition);
      currentPosition = predecessors[currentPosition.x][currentPosition.y];
    } while (resultPath.top() != start);
  
    cout << "Path:\n";
    while (!resultPath.empty())
    {
      cout << resultPath.top() << endl;
      resultPath.pop();
    }
  }
  else
  {
    cout << "No path!" << endl;
  }

  freePredecessors(predecessors, fieldSize);
}

int main()
{
  int n = 6;
  int **mat = new int*[n];
  int demoInput[6][6] = {
    {1, 1, 0, 1, 1, 1},
    {1, 2, 0, 0, 1, 1},
    {1, 1, 1, 1, 2, 1},
    {1, 1, 1, 1, 1, 1},
    {1, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1}
  };
  
  for (int i = 0; i < n; ++i)
  {
    mat[i] = new int[n];
    for (int j = 0; j < n; ++j)
    {
      // cin >> mat[i][j];
      mat[i][j] = demoInput[i][j];
    }
  }

  Pair start(0, 0);
  Pair finish(4, 4); 

  solve(mat, n, start, finish);

  for (int i = 0; i < n; ++i)
  {
    delete[] mat[i];
  }
  delete[] mat;
  return 0;
}
