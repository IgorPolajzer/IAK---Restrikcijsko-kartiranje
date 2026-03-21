#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<size_t> findRestrictionIndexes(const string& restrictionStrings, const string& fileString) {
    vector<string> restrictions;
    vector<vector<size_t>> indexes;
    vector<size_t> indexUnion;

    // Split CSV into strings,
    stringstream ss(restrictionStrings);

    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        restrictions.push_back(substr);
    }

    // Find indexes for restriction strings.
    for (auto &restriction : restrictions) {
        vector<size_t> restrictionIndexes;
        size_t res = 0;
        while ((res = fileString.find(restriction, res)) != string::npos) {
            restrictionIndexes.push_back(res);
            ++res;
        }
        indexes.push_back(restrictionIndexes);
    }

    if (indexes.empty()) {
        throw runtime_error("No restrictions found");
    }

    for (const auto& index : indexes) {
        indexUnion.insert(indexUnion.end(), index.begin(), index.end());
    }

    // Add beginning and end of the DNA.
    indexUnion.push_back(0);
    indexUnion.push_back(fileString.size() - 1);

    // Sort indexes.
    ranges::sort(indexUnion);

    return indexUnion;
}

int readFile(const string& fileName, string &stringFile) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cerr << "Failed to open file: " << fileName << endl;
        return 1;
    }

    ostringstream buffer;
    buffer << file.rdbuf();
    stringFile = buffer.str();

    file.close();
    return 0;
}

vector<size_t> getDistances(const string& fileName, const string& restrictionStrings) {

    vector<size_t> distances;
    string fileString;

    if (readFile(fileName, fileString) != 0) {
        throw runtime_error("Failed to open file: " + fileName);
    }

    const vector<size_t> restrictionIndexes = findRestrictionIndexes(restrictionStrings, fileString);

    for (size_t i = 0; i < restrictionIndexes.size(); i++) {
        for (size_t j = i + 1; j < restrictionIndexes.size(); j++) {
            distances.push_back(restrictionIndexes[j] - restrictionIndexes[i]);
        }
    }

    // Sort the distances.
    ranges::sort(distances);

    return distances;
}

void test() {
    const string folder = "examples/";

    const vector<pair<string, string>> tests = {
        { "DNK1.txt", "GTGTG" },
        { "DNK1.txt", "TTCC,CTCTCT" },
        { "DNK1.txt", "AAAA,CCCC,TTTT,GGGG" },
        { "DNK2.txt", "ACTACT,GGAGGA,GAGGCC,CTCTCT" },
        { "DNK3.txt", "TTTTTTT,GTGTCGT,ACACACA" },
    };

    for (const auto& [file, restrictions] : tests) {
        cout << file << " [" << restrictions << "]:" << endl;
        try {
            vector<size_t> L = getDistances(folder + file, restrictions);
            for (const auto& d : L) cout << d << ",";
            cout << "\n" << endl;
        } catch (const exception& e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
}

int main(const int argc, char **argv) {
    // Test mode.
    if (argc >= 2 && string(argv[argc - 1]) == "-t") {
        test();
        return 0;
    }

    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <file> <restriction-strings> [-t]" << endl;
        return 1;
    }

    const string fileName = argv[1];
    const string restrictionStrings = argv[2];

    vector<size_t> L = getDistances(fileName, restrictionStrings);

    for (const auto& index : L) {
        cout << index << ",";
    }

    return 0;
}
