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
#include "matplotlibcpp.h"


std::vector<std::string> parseCSV(std::string fileName, matplotlibcpp::Figure &fig);

matplotlibcpp::figure plotData(std::vector<std::string> data);


void plotData();