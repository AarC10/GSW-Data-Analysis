#include "include/analysis.h"
#include <sstream>
#include <fstream>
#include <experimental/filesystem>


using namespace std;
namespace plt = matplotlibcpp;
namespace fs = std::experimental::filesystem;


static int timeBlock = 1000; // TODO: Allow user to set time block
static ulong startTime = 0;
static long endTime = 0;
static bool finishedReading = false;

ulong timeCombine(long milli, long micro) {
    return (milli * 1000) + micro;
}

vector<string> splitCSVLine(const string &str) {
    vector<string> result;

    stringstream ss(str);
    string token;

    while (getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}

array<vector<npy_double>, 2> parseCSV(const string &filename, const string &xCol, const string &yCol) {
    int millisecColNum = -1;
    int microsecColNum = -1;
    int xColNum = -1;
    int yColNum = -1;

    if (!fs::exists(filename)) {
        cout << "File does not exist" << endl;
        return {};
    }

    ifstream file(filename);
    vector<npy_double> xVals;
    vector<npy_double> yVals;

    string line;

    getline(file, line);
    vector<string> headers = splitCSVLine(line);

    for (int i = 0; i < headers.size(); i++) {
        if (headers[i] == "MILLISEC") {
            millisecColNum = i;
        }

        if (headers[i] == "MICROSEC") {
            microsecColNum = i;
        }

        if (headers[i] == xCol) {
            xColNum = i;
        }

        if (headers[i] == yCol) {
            yColNum = i;
        }
    }

    if (millisecColNum == -1 || microsecColNum == -1 || xColNum == -1 || yColNum == -1) {
        cout << "Columns not found" << endl;
        return {};
    }

    if (file.is_open()) {
        double timeblockXSum = 0;
        double timeblockYSum = 0;
        int lineCount = 0;

        while (!finishedReading && getline(file, line)) {
            vector<string> lineVals = splitCSVLine(line);

            if (lineVals[millisecColNum].empty() && lineVals[microsecColNum].empty()) {
                continue;
            }

            ulong currentMillis = stol(lineVals[millisecColNum]);
            ulong currentMicros = stol(lineVals[microsecColNum]);
            double xVal = stod(lineVals[xColNum]);
            double yVal = stod(lineVals[yColNum]);


            if (currentMillis < startTime) {
                cout << "Start time is before first time stamp: " << startTime <<  " " << lineCount << endl;
                continue;
            }

            if (currentMillis > endTime) {
                cout << "End time is after last time stamp" << endl;
                finishedReading = true;
                break;
            }


            timeblockXSum += xVal;
            timeblockYSum += yVal;
            lineCount++;

            if (currentMillis - startTime > timeBlock) {
                xVals.push_back(timeblockXSum / lineCount);
                yVals.push_back(timeblockYSum / lineCount);
                cout << lineCount << " " << timeblockXSum / lineCount << " " << timeblockYSum / lineCount << endl;
                timeblockXSum = 0;
                timeblockYSum = 0;
                lineCount = 0;

                startTime = currentMillis;
            }
        }

    }

    file.close();

    return {xVals, yVals};
}

void plotData(const string &directory, const string &xCol, const string &yCol) {
    array<vector<npy_double>, 2> csvData;
    array<vector<npy_double>, 2> newData;
    int fileCount = 1;

    plt::xlabel(xCol);
    plt::ylabel(yCol);

    // While file exists
    while (fs::exists(directory + "/" + "log" + to_string(fileCount) + ".csv")) {
        newData = parseCSV(directory + "/" + "log" + to_string(fileCount) + ".csv", xCol, yCol);

        csvData[0].insert(csvData[0].end(), newData[0].begin(), newData[0].end());
        csvData[1].insert(csvData[1].end(), newData[1].begin(), newData[1].end());

        if (finishedReading) {
            break;
        }

        fileCount++;
    }

    plt::plot(csvData[0], csvData[1]);

    plt::save("plot.pdf");
}


int main(int argc, char *argv[]) {
    // TODO: Improve flexibility of passing arguments
    if (argc != 6) {
        cout << "Only received " << argc - 1 << " arguments, but expected 5." << endl;
        cout << "Usage: " << argv[0] << " <start_time> <end_time> <directory> <x_col> <y_col>" << endl;

        return 1;
    }

    startTime = stol(argv[1]);
    endTime = stol(argv[2]);
    string directory = argv[3];
    string xCol = argv[4];
    string yCol = argv[5];

    plotData(directory, xCol, yCol);

    return 0;
}