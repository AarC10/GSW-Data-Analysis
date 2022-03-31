#include "analysis.h"
#include <sstream>
#include <fstream>
#include <experimental/filesystem>


using namespace std;
namespace plt = matplotlibcpp;
namespace fs = std::experimental::filesystem;


static int timeBlock = 10000000; // TODO: Allow user to set time block
static long startTime = 0;
static long endTime = 0;
static bool finishedReading = false;

vector <string> splitCSVLine(string str) {
    vector <string> result;

    stringstream ss(str);
    string token;

    while (getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}

array<vector<npy_double>, 2> parseCSV(string filename, string xCol, string yCol) {
    if (!fs::exists(filename)) {
        cout << "File does not exist" << endl;
        return {};
    }

    ifstream file(filename);
    array<vector<npy_double>, 2> data;

    // Variables for time block purposes
    npy_double sumX = 0;
    npy_double sumY = 0;
    int timeColNum = 0;
    int blockStartTime = 0;
    int processedCount = 0;

    // Columns being read
    int xColNum = 0;
    int yColNum = 0;

    string line = "";

    if (file.is_open()) {
        // Get the headers from the first line
        getline(file, line);
        vector <string> headers = splitCSVLine(line);

        // Set the necessary column numbers
        for (int i = 0; i < headers.size(); i++) {
            if (headers[i] == "MILLISEC") {
                timeColNum = i;
            }

            if (headers[i] == xCol) {
                xColNum = i;
            }
            if (headers[i] == yCol) {
                yColNum = i;
            }
        }

        // Get the data
        while (getline(file, line)) {
            vector <string> values = splitCSVLine(line);

            if (values[timeColNum] == "" || values[xColNum] == "" || values[yColNum] == "") {
                continue;
            }

            long currentTime = stol(values[timeColNum]);

            if (currentTime < startTime) {
                continue;
            } else if (currentTime > endTime) {
                finishedReading = true;
                break;
            }

            sumX += stof(values[xColNum]);
            sumY += stof(values[yColNum]);
            processedCount++;

            if (currentTime - blockStartTime >= timeBlock) {
                npy_double x = sumX / processedCount;
                npy_double y = sumY / processedCount;

                data[0].push_back(x);
                data[1].push_back(y);

                sumX = 0;
                sumY = 0;
                blockStartTime = currentTime;
            }


        }

    } else {
        cout << "Could not open " << filename << endl;
    }

    file.close();

    return data;
}

void plotData(string directory, string xCol, string yCol) {
    array<vector<npy_double>, 2> csvData;

    plt::xlabel(xCol);
    plt::ylabel(yCol);


    // Read files in directory
    for (const auto &entry: fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".csv") {
            csvData = parseCSV(entry.path().string(), xCol, yCol);

            plt::plot(csvData[0], csvData[1]);
        }

        if (finishedReading) {
            break;
        }
    }

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