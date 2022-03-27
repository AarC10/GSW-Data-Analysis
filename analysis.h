//
// Created by aarc on 3/25/22.
//

#ifndef DATA_ANALYSIS_ANALYSIS_H
#define DATA_ANALYSIS_ANALYSIS_H

#endif //DATA_ANALYSIS_ANALYSIS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

void plotData(std::vector <std::string> fileNames);

array<vector<float>, 2> parseCSV(std::string filename, string xCol, string yCol);

vector<string> splitCSVLine(string str);