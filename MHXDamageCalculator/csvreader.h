#ifndef CSVREADER_H
#define CSVREADER_H

#include <fstream>
#include <string>
#include <vector>

class CSVReader{
private:
    std::fstream fs;
public:
    CSVReader();
    CSVReader(char *filename);
    CSVReader(std::string filename);
    ~CSVReader();
    void open(char *filename);
    void open(std::string filename);
    void close();
    bool eof();
    std::vector<std::string> read();
};

#endif // CSVREADER_H
