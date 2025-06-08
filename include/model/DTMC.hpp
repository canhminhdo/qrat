//
// Created by Canh Minh Do on 2025/05/10.
//

#ifndef DTMC_H
#define DTMC_H

#include "utility/FileWriter.hpp"
#include <core/StateSpaceGraph.hpp>

class DTMC {

public:
    DTMC(SyntaxProg *currentProg, StateSpaceGraph *graphSearch);

    ~DTMC();

    void addHeader();

    void buildModel();

    void addFooter();

    void addLabels();

    void addRewrads();

    FileWriter *getFileModel();

    void printDtmcTiming(const Timer &timer);

    void cleanup();

private:
    FileWriter *fileWriter;
    StateSpaceGraph *graphSearch;
    SyntaxProg *currentProg;
};

#endif//DTMC_H
