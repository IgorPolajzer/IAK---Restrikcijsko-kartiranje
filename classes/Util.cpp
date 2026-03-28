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
    const std::string& dnaString) {
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
        while ((res = dnaString.find(restriction, res)) != std::string::npos) {
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
    indexUnion.push_back(dnaString.size() - 1);

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

void Util::removeElements(std::vector<size_t>& array, const std::vector<size_t>& elementsToRemove) {
    for (const auto& element : elementsToRemove) {
        array.erase(std::ranges::remove(array, element).begin(), array.end());
    }
}

bool Util::isDistanceOfElementsInVector(const size_t &elementOne, const std::vector<size_t> &elements,
                                       const std::vector<size_t> &vector, std::vector<unsigned long long>& distances) {
    bool foundInVector = true;

    for (const auto& element : elements) {
        auto distance = static_cast<long long>(elementOne - element);
        size_t absoluteDistance = std::abs(distance);

        distances.push_back(absoluteDistance);
        if (std::ranges::find(vector, absoluteDistance)==vector.end()) {
            foundInVector = false;
        }
    }

    return foundInVector;
}

void Util::addAndSort(std::vector<size_t>& vector, size_t& elementToAdd) {
    vector.push_back(elementToAdd);
    std::ranges::sort(vector.begin(), vector.end());
}

void Util::removeElementsFromVector(std::vector<size_t>& vector, const std::vector<size_t>& elementsToRemove) {
    for (const auto& element : elementsToRemove) {
        auto it = std::ranges::find(vector, element);
        if (it != vector.end()) {
            vector.erase(it);
        }
    }
}

void Util::removeElementFromVector(std::vector<size_t>& vector, size_t element) {
    removeElementsFromVector(vector, std::vector(1, element));
}

void Util::addElementsToVector(std::vector<size_t>& vector, const std::vector<size_t>& elementToAdd) {
    for (const auto& element : elementToAdd) {
        vector.push_back(element);
    }
}

void Util::deleteDistances(std::vector<size_t>& distances, const size_t& distanceToDelete) {
    for (unsigned long long & i : distances) {
        auto distance = static_cast<long long>(i - distanceToDelete);
        i = std::abs(distance);
    }
}