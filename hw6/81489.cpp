#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <math.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class Example
{
private:
    unordered_map<string, double> attributes;
    string exampleClass;

public:
    Example(const string &exampleClass = "", int n = 0) : attributes(n), exampleClass(exampleClass) {}
    Example(const Example &other) : attributes(other.attributes), exampleClass(other.exampleClass) {}
    Example& operator = (const Example &other) {
        if (this != &other)
        {
            this->exampleClass = other.exampleClass;
            this->attributes = other.attributes;
        }

        return *this;
    }

    void setAttributeValue(const string &key, const double value) { attributes[key] = value; }
    const double getAttributeValue(const string &key) const { return attributes.at(key); }

    void setClass(const string &exampleClass) { this->exampleClass = exampleClass; }
    const string getClass() const { return exampleClass; }
};

Example createExample(string line, const vector<string> &attributes)
{
    Example example("", attributes.size());
    size_t comaPlacement;
    for (size_t i = 0; i < attributes.size(); ++i)
    {
        comaPlacement = line.find_first_of(',');
        example.setAttributeValue(attributes[i], stof(line.substr(0, comaPlacement)));
        line = line.substr(comaPlacement + 1);
    }
    example.setClass(line);
    return example;
}

vector<Example> input(const vector<string> &attributes, const char *fileName)
{
    ifstream is(fileName, ios::in);
    vector<Example> examples;

    string line;
    while (getline(is, line))
    {
        examples.push_back(createExample(line, attributes));
    }
    return examples;
}

void printExample(const Example &example, const vector<string> &attributes)
{
    for (size_t j = 0; j < 4; ++j)
    {
        cout << setiosflags(ios::fixed) << setprecision(5) << example.getAttributeValue(attributes[j]) << "\t\t";
    }
    cout << example.getClass() << endl;
}

void print(const vector<Example> &examples, const vector<string> &attributes)
{
    for (size_t i = 0; i < examples.size(); ++i)
    {
        printExample(examples[i], attributes);
    }
}

void normalize(vector<Example> &examples, const vector<string> &attributes, unordered_map<string, pair<double, double>> normalizationData)
{
    for (size_t i = 0; i < attributes.size(); ++i)
    {
        string attribute = attributes[i];
        double minValue, maxValue;
        if (normalizationData.find(attribute) == normalizationData.end())
        {
            int minIndex = 0, maxIndex = 0;
            for (size_t j = 1; j < examples.size(); ++j)
            {
                if (examples[j].getAttributeValue(attribute) < examples[minIndex].getAttributeValue(attribute))
                {
                    minIndex = j;
                }
                if (examples[j].getAttributeValue(attribute) > examples[maxIndex].getAttributeValue(attribute))
                {
                    maxIndex = j;
                }
            }

            minValue = examples[minIndex].getAttributeValue(attribute);
            maxValue = examples[maxIndex].getAttributeValue(attribute);
            normalizationData[attribute] = pair<double, double>(minValue, maxValue);
        }
        else
        {
            auto data = normalizationData[attribute];
            minValue = data.first;
            maxValue = data.second;
        }

        double divisor = maxValue - minValue;
        for (size_t j = 0; j < examples.size(); ++j)
        {
            double val = examples[j].getAttributeValue(attribute);
            val -= minValue;
            examples[j].setAttributeValue(attribute, val / divisor);
        }
    }
}

double dist(const Example &left, const Example &right, const vector<string> &attributes)
{
    double distance = 0;
    for (size_t i = 0; i < attributes.size(); ++i)
    {
        distance += pow((left.getAttributeValue(attributes[i]) - right.getAttributeValue(attributes[i])), 2);
    }

    return sqrt(distance);
}

size_t findNearestCenter(const Example &example, const vector<Example> &centerPoints, const vector<string> &attributes, int k)
{
    size_t nearestCenterIndex = 0;
    for (size_t i = 1; i < k; ++i)
    {
        if (dist(example, centerPoints[i], attributes) < dist(example, centerPoints[nearestCenterIndex], attributes))
        {
            nearestCenterIndex = i;
        }
    }

    return nearestCenterIndex;
}

void recalculateCenterPoints(vector<Example> &centerPoints, unordered_map<size_t, vector<Example>> &clusters, const vector<string> &attributes, int k)
{
    for (size_t i = 0; i < k; ++i)
    {
        vector<Example> cluster = clusters.at(i);
        unordered_map<string, double> midAttributes;
        for (size_t j = 0; j < attributes.size(); ++j)
        {
            midAttributes[attributes[j]] = 0.0;
        }
        size_t clusterSize = cluster.size();

        for (size_t j = 0; j < clusterSize; ++j)
        {
            for (size_t t = 0; t < attributes.size(); ++t)
            {
                midAttributes[attributes[t]] += cluster[j].getAttributeValue(attributes[t]);
            }
        }

        Example newCenter("CENTER", attributes.size());

        for (size_t t = 0; t < attributes.size(); ++t)
        {
            newCenter.setAttributeValue(attributes[t], midAttributes[attributes[t]] / (double)clusterSize);
        }

        centerPoints[i] = newCenter;
        clusters[i].clear();
    }
}

void kMeans(const vector<Example> &examples, const vector<string> &attributes, int k)
{
    vector<Example> centerPoints(k);
    unordered_map<size_t, vector<Example>> clusters;
    srand(time(NULL));

    for (size_t i = 0; i < k; ++i)
    {
        size_t index = rand() % examples.size();
        centerPoints[i] = examples[index];
        clusters[i] = vector<Example>({ examples[index] });
    }

    for (size_t i = 0; i < 100; ++i)
    {
        cout << "ITERATION " << i << endl;
        for (int j = 0; j < k; ++j)
        {
            printExample(centerPoints[j], attributes);
        }

        recalculateCenterPoints(centerPoints, clusters, attributes, k);
        for (size_t j = 0; j < examples.size(); ++j)
        {
            size_t nearestClusterIndex = findNearestCenter(examples[j], centerPoints, attributes, k);
            clusters[nearestClusterIndex].push_back(examples[j]);
        }
    }

    for (size_t i = 0; i < k; ++i) {
        printExample(centerPoints[i], attributes);

        cout << "\nCLUSTER\n";
        for (size_t j = 0; j < clusters[i].size(); ++j)
        {
            printExample(clusters[i][j], attributes);
        }
    }
}

int main()
{
    vector<string> attributes = {"sepallength", "sepalwitdth", "petallength", "petalwidth"};
    vector<Example> examples = input(attributes, "iris.arff");
    unordered_map<string, pair<double, double>> normalizationData;
    normalize(examples, attributes, normalizationData);

    kMeans(examples, attributes, 7);

    return 0;
}