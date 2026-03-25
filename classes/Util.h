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
                                                      const std::string& dnaString);

    static std::vector<size_t> getDistances(const std::vector<size_t> &restrictionIndexes);

    // Computes all pairwise distances between restriction indexes in a file.
    static std::string readFile(const std::string& fileName);

    static void removeElements(std::vector<size_t> &array, const std::vector<size_t> &elementsToRemove);

    static bool isDistanceOfElementsInVector(const size_t &elementOne, const std::vector<size_t> &elements,
                                            const std::vector<size_t> &vector, std::vector<unsigned long long> distances = std::vector<size_t>());

    static void removeElementsFromVector(std::vector<size_t> &vector, const std::vector<size_t> &elementsToRemove);

    static void removeElementFromVector(std::vector<size_t> &vector, size_t element);

    static void addElementsToVector(std::vector<size_t> &vector, const std::vector<size_t> &elementToAdd);

    // Read file into a string.
    static std::vector<std::vector<size_t>> getAllCombinations(const std::vector<unsigned long long> & vector, size_t size);
};


#endif //IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H