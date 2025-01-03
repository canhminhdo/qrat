//
// Created by CanhDo on 2025/01/03.
//

#include "utility/fileReader.hpp"
#include <iostream>

void printFile(const char *fileName) {
    // Open the file in read mode
    FILE *file = fopen(fileName, "r");
    // Check if the file was opened successfully
    if (file == nullptr) {
        std::cerr << "Error opening file: " << fileName << std::endl;
        return;
    }
    // Read and print the file content character by character
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        std::cout << ch;
    }
    // Close the file
    fclose(file);
}