#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>

using namespace std;

int getDigit(int number, int place) {
    if ((log10(number) + 1) < place) return 0;

    if (place == 1) {
        return number % 10;
    } else {
        return static_cast<int>(number / pow(10, place - 1)) % 10;
    }
}

vector<int> radixSort(vector<int> input, int digit) {
    vector<int> sortedList;
    sortedList.reserve(input.size());
    vector <vector<int>> buckets;
    for (int i = 0; i < 10; ++i) {
        if (sortedList.size() == input.size())break;
        buckets.emplace_back();
        for (int number : input) {
            if (getDigit(number, digit) == i) {
                if (digit == 1) {
                    sortedList.push_back(number);
                    continue;
                }
                buckets[i].push_back(number);
            }
        }
        if (!buckets[i].empty()) {
            for (int number: radixSort(buckets[i], digit - 1)) {
                sortedList.push_back(number);
            }
        }
    }

    return sortedList;
}

vector<int> getUsortedList(int size, unsigned int minValue, unsigned int maxValue) {
    // random seed
    random_device rd;
    default_random_engine generator(rd());
    uniform_int_distribution<unsigned int> distribution(minValue, maxValue);

    vector<int> data;

    for (int i = 0; i < size; i++) {
        data.push_back(distribution(generator));
    }
    return data;
}

int sort(unsigned int amount, unsigned int max = 100, unsigned int min = 1) {
    using namespace chrono;
    auto unsortedList = getUsortedList(amount, min, max);
    long timeStart = duration_cast<milliseconds>(
            time_point_cast<milliseconds>(system_clock::now()).time_since_epoch()).count();

    auto sortedList = radixSort(unsortedList, static_cast<unsigned int>(log10(max) + 1));

    using namespace chrono;
    long timeEnd = duration_cast<milliseconds>(
            time_point_cast<milliseconds>(system_clock::now()).time_since_epoch()).count();
    // debug print sorted list
//    for (int j = 0; j < amount; ++j) {
//        cout << sortedList[j] << ", ";
//    }

    return static_cast<int>(timeEnd - timeStart);
}

int main() {
    unsigned int max = 100;
    unsigned int min = 1;
    unsigned int amount = 1;
    int loops = 100;
    int totalTime = 0;
    for (int i = 1; i <= loops; ++i) {
        int result = sort(amount * (i * 10000));
        totalTime += result;
        cout << "Items: " << amount * (i * 10000) << "\t\tElapsed time: " << result << "ms" << endl;
        ofstream resultFile;
        resultFile.open("result.csv");
        resultFile << amount * (i * 10000) << ";" << result << "\n";
        resultFile.close();
    }
    cout << "Total elapsed time: " << totalTime << "ms" << endl;
}