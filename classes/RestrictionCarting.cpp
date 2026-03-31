#include "RestrictionCarting.h"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <cmath>
#include <numeric>

#include "Util.h"


std::vector<std::vector<size_t>> RestrictionCarting::bruteForce(std::vector<size_t> &L, const size_t n) {
    std::set<std::vector<size_t>> unique_results;

    if (!std::ranges::is_sorted(L)) std::ranges::sort(L);
    const std::vector<size_t> original_L = L;

    const size_t m = L.size();
    const size_t number_of_x = n - 2;

    if (number_of_x > m) return {};

    std::vector<size_t> combination(number_of_x);
    std::iota(combination.begin(), combination.end(), 0);

    while (true) {
        std::vector X = {0, L.back()};
        for (const size_t i : combination) X.push_back(L[i]);

        std::ranges::sort(X);
        std::vector<size_t> x_distance = Util::getDistances(X);
        std::ranges::sort(x_distance);

        if (x_distance == original_L) unique_results.insert(X);

        int i = static_cast<int>(number_of_x) - 1;
        while (i >= 0 && combination[i] == m - number_of_x + i) i--;
        if (i < 0) break;

        combination[i]++;
        for (size_t j = i + 1; j < number_of_x; j++) combination[j] = combination[j - 1] + 1;
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

void RestrictionCarting::solveProblem(const std::string& problemFilePath, const std::string& restrictionsString,
    const std::string& algorithm, std::ofstream& outputFile, const int i) {
    bool print = i == 0;

    // Construct multiset.
    const std::string fileString = Util::readFile(problemFilePath);
    const std::vector<std::string> restrictions = Util::splitRestrictions(restrictionsString);
    const std::vector<size_t> indexes = Util::findRestrictionIndexes(restrictions, fileString);
    std::vector<size_t> L = Util::getDistances(indexes);

    // Calculate restriction cut frequencies.
    Util::output(outputFile, "\nRestriction cut frequencies:\n", print);

    for (const auto& restriction : restrictions) {
        // number_of_nucleotide_bases -> A/C/G/T.
        // 1 / (number_of_nucleotide_bases^length_of_the_restriction_cut)
        double frequency = 1.0 / std::pow(4.0, restriction.length());
        Util::output(outputFile, "[" + restriction + "]: " + std::to_string(frequency) + "\n", print);
    }

    Util::output(outputFile, "\nMultiset: ", print);
    for (const auto& d : L) Util::output(outputFile, std::to_string(d)  + ",", print);
    Util::output(outputFile, "\n\n", print);

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

    Util::output(outputFile, "Solution: ", print);
    if (!solutions.empty()) {
        Util::output(outputFile, "\n", print);
        for (const auto& solution: solutions) {
            for (const auto& x : solution) {
                Util::output(outputFile, std::to_string(x) + ",", print);
            }
            Util::output(outputFile, "\n", print);
        }
    } else {
        Util::output(outputFile, "No solution\n", print);
    }

    const auto msDuration = duration_cast<std::chrono::milliseconds>(stop - start);
    const auto microsDuration = duration_cast<std::chrono::microseconds>(stop - start);
    Util::output(
        outputFile,
        "Execution time [i=" + std::to_string(i) + "]: " +
        std::to_string(msDuration.count()) + " ms -> " +
        std::to_string(microsDuration.count()) + " us\n",
        true
    );
}

void RestrictionCarting::test(const std::string& algorithm) {
    const std::string outputFolder = "results/";
    const std::string inputFolder = "examples/";

    std::vector<std::pair<std::string, std::string>> tests;
    const std::vector<std::pair<std::string, std::string>> branchAndBoundTests = {
        { "DNK1.txt", "GTGTG" },
        { "DNK1.txt", "TTCC,CTCTCT" },
        { "DNK1.txt", "AAAA,CCCC,TTTT,GGGG" },
        { "DNK2.txt", "ACTACT,GGAGGA,GAGGCC,CTCTCT" },
        { "DNK3.txt", "TTTTTTT,GTGTCGT,ACACACA" },
    };

    const std::vector<std::pair<std::string, std::string>> bruteForceTests = {
        { "DNK1.txt", "GTGTG" },
        { "DNK1.txt", "TTCC,CTCTCT" },
        { "DNK1.txt", "AAAA,CCCC,TTTT,GGGG" }
    };


    std::ofstream outputFile;
    if (algorithm == "-bf") {
        tests = bruteForceTests;
        outputFile = std::ofstream(outputFolder + "brute_force_results.txt");
        Util::output(outputFile, "BRUTE FORCE\n\n", true);
    } else if (algorithm == "-pd") {
        tests = branchAndBoundTests;
        outputFile = std::ofstream(outputFolder + "branch_and_bound_results.txt");
        Util::output(outputFile, "BRANCH AND BOUND\n\n", true);
    } else {
        Util::output(outputFile, "Algorithm not supported\n\n", true);
        return;
    }

    for (const auto& [problemFile, restrictions] : tests) {
        Util::output(outputFile, problemFile + " [" + restrictions + "]:\n", true);
        try {
            for (int i = 0; i < 100; i++) {
                solveProblem(inputFolder + problemFile, restrictions, algorithm, outputFile, i);
            }
            Util::output(outputFile, "\n", true);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}