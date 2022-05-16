#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <algorithm>

#ifndef Arguments_H
#define Arguments_H

char *getCmdOption(char **begin, char **end, const std::string &option) {
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    cout << option;
    return 0;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option) {
    return std::find(begin, end, option) != end;
}

void printHelpMenu() {
    cout << "\033[1;31m3D Cellular Automata Visualization Tool\033[0m\n";
    cout << "  ./main [animation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rotation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rule number (\033[4;35mint\033[0m)] ["
            "side length (\033[4;37mint\033[0m)] [animstep (\033[4;34mint\033[0m)]"
         << endl;
    cout << "  ./main [animation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rotation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rule number (\033[4;35mint\033[0m)] ["
            "side length (\033[4;37mint\033[0m)]"
         << endl;
    cout << "  ./main [animation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rotation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rule number (\033[4;35mint\033[0m)]"
         << endl;
    cout << "  ./main [animation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)] [rotation enabled (\033[4;32mtrue\033[0m/\033[4;31mfalse\033[0m)]" << endl;
    cout << "  ./main [rule number (\033[4;35mint\033[0m)]" << endl;
    cout << "  ./main" << endl;
    cout << "\033[1;31mHow to Use it\033[0m\n";
    cout << "  ./main true false 10" << endl;
    cout << "  ./main 6" << endl;
    cout << "  ./main true true 10 20 5" << endl;
    exit(1);
};

#endif // Arguments_H
