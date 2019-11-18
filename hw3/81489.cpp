#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include <algorithm>
#include <queue>

using namespace std;

class Chromosome {
    public:
        Chromosome() {}
};

void parseLine(string &line, vector<int> &cost, vector<int> &value)
{
    int spaceIndex = line.find(' ');
    cost.push_back(atoi(line.substr(0, spaceIndex).c_str()));
    value.push_back(atoi(line.substr(spaceIndex + 1).c_str()));
}

void input(vector<int> &cost, vector<int> &value)
{
    ifstream input("testData.txt", ifstream::in);
    string line;

    while(getline(input, line)) {
        parseLine(line, cost, value);
    }
}

vector<bool> initChromosome(int length)
{
    vector<bool> chromosome(length);
    for (int i = 0; i < length; ++i)
    {
        chromosome[i] = rand() % 2;
    }
    return chromosome;
}

int totalValue(vector<bool> &chromosome, const vector<int> &cost, const vector<int> &value, int size)
{
    int totalCost = 0;
    int totalValue = 0;
    for (int i = 0; i < size; ++i)
    {
        totalCost += chromosome[i] * cost[i];
        totalValue += chromosome[i] * value[i];
    }

    return totalCost > 5000 ? 5000 - totalCost : totalValue;
}

void solve(vector<int> &cost, vector<int> &value)
{
    srand(time(NULL));

    const int POPULATION_SIZE = 100;
    int CHROMOSOME_LENGTH = cost.size();

    auto chromosomeComparator = [cost, value, CHROMOSOME_LENGTH](vector<bool> &left, vector<bool> &right) {
        return totalValue(left, cost, value, CHROMOSOME_LENGTH) < totalValue(right, cost, value, CHROMOSOME_LENGTH);
    };
    priority_queue<vector<bool>, vector<vector<bool>>, decltype(chromosomeComparator)> population(chromosomeComparator);

    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        population.push(initChromosome(CHROMOSOME_LENGTH));
    }

    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        vector<bool> top = population.top();
        cout << totalValue(top, cost, value, CHROMOSOME_LENGTH) << " ";
        population.pop();
    }
    cout << endl;
}

int main()
{

    vector<int> cost;
    vector<int> value;
    input(cost, value);

    solve(cost, value);

    return 0;
}