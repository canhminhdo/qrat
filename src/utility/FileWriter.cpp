//
// Created by Canh Minh Do on 2025/05/10.
//

#include "utility/FileWriter.hpp"

FileWriter::FileWriter(const std::string fileName) {
    this->fileName = fileName;
    outFile.open(fileName);
}

FileWriter::~FileWriter() {
    if (outFile.is_open()) {
        outFile.close();
    }
}

void FileWriter::writeLine(const std::string &line) {
    outFile << line << std::endl;
}

void FileWriter::writeLine(char *line) {
    outFile << line << std::endl;
}

void FileWriter::writeEmptyLine() {
    outFile << std::endl;
}

void FileWriter::write(const std::string &text) {
    outFile << text;
}

void FileWriter::write(char *text) {
    outFile << text;
}

std::string FileWriter::getFileName() const {
    return fileName;
}