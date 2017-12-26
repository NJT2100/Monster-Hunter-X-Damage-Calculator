#include "csvreader.h"
#include <sstream>

using namespace std;

CSVReader::CSVReader() : fs(){}

CSVReader::CSVReader(char* filename) : fs(filename){}

CSVReader::CSVReader(string filename) :fs(filename.c_str()){}

CSVReader::~CSVReader(){
    if (fs.is_open()){
        fs.close();
    }
}

void CSVReader::open(char *filename){
    if (!fs.is_open()){
        fs.open(filename);
    }
}

void CSVReader::close(){
    if (fs.is_open()){
        fs.close();
    }
}

bool CSVReader::eof(){
    if (fs.eof()){
        return true;
    }
    return false;
}
vector<string> CSVReader::read(){
    vector<string> row;
    std::stringstream ss;
    string line, cell;
    getline(fs, line);
    ss.str(line);
    while (getline(ss, cell, ',')){
        row.push_back(cell);
    }
    return row;
}

