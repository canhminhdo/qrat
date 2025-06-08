//
// Created by Canh Minh Do on 2025/06/09.
//

#ifndef RUNNERFACTORY_HPP
#define RUNNERFACTORY_HPP

#include "model/Runner.hpp"
#include "model/PrismRunner.hpp"
#include "model/StormRunner.hpp"

class RunnerFactory {
public:
    enum Backend {
        PRISM,
        STORM
    };

    static Runner *createRunner(std::vector<char *> *args) {
        Backend backend = Backend::PRISM;
        bool saveModel = false;
        for (int i = 0; i < args->size(); i++) {
            if (strcmp(args->at(i), "--backend=Storm") == 0) {
                backend = Backend::STORM;
            } else if (strcmp(args->at(i), "--backend=PRISM") == 0) {
                backend = Backend::PRISM;
            } else if (strcmp(args->at(i), "--save-model=true") == 0) {
                saveModel = true;
            } else if (strcmp(args->at(i), "--save-model=false") == 0) {
                saveModel = false;
            }
            delete args->at(i);
        }
        Runner *runner = nullptr;
        if (backend == Backend::PRISM) {
            runner = new PrismRunner();
        } else if (backend == Backend::STORM) {
            runner = new StormRunner();
        }
        runner->setSaveModel(saveModel);
        return runner;
    }
};

#endif //RUNNERFACTORY_HPP
