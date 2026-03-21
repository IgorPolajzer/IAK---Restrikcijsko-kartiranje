#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

vector<vector<size_t>> findRestrictionIndexes(const string& restrictionStrings, const string& fileString) {
    vector<string> restrictions;
    vector<vector<size_t>> indexes;

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

    return indexes;
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

    vector<size_t> indexUnion, distances;
    string fileString;

    if (readFile(fileName, fileString) != 0) {
        throw runtime_error("Failed to open file: " + fileString);
    }

    const vector<vector<size_t>> restrictionIndexes = findRestrictionIndexes(restrictionStrings, fileString);

    if (restrictionIndexes.empty()) {
        throw runtime_error("No restrictions found");
    }

    for (const auto& index : restrictionIndexes) {
        indexUnion.insert(indexUnion.end(), index.begin(), index.end());
    }

    if (indexUnion[0] != 0) {
        indexUnion.push_back(0);
    } else if (indexUnion[indexUnion.size()] != indexUnion.size()) {
        indexUnion.push_back(fileString.size());
    }

    // Sort all restrictions.
    ranges::sort(indexUnion);

    for (size_t  i = 0; i < indexUnion.size(); i++) {
        for (size_t  j = i + 1; j < indexUnion.size(); j++) {
            distances.push_back(indexUnion[j] - indexUnion[i]);
        }
    }

    // Sort the distances.
    ranges::sort(distances);

    return distances;
}

int main(const int argc, char **argv) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <file>" << "<restriction-strings>" << endl;
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
