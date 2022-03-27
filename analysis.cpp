#include "analysis.h"
#include <dirent.h>
#include <sstream>
#include <string.h>


string directory = "";
static int timeBlock = 10000000; // TODO: Allow user to set time block
static long startTime = 0;
static long endTime = 0;


vector<string> splitCSVLine(string str) {
    vector <string> internal;
    stringstream ss(str);
    string token;

    while (getline(ss, token, ',')) {
        internal.push_back(token);
    }

    return internal;
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

    startTime = stoi(argv[1]);
    endTime = stoi(argv[2]);
    string xCol = argv[4];
    string yCol = argv[5];

    DIR *dir = opendir(argv[3]);
    directory = argv[3];
    if (dir == NULL) {
        cout << "Could not open directory " << argv[3] << endl;
        return 1;
    }

    struct dirent *ent = NULL;
    vector <string> files;

    while ((ent = readdir(dir)) != NULL) {
        string filename = ent->d_name;
        if (filename.find(".csv") != string::npos) {
            files.push_back(filename);
            array<vector<npy_double>, 2> csvData = parseCSV(filename.c_str(), xCol, yCol);

            plt::plot(csvData[0], csvData[1]);

        }
    }

    plt::xlabel(xCol);
    plt::ylabel(yCol);

    plt::show();


    closedir(dir);

    return 0;
}