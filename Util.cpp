//
// Created by igork on 25/03/2026.
//
#include "Util.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <ranges>
#include <stdexcept>

std::vector<std::vector<size_t>> Util::getAllCombinations(const std::vector<size_t> &v, const size_t r) {
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

std::vector<size_t> Util::findRestrictionIndexes(const std::string& restrictionStrings,
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

std::vector<size_t> Util::getDistances(const std::vector<size_t>& restrictionIndexes) {
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

std::string Util::readFile(const std::string& fileName) {
    std::string stringFile;

    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return "";
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    stringFile = buffer.str();
    stringFile = stringFile.substr(0, stringFile.find_last_not_of(" \t\n\r") + 1);

    return stringFile;
}
