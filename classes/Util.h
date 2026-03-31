//
// Created by igork on 25/03/2026.
//

#ifndef IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H
#define IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H
#include <string>
#include <vector>


class Util {
public:
    static std::vector<size_t> findRestrictionIndexes(const std::vector<std::string>& restrictions, const std::string &dnaString);

    static std::vector<size_t> getDistances(const std::vector<size_t> &restrictionIndexes);

    static std::string readFile(const std::string& fileName);

    static void removeElements(std::vector<size_t> &array, const std::vector<size_t> &elementsToRemove);

    static bool isDistanceOfElementsInVector(const size_t &elementOne, const std::vector<size_t> &elements,
                                             const std::vector<size_t> &vector, std::vector<unsigned long long> &distances);

    static void addAndSort(std::vector<size_t> &vector, size_t &elementToAdd);

    static void removeElementsFromVector(std::vector<size_t> &vector, const std::vector<size_t> &elementsToRemove);

    static void removeElementFromVector(std::vector<size_t> &vector, size_t element);

    static void addElementsToVector(std::vector<size_t> &vector, const std::vector<size_t> &elementToAdd);

    static void deleteDistances(std::vector<size_t> &distances, const size_t &distanceToDelete);

    static void output(std::ostream &file, const std::string &msg, bool print);

    static std::vector<std::vector<size_t>> getAllCombinations(const std::vector<unsigned long long> & vector, size_t size);

    static std::vector<std::string> splitRestrictions(const std::string &filePath);
};


#endif //IZBRANI_ALGORITMI_KOMBINATORIKE_UTIL_H