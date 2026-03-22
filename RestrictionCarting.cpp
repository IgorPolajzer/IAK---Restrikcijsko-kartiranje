#include "RestrictionCarting.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <ranges>
#include <set>
#include <stdexcept>

int RestrictionCarting::readFile(const std::string& fileName, std::string& stringFile) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return 1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    stringFile = buffer.str();
    stringFile = stringFile.substr(0, stringFile.find_last_not_of(" \t\n\r") + 1);
    return 0;
}

std::vector<size_t> RestrictionCarting::findRestrictionIndexes(const std::string& restrictionStrings,
    const std::string& fileString) {
    std::vector<std::string> restrictions;
    std::vector<std::vector<size_t>> indexes;
    std::vector<size_t> indexUnion;

    // Split CSV into strings.
    std::stringstream ss(restrictionStrings);
    while (ss.good()) {
        std::string substr;
        getline(ss, substr, ',');
        restrictions.push_back(substr);
    }

    // Find indexes for restriction strings.
    for (auto& restriction : restrictions) {
        std::vector<size_t> restrictionIndexes;
        size_t res = 0;
        while ((res = fileString.find(restriction, res)) != std::string::npos) {
            restrictionIndexes.push_back(res);
            ++res;
        }
        indexes.push_back(restrictionIndexes);
    }

    if (indexes.empty()) throw std::runtime_error("No restrictions found");

    for (const auto& index : indexes) {
        indexUnion.insert(indexUnion.end(), index.begin(), index.end());
    }

    // Add beginning and end of the DNA.
    indexUnion.push_back(0);
    indexUnion.push_back(fileString.size() - 1);

    std::ranges::sort(indexUnion);

    return indexUnion;
}

std::vector<size_t> RestrictionCarting::getDistances(const std::vector<size_t>& restrictionIndexes) {
    std::vector<size_t> distances;

    for (size_t i = 0; i < restrictionIndexes.size(); i++) {
        for (size_t j = i + 1; j < restrictionIndexes.size(); j++) {
            distances.push_back(restrictionIndexes[j] - restrictionIndexes[i]);
        }
    }

    // Sort the distances.
    std::ranges::sort(distances);
    return distances;
}

std::vector<size_t> RestrictionCarting::getDistances(const std::string& fileName, const std::string& restrictionStrings) {
    std::vector<size_t> distances;
    std::string fileString;

    if (readFile(fileName, fileString) != 0) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    return getDistances(findRestrictionIndexes(restrictionStrings, fileString));
}

std::vector<std::vector<size_t>> RestrictionCarting::bruteForce(std::vector<size_t> &L) {
    std::set<std::vector<size_t>> unique_results;

    std::ranges::sort(L);
    const size_t max = L.back();
    std::vector<std::vector<size_t>> result;// vector of Xi

    // Store original L for comparison.
    auto original_L = L;
    std::ranges::sort(original_L);

    // Remove duplicate distances from L.
    const auto it = std::ranges::unique(L).begin();
    L.erase(it, L.end());

    do {
        std::vector<size_t> X;

        X.push_back(0);
        for (int i = 1; i < L.size() - 1; i++) {
            X.push_back(L[i]);
        }
        X.push_back(max);

        // Sort before comparison.
        std::vector<size_t> distances = getDistances(X);
        std::ranges::sort(distances);

        if (distances == original_L) unique_results.insert(X);
    } while (std::ranges::next_permutation(L).found);

    return {unique_results.begin(), unique_results.end()};
}


std::vector<size_t> RestrictionCarting::place(std::vector<size_t> &L, std::vector<size_t> &X) {
    if (L.empty()) {
        return X;
    }

    // if delta (y, X).
    size_t y = L.back();
    bool notPresent = false;
    std::vector<size_t> distances;

    for (const auto& el : X) {
        size_t distance = y - el;
        distances.push_back(distance);
        if (std::ranges::find(L, distance)==L.end()) {
            notPresent = true;
        }
    }

    if (!notPresent) {
        X.push_back(y);

        for (const auto& el : distances) {

        }
    }


}

std::vector<std::vector<size_t>> RestrictionCarting::branchAndBound(std::vector<size_t> &L) {
    std::vector<std::vector<size_t>> result;

    std::ranges::sort(L);
    const size_t width = L.back();


    return result;
}


void RestrictionCarting::test() {
    const std::string folder = "examples/";
    const std::vector<std::pair<std::string, std::string>> tests = {
        { "DNK1.txt", "GTGTG" },
        { "DNK1.txt", "TTCC,CTCTCT" },
        { "DNK1.txt", "AAAA,CCCC,TTTT,GGGG" },
        { "DNK2.txt", "ACTACT,GGAGGA,GAGGCC,CTCTCT" },
        { "DNK3.txt", "TTTTTTT,GTGTCGT,ACACACA" },
    };

    for (const auto& [file, restrictions] : tests) {
        std::cout << file << " [" << restrictions << "]:" << std::endl;
        try {
            // Construct multiset.
            std::vector<size_t> L = getDistances(folder + file, restrictions);
            std::cout << "Multiset: ";
            for (const auto& d : L) std::cout << d << ",";
            std::cout << std::endl << std::endl;

            // Solve restriction carting problem.
            std::vector<std::vector<size_t>> solutions = branchAndBound(L);

            std::cout << "Solution: " << std::endl;
            for (const auto& solution: solutions) {
                for (const auto& x : solution) {
                    std::cout << x << ",";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
