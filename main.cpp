#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <fstream>
#include <sys/timeb.h>

using namespace std;

int getMilliCount() {
    timeb tb{};
    ftime(&tb);
    int nCount = static_cast<int>(tb.millitm + (tb.time & 0xfffff) * 1000);
    return nCount;
}

int getMilliSpan(int nTimeStart) {
    int nSpan = getMilliCount() - nTimeStart;
    if (nSpan < 0)
        nSpan += 0x100000 * 1000;
    return nSpan;
}

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
    auto unsortedList = getUsortedList(amount, min, max);
    int timeStart = getMilliCount();

    auto sortedList = radixSort(unsortedList, static_cast<unsigned int>(log10(max) + 1));


    // debug print sorted list
//    for (int j = 0; j < amount; ++j) {
//        cout << sortedList[j] << ", ";
//    }

    return getMilliSpan(timeStart);
}

int main() {
    unsigned int max = 100;
    unsigned int min = 1;
    unsigned int amount = 100;
    int loops = 500;
    int stepSize = 10;
    int totalTime = 0;
    ofstream resultFile;
    resultFile.open("result.csv");
    resultFile << "Items, Time(ms)\n";
    for (int i = 1; i <= loops; ++i) {
        int result = sort(amount * (i * stepSize), max);
        totalTime += result;
        cout << "Items: " << amount * (i * stepSize) << "\t\t\tElapsed time: " << result << "ms" << endl;
        resultFile << amount * (i * stepSize) << "," << result << "\n";
    }
    resultFile.close();
    cout << "Total sorting time: " << totalTime << "ms" << endl;
}