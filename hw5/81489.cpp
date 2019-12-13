#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <unordered_map>
#include <queue>
#include <math.h>

using namespace std;

class Example
{
private:
    unordered_map<string, double> attributes;
    string exampleClass;

public:
    Example(const string &exampleClass = "", int n = 0) : attributes(n), exampleClass(exampleClass) {}
    Example(const Example &other) : attributes(other.attributes), exampleClass(other.exampleClass) {}

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

string defineClass(const vector<Example> &nearestNeighbors)
{
    unordered_map<string, pair<int, int>> classCountAndFirstOccurrence;
    string definedClass = "";
    for (size_t i = 0; i < nearestNeighbors.size(); ++i)
    {
        string exampleClass = nearestNeighbors[i].getClass();
        if (classCountAndFirstOccurrence.find(exampleClass) == classCountAndFirstOccurrence.end())
        {
            classCountAndFirstOccurrence[exampleClass] = pair<int, int>(1, i);
        }
        else
        {
            auto data = classCountAndFirstOccurrence[exampleClass];
            data.first++;
            classCountAndFirstOccurrence[exampleClass] = data;
        }

        if (definedClass == "")
        {
            definedClass = exampleClass;
        }
        else if (definedClass != exampleClass)
        {
            const auto prevData = classCountAndFirstOccurrence[definedClass];
            const auto exampleData = classCountAndFirstOccurrence[exampleClass];
            
            if (prevData.first > prevData.first || (prevData.first == prevData.first && prevData.second > prevData.second))
            {
               definedClass = exampleClass; 
            }
        }
    }

    return definedClass;
}

void kNN(const Example &testExample, const vector<Example> &examples, const vector<string> &attributes, int k)
{
    auto comparator = [testExample, attributes](const auto &left, const auto &right) {
        return dist(testExample, left, attributes) > dist(testExample, right, attributes);
    };
    priority_queue<Example, vector<Example>, decltype(comparator)> nearest(comparator);

    for (size_t i = 0; i < examples.size(); ++i)
    {
        nearest.push(examples[i]);
    }

    vector<Example> nearestNeighbors(k);

    for (size_t i = 0; i < k; ++i)
    {
        printExample(nearest.top(), attributes);
        nearestNeighbors[i] = nearest.top();
        cout << dist(testExample, nearest.top(), attributes) << endl;
        nearest.pop();
    }

    string testExampleClass = defineClass(nearestNeighbors);
    cout << "CLASSIFIED AS \"" << testExampleClass << "\"\n\n";
}

int main()
{
    vector<string> attributes = {"sepallength", "sepalwitdth", "petallength", "petalwidth"};
    vector<Example> examples = input(attributes, "iris.arff");
    unordered_map<string, pair<double, double>> normalizationData;
    normalize(examples, attributes, normalizationData);

    vector<Example> testData = input(attributes, "samples.txt");
    normalize(testData, attributes, normalizationData);

    for (size_t i = 0; i < testData.size(); ++i)
    {
        kNN(testData[i], examples, attributes, 6);
    }

    return 0;
}