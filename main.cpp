#include <fstream>
#include <iostream>
#include <vector>

#include "classes/RestrictionCarting.h"

using namespace std;

int main(const int argc, char** argv) {
    const std::string modeFlag = argv[1];
    const std::string algorithmFlag = argv[2];

    if (modeFlag == "-t") {
        RestrictionCarting::test(algorithmFlag);
        return 0;
    }

    if (argc < 4) {
        cerr << "Usage: " << endl;
        cerr << "1.: " << argv[0] << " [-t] [-bf or -pd]" << endl;
        cerr << "2.: " << argv[0] << " [-bf or -pd] <file-path> <CSV-DNK-sections>" << endl;
        return 1;
    }

    const std::string filePath = argv[2];
    const std::string restrictions = argv[3];
    RestrictionCarting::solveProblem(filePath, restrictions, modeFlag);

    return 0;
}
