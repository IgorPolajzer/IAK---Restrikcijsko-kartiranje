#include "RestrictionCarting.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>

#include "Util.h"


std::vector<std::vector<size_t>> RestrictionCarting::bruteForce(std::vector<size_t> &L, const size_t n) {
    std::set<std::vector<size_t>> unique_results;
    std::vector<std::vector<size_t>> result;

    if (!std::ranges::is_sorted(L)) std::ranges::sort(L);
    const std::vector<size_t> original_L = L;

    std::vector<std::vector<size_t>> combinations = Util::getAllCombinations(L, n - 2);

    for (const auto& comb : combinations) {
        std::vector X = { 0, L.back() };
        X.insert(X.end(), comb.begin(), comb.end());
        std::ranges::sort(X);
        std::vector<size_t> x_distance = Util::getDistances(X);
        std::ranges::sort(x_distance);

        if (x_distance == original_L) {
            unique_results.insert(X);
        }
    }

    return {unique_results.begin(), unique_results.end()};
}

std::vector<std::vector<size_t>> RestrictionCarting::place(std::vector<size_t> &L, std::vector<size_t> &X, const size_t& width) {
    std::vector<std::vector<size_t>> finalResults;

    if (L.empty()) {
        finalResults.push_back(X);
        return finalResults;
    };

    if (!std::ranges::is_sorted(L)) std::ranges::sort(L);
    size_t y = L.back();

    std::vector<size_t> distancesOne;
    if (Util::isDistanceOfElementsInVector(y, X, L, distancesOne)) {
        Util::addAndSort(X, y);
        Util::removeElementsFromVector(L, distancesOne);

        std::vector<std::vector<size_t>> results = place(L, X, width);
        for (const auto& res: results) {
            finalResults.push_back(res);
        }

        Util::removeElementFromVector(X, y);
        Util::addElementsToVector(L, distancesOne);
    }

    std::vector<size_t> distancesTwo;
    size_t widthDifference = width - y;
    if (Util::isDistanceOfElementsInVector(widthDifference, X, L, distancesTwo)) {
        Util::addAndSort(X, widthDifference);
        Util::removeElementsFromVector(L, distancesTwo);

        std::vector<std::vector<size_t>> results = place(L, X, width);
        for (const auto& res: results) {
            finalResults.push_back(res);
        }

        Util::removeElementFromVector(X, widthDifference);
        Util::addElementsToVector(L, distancesTwo);
    }

    return finalResults;
}

std::vector<std::vector<size_t>> RestrictionCarting::partialDigest(std::vector<size_t> &L) {
    const size_t width = L.back();
    Util::removeElementFromVector(L, width);
    std::vector X = {0, width};
    return place(L, X, width);
}

void RestrictionCarting::solveProblem(const std::string& filePath, const std::string& restrictions, const std::string& algorithm) {
    // Construct multiset.
    std::string fileString = Util::readFile(filePath);
    std::vector<size_t> indexes = Util::findRestrictionIndexes(restrictions, fileString);
    std::vector<size_t> L = Util::getDistances(indexes);

    std::cout << "Multiset: ";
    for (const auto& d : L) std::cout << d << ",";
    std::cout << std::endl << std::endl;

    // Solve restriction carting problem.
    std::vector<std::vector<size_t>> solutions;
    auto start = std::chrono::high_resolution_clock::now();
    if (algorithm == "-bf") {
        solutions = bruteForce(L, indexes.size());
    } else if (algorithm == "-pd") {
        solutions = partialDigest(L);
    } else {
        return;
    }
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
}

void RestrictionCarting::test(const std::string& algorithm) {
    const std::string folder = "examples/";
    const std::vector<std::pair<std::string, std::string>> tests = {
        { "DNK1.txt", "GTGTG" },
        { "DNK1.txt", "TTCC,CTCTCT" },
        { "DNK1.txt", "AAAA,CCCC,TTTT,GGGG" },
        { "DNK2.txt", "ACTACT,GGAGGA,GAGGCC,CTCTCT" },
        { "DNK3.txt", "TTTTTTT,GTGTCGT,ACACACA" },
    };

    if (algorithm == "-bf") {
        std::cout << "BRUTE FORCE" << std::endl << std::endl;
    } else if (algorithm == "-pd") {
        std::cout << "BRANCH AND BOUND" << std::endl << std::endl;
    } else {
        std::cout << "Algorithm not supported" << std::endl << std::endl;
        return;
    }

    for (const auto& [file, restrictions] : tests) {
        std::cout << file << " [" << restrictions << "]:" << std::endl;
        try {
            solveProblem(folder + file, restrictions, algorithm);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}