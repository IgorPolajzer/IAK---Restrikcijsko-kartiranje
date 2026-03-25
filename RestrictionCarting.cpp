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
            std::string fileString = Util::readFile(folder + file);
            std::vector<size_t> indexes = Util::findRestrictionIndexes(restrictions, fileString);
            std::vector<size_t> L = Util::getDistances(indexes);
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