//
// Created by Canh Minh Do on 2025/05/10.
//

#ifndef DTMC_H
#define DTMC_H

#include "utility/FileWriter.hpp"

#include <core/StateTransitionGraph2.hpp>

class DTMC {

public:
    DTMC(SyntaxProg *currentProg, StateTransitionGraph2 *graphSearch);

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
    StateTransitionGraph2 *graphSearch;
    SyntaxProg *currentProg;
};

#endif //DTMC_H
