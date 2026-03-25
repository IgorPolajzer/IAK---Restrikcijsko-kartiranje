//
// Created by igork on 21/03/2026.
//

#ifndef IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H
#define IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H

#include <string>
#include <vector>

class RestrictionCarting {
    // Reads the entire file into a string.
    static int readFile(const std::string& fileName, std::string& stringFile);

public:
    // Finds all indexes of restriction substrings in the given file string.
    static std::vector<size_t> findRestrictionIndexes(const std::string& restrictionStrings,
                                                      const std::string& fileString);

    static std::vector<size_t> getDistances(const std::vector<size_t> &restrictionIndexes);

    // Computes all pairwise distances between restriction indexes in a file.
    static std::string readFile(const std::string& fileName);

    // Brute force algorithm.
    static std::vector<std::vector<size_t>> bruteForce(std::vector<size_t> &L, size_t n);

    static std::vector<size_t> place(std::vector<size_t> &L, std::vector<size_t> &X);

    static std::vector<std::vector<size_t>> branchAndBound(std::vector<size_t> &L);

    // A test function.
    static void test();
};

#endif //IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H