/**
 * @author Jwalant Patel, Ross Cameron, Lance Cheong Youne
 * @date 2024-03-12
 * @brief csvdata class that loads data from a specified file into a vetor data,
 *        that stores a list of rows(vectors), that contain cells(coumns) of strings
*/

#include "csvdata.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

/** @brief constuctor CsvData, creates a new instance of the class for a specified file and loads data from a .csv file into the vector columns of data 
 * @param filename is the file used to load data from
 */
CsvData::CsvData(const string& filename){
    loadFromFile(filename);
}

/** @brief print function used to debug code
 */
void CsvData::printData() const{
    for(const auto& row : data){
        for(const auto& cell : row){
            cout << cell << " ";
    }
    cout << endl;
    }
}

/** @brief Loads data into vector data when reading from a specified csv filen
 * @param filename is the file used to load data from
 */
void CsvData::loadFromFile(const string& filename){
    ifstream file(filename);
    string line;
    
    // prints error if file cannot open
    if(!file.is_open()){
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    bool firstIter = true;
    // loops through each line in the file
    while(getline(file, line)){
        if(firstIter){
            firstIter = false;
            continue;
        }
        vector<string> row;
        stringstream lineStream(line);
        string cell;

        // reads line for a certain columns 1 & 2 which correspond to the name and the url link
        int colIndex = 0;
        while(getline(lineStream, cell, ',')){ // loops through each cell (column)
            // if index of 1 or 2 adds to vector row (in a selective manor this way)
            if(colIndex == 1 || colIndex == 2){
                if(colIndex == 1){
                    names.push_back(cell);
                }
                if(colIndex == 2){
                    urls.push_back(cell);
                }
                row.push_back(cell);
            }
            colIndex++; //otherwise increments column
        }

        // vector row is then added to data
        if(!row.empty()){
            data.push_back(row);
        }
    }
    // closes file once finished reading
    file.close();
}

/** @brief Clears the data in the csv
 */
void CsvData::clearData(){
    data.clear();
}

/** @brief Getter method that returns the data (names and url together) in vector form
 * @return data from the file entered
 */
vector<vector<string>> CsvData::getData(){
    return this->data;
}

/** @brief Getter method that returns the names in vector form
 * @return names from the file entered
 */
vector<string> CsvData::getNames(){
    return this->names;
}

/** @brief Getter method that returns the URLs in vector form
 * @return URLs from the file entered
 */
vector<string> CsvData::getURLs(){
    return this->urls;
}