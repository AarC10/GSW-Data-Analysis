/**
 * Parses csv files in a given directory and graphs data based
 * on user specifications.
 *
 * @author Aaron Chan
 */

#ifndef DATA_ANALYSIS_ANALYSIS_H
#define DATA_ANALYSIS_ANALYSIS_H

#endif //DATA_ANALYSIS_ANALYSIS_H

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "matplotlibcpp.h"

using namespace std;
namespace plt = matplotlibcpp;

/**
 * Parses a given csv file and returns an array of two vectors
 * storing x and y values
 *
 * @param filename
 * @param xCol
 * @param yCol
 * @return vector<vector<double>>
 */
array<vector<npy_double>, 2> parseCSV(std::string filename, string xCol, string yCol);

/**
 * Splits a line of comma separated values into a vector of strings
 * @param str
 * @return
 */
vector<string> splitCSVLine(string str);


/**
 * Takes parsed data and graphs it
 * @param startTime
 * @param endTime
 * @param directory
 * @param xCol
 * @param yCol
 */
void plotData(int startTime, int endTime, string directory, string xCol, string yCol)