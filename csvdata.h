/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne
 * @date 2024-03-12
 * @brief header that contains the libraries, variables and methods required for csvdata.cpp
*/
#ifndef CSVDATA_H
#define CSVDATA_H
//include necessary libraries
#include <vector>
#include <string>
using namespace std;

class CsvData {
public:
//initialize public functions to be used in csvdata.cpp
    CsvData(const string& filename);
    void printData() const;
    vector<vector<string>> getData();
    vector<string> getNames();
    vector<string> getURLs();
    void clearData();

private:
    vector<vector<string>> data; //initialize vector of vectors of strings to contain data (2D)
    vector<string> urls; //initialize vector of strings to contain urls
    vector<string> names; //initialize vector of strings to contain names
    void loadFromFile(const string& filename); //initialize private function to be used in csvdata.cpp
    
};

#endif // CSVDATA_H