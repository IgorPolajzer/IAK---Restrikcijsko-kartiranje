#include <fstream>
#include <iostream>
#include <vector>

#include "RestrictionCarting.h"

using namespace std;

int main(const int argc, char** argv) {
    // Test mode.
    if (argc >= 2 && string(argv[argc - 1]) == "-t") {
        RestrictionCarting::test();
        return 0;
    }

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <file> <restriction-strings> [-t]" << endl;
        return 1;
    }

    const string fileName = argv[1];
    const string restrictionStrings = argv[2];

    vector<size_t> L = RestrictionCarting::getDistances(fileName, restrictionStrings);

    for (const auto& index : L) {
        cout << index << ",";
    }

    return 0;
}
