//
// Created by igork on 21/03/2026.
//

#ifndef IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H
#define IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H

#include <string>
#include <vector>

class RestrictionCarting {
public:

    static std::vector<std::vector<size_t>> bruteForce(std::vector<size_t> &L, size_t n);

    static std::vector<std::vector<size_t>> place(std::vector<size_t> &L, std::vector<size_t> &X, const size_t &width);

    static std::vector<std::vector<size_t>> partialDigest(std::vector<size_t> &L);

    static void solveProblem(const std::string &problemFilePath, const std::string &restrictionsString, const std::string &algorithm, std::ofstream
                             &outputFile, int i);

    static void test(const std::string &algorithm);
};

#endif //IZBRANI_ALGORITMI_KOMBINATORIKE_RESTRICTIONCARTING_H