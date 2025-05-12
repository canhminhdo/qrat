//
// Created by Canh Minh Do on 2025/05/10.
//

#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <fstream>
#include <string>

class FileWriter {

public:
    FileWriter(const std::string fileName);
    ~FileWriter();

    // disable copy constructor and assignment operator
    FileWriter(const FileWriter &) = delete;
    FileWriter &operator=(const FileWriter &) = delete;

    void writeLine(const std::string &line);

    void writeLine(char *line);

    void writeEmptyLine();

    void write(const std::string &text);

    void write(char *text);

    std::string getFileName() const;

private:
    std::ofstream outFile;
    std::string fileName;
};

#endif//FILEWRITER_H
