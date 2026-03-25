#include "RestrictionCarting.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <ranges>
#include <set>
#include <stdexcept>

std::vector<std::vector<size_t>> getAllCombinations(const std::vector<size_t> &v, const size_t r) {
    std::vector<std::vector<size_t>> results;
    size_t n = v.size();
    if (r > n) return results;

    std::vector<size_t> indices(r);
    std::iota(indices.begin(), indices.end(), 0);

    while (true) {
        std::vector<size_t> combo;
        for (const size_t i : indices)
            combo.push_back(v[i]);
        results.push_back(combo);

        int i = static_cast<int>(r) - 1;
        while (i >= 0 && indices[i] == n - r + i)
            i--;
        if (i < 0) break;
        indices[i]++;
        for (size_t j = i + 1; j < r; j++)
            indices[j] = indices[j - 1] + 1;
    }

    return results;
}

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

    if (!std::ranges::is_sorted(indexUnion)) std::ranges::sort(indexUnion);

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
    if (!std::ranges::is_sorted(distances)) std::ranges::sort(distances);
    return distances;
}

std::string RestrictionCarting::readFile(const std::string& fileName) {
    std::string fileString;

    if (readFile(fileName, fileString) != 0) {
        throw std::runtime_error("Failed to open file: " + fileName);
    }

    return fileString;
}

std::vector<std::vector<size_t>> RestrictionCarting::bruteForce(std::vector<size_t> &L, const size_t n) {
    std::set<std::vector<size_t>> unique_results;
    std::vector<std::vector<size_t>> result;

    if (!std::ranges::is_sorted(L)) std::ranges::sort(L);
    const std::vector<size_t> original_L = L;

    std::vector<std::vector<size_t>> combinations = getAllCombinations(L, n - 2);

    for (const auto& comb : combinations) {
        std::vector X = { 0, L.back() };
        X.insert(X.end(), comb.begin(), comb.end());
        std::ranges::sort(X);
        std::vector<size_t> x_distance = getDistances(X);
        std::ranges::sort(x_distance);

        if (x_distance == original_L) {
            unique_results.insert(X);
        }
    }

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

    if (!std::ranges::is_sorted(L)) std::ranges::sort(L);
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
            std::string fileString = readFile(folder + file);
            std::vector<size_t> indexes = findRestrictionIndexes(restrictions, fileString);
            std::vector<size_t> L = getDistances(indexes);
            std::cout << "Multiset: ";
            for (const auto& d : L) std::cout << d << ",";
            std::cout << std::endl << std::endl;

            // Solve restriction carting problem.
            auto start = std::chrono::high_resolution_clock::now();
            std::vector<std::vector<size_t>> solutions = bruteForce(L, indexes.size());
            auto stop = std::chrono::high_resolution_clock::now();

            std::cout << "Solution: ";
            if (!solutions.empty()) {
                std::cout << std::endl;
                for (const auto& solution: solutions) {
                    for (const auto& x : solution) {
                        std::cout << x << ",";
                    }
                    std::cout << std::endl;
                }
            } else {
                std::cout << "No solution" << std::endl;
            }

            auto msDuration = duration_cast<std::chrono::milliseconds>(stop - start);
            auto microsDuration = duration_cast<std::chrono::microseconds>(stop - start);
            std::cout << "Execution time: " << msDuration << " -> " << microsDuration << std::endl;

            std::cout << std::endl << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}