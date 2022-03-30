#include "analysis.h"
#include <dirent.h>
#include <sstream>
#include <string.h>


string directory = "";
static int timeBlock = 10000000; // TODO: Allow user to set time block
static long startTime = 0;
static long endTime = 0;


vector <string> splitCSVLine(string str) {
    vector <string> internal;
    stringstream ss(str);
    string token;

    while (getline(ss, token, ',')) {
        internal.push_back(token);
    }

    return internal;
}

void plotData(int startTime, int endTime, string directory, string xCol, string yCol) {
    array<vector<npy_double>, 2> csvData;

    DIR *dir = opendir(directory.c_str());

    if (dir == NULL) {
        cout << "Could not open directory " << directory << endl;
    }

    struct dirent *ent = NULL;
    vector <string> files;


    while ((ent = readdir(dir)) != NULL) {
        string filename = ent->d_name;
        if (filename.find(".csv") != string::npos) {
            files.push_back(filename);
            array<vector<npy_double>, 2> newCSVData = parseCSV(filename.c_str(), xCol, yCol);
            csvData[0].insert(csvData[0].end(), newCSVData[0].begin(), newCSVData[0].end());
            csvData[1].insert(csvData[1].end(), newCSVData[1].begin(), newCSVData[1].end());
        }
    }

    plt::plot(csvData[0], csvData[1]);


    plt::xlabel(xCol);
    plt::ylabel(yCol);

    plt::save("plot.pdf");

    closedir(dir);
}


array<vector<npy_double>, 2> parseCSV(string filename, string xCol, string yCol) {
    ifstream file(directory + filename);
    array<vector<npy_double>, 2> data;

    // Variables for time block purposes
    npy_double sumX = 0;
    npy_double sumY = 0;
    int timeColNum = 0;
    int blockStartTime = 0;
    int timeDiff = 0;
    int processedCount = 0;

    // Columns being read
    int xColNum = 0;
    int yColNum = 0;

    string line = "";


    file.open("test_data/" + filename);
    if (file.is_open()) {

        // Get the headers from the first line
        getline(file, line);
        vector <string> headers = splitCSVLine(line);

        // Set the necessary column numbers
        for (int i = 0; i < headers.size(); i++) {
            if (headers[i] == "MICROSEC") {
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
            getline(file, line);
            vector <string> values = splitCSVLine(line);

            if (values[timeColNum] == "" || values[xColNum] == "" || values[yColNum] == "") {
                continue;
            }

            long currentTime = stol(values[timeColNum]);

            if (currentTime <= startTime) {
                blockStartTime = currentTime;
                continue;
            } else if (currentTime >= endTime) {
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

    plotData(startTime, endTime, directory, xCol, yCol);

    return 0;
}