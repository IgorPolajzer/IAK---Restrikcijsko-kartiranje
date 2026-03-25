//
// Created by igork on 25/03/2026.
//

#ifndef IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H
#define IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H
#include <string>
#include <vector>


class Util {
public:
    // Finds all indexes of restriction substrings in the given file string.
    static std::vector<size_t> findRestrictionIndexes(const std::string& restrictionStrings,
                                                      const std::string& fileString);

    static std::vector<size_t> getDistances(const std::vector<size_t> &restrictionIndexes);

    // Computes all pairwise distances between restriction indexes in a file.
    static std::string readFile(const std::string& fileName);

    // Read file into a string.
    static std::vector<std::vector<size_t>> getAllCombinations(const std::vector<unsigned long long> & vector, size_t size);
};


#endif //IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H