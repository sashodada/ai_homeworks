/**
 * Genetic algorithm
 * 3rd homework for AI course @FMI, Sofia University
 * Aleksandar Shumakov, FN 81489
 **/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <time.h>
#include <algorithm>
#include <queue>

using namespace std;

void parseLine(string &line, vector<int> &cost, vector<int> &value)
{
    int spaceIndex = line.find(' ');
    cost.push_back(atoi(line.substr(0, spaceIndex).c_str()));
    value.push_back(atoi(line.substr(spaceIndex + 1).c_str()));
}

void input(vector<int> &cost, vector<int> &value)
{
    ifstream input("testData_long.txt", ifstream::in);
    string line;

    getline(input, line);
    while (getline(input, line))
    {
        parseLine(line, cost, value);
        cout << cost[cost.size() - 1] << endl; 
    }
}

vector<bool> initChromosome(int chromosomeLength)
{
    vector<bool> chromosome(chromosomeLength);
    for (int i = 0; i < chromosomeLength; ++i)
    {
        chromosome[i] = rand() % 10 == 0;
    }
    return chromosome;
}

int totalValue(vector<bool> &chromosome, const vector<int> &cost, const vector<int> &value, int chromosomeSize)
{
    int totalCost = 0;
    int totalValue = 0;
    for (int i = 0; i < chromosomeSize; ++i)
    {
        totalCost += chromosome[i] ? cost[i] : 0;
        totalValue += chromosome[i] ? value[i] : 0;
    }

    return totalCost > 5000 ? 5000 - totalCost : totalValue;
}

void generateChildren(vector<bool> &parent1, vector<bool> &parent2, vector<vector<bool>> &population, int startIndex, int chromosomeLength)
{
    for (int i = 0; i < 2; ++i)
    {
        vector<bool> chromosome(chromosomeLength);
        for (int j = 0; j < chromosomeLength; ++j)
        {
            chromosome[j] = rand() % 2 ? parent1[j] : parent2[j];
        }
        population[startIndex + i] = chromosome;
    }
}

void mutatePopulation(vector<vector<bool>> &population, int populationSize, int chromosomeLength)
{
    for (int i = 0; i < populationSize; ++i)
    {
        // Chromosome mutation change is 10% here
        if (rand() % 100 < 10)
        {
            for (int j = 0; j < 3; ++j)
            {
                int bitIndex = rand() % chromosomeLength;
                population[i][bitIndex] = !population[i][bitIndex];
            }
        }
    }
}

void solve(vector<int> &cost, vector<int> &value)
{
    srand(time(NULL));

    const int POPULATION_SIZE = 100;
    int CHROMOSOME_LENGTH = cost.size();

    auto chromosomeComparator = [cost, value, CHROMOSOME_LENGTH](vector<bool> &left, vector<bool> &right) {
        return totalValue(left, cost, value, CHROMOSOME_LENGTH) > totalValue(right, cost, value, CHROMOSOME_LENGTH);
    };

    vector<vector<bool>> population(POPULATION_SIZE);

    for (int i = 0; i < POPULATION_SIZE; ++i)
    {
        population[i] = initChromosome(CHROMOSOME_LENGTH);
    }

    sort(population.begin(), population.end(), chromosomeComparator);

    for (int i = 0; i < 5000; i++)
    {
        cout << "ITERATION: " << i + 1 << endl;
        int killCount = (POPULATION_SIZE / 10) * 2;
        for (int i = 0; i < killCount; i += 2)
        {
            auto parent1 = population[i];
            auto parent2 = population[i + 1];

            generateChildren(parent1, parent2, population, POPULATION_SIZE - killCount + i, CHROMOSOME_LENGTH);
        }
        mutatePopulation(population, POPULATION_SIZE, CHROMOSOME_LENGTH);

        sort(population.begin(), population.end(), chromosomeComparator);
        cout << "\n\n";
        cout << "FITTEST CHROMOSOME: " << totalValue(population[0], cost, value, CHROMOSOME_LENGTH) << "\n";
    }
}

int main()
{
    vector<int> cost;
    vector<int> value;
    input(cost, value);

    solve(cost, value);

    return 0;
}