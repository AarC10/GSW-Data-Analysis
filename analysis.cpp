#include <vector>
#include <filesystem>
#include <dirent.h>
#include <string>
#include <stdio.h>
#include <ostream>
#include <iostream>
#include <fstream>
#include <ios>
#include "matplotlibcpp.h"
#include <vector>

using namespace std;
namespace plt = matplotlibcpp;

string directory = "";
int timeBlock = 10; // TODO: Allow user to set time block

int totalLines = 0;

vector<float[2]> parseCSV(std::string filename, string x, string y) {
    ifstream file(directory + filename);
    vector<float[2]> data;
    int sum = 0;
    int values = 0;
    string line = "";
    string delimiter = ",";

    file.open("test_data/" + filename);
    if (file.is_open()) {
        while (getline(file, line)) {
            getline(file, line);
            cout << line << endl;
            totalLines++;
        }

    } else {
        cout << "Could not open " << filename << endl;
    }

    file.close();

    return data;
}


//void plotData() {
//
//
//
//    vector<double> x;
//    vector<double> y;
//
//
//    plt::plot(x, y);
//    plt::save("./plot.png");
//}

int main(int argc, char *argv[]) {
    // TODO: Improve flexibility of passing arguments
    if (argc != 4) {
        cout << "Only received " << argc - 1 << " arguments, but expected 3." << endl;
        cout << "Usage: " << argv[0] << " <start_time> <end_time> <directory>" << endl;
        return 1;
    }

    int startTimeInSec = stoi(argv[1]);
    int endTimeInSec = stoi(argv[2]);

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
            parseCSV(filename.c_str(), "x", "y");
        }
    }


    closedir(dir);

    printf("%d\n", totalLines);

    return 0;
}