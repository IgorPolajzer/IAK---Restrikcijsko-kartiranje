//
// Created by igork on 21/03/2026.
//

#ifndef IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H
#define IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H

#include <string>
#include <vector>

class RestrictionCarting {
public:

    // Brute force algorithm.
    static std::vector<std::vector<size_t>> bruteForce(std::vector<size_t> &L, size_t n);

    static std::vector<size_t> place(std::vector<size_t> &L, std::vector<size_t> &X);

    static std::vector<std::vector<size_t>> branchAndBound(std::vector<size_t> &L);

    static void solveProblem(const std::string &filePath, const std::string &restrictions, const std::string &algorithm);

    // A test function.
    static void test(std::string algorithm);
};

#endif //IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H