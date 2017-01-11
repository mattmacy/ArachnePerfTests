#include <stdio.h>
#include <unistd.h>
#include <vector>

#include "Arachne.h"
#include "Cycles.h"
#include "TimeTrace.h"


using PerfUtils::Cycles;
using PerfUtils::TimeTrace;


/**
  * This experiment is meant to test whether Arachne correctly ensures that
  * starvation does not occur when there are three runnable threads on a core
  * that are perpetually yielding.
  */


void runForever(int id) {
    while (true) {
        Arachne::yield();
        if (id == 2) { 
            printf("Starvation has not occurred!\n");
            id = 3;
        }
    }
}

int realMain(int argc, char** argv) {
    // Add some work
    Arachne::createThread(0, runForever, 0);
    Arachne::createThread(0, runForever, 1);
    Arachne::createThread(0, runForever, 2);
    return 0;
}

int main(int argc, char** argv){
    // Initialize the library
    int nArgs = 2;
    const char* args[] = {"--numCores", "2"};
    Arachne::init(&nArgs, args);
    Arachne::createThread(3, realMain, argc, argv);
    // Must be the last call
    Arachne::waitForTermination();
}
