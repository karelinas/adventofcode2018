// g++ -std=c++14 -Wall -O3 solution.cpp

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <limits>
#include <list>
#include <stack>
#include <type_traits>

std::list<char> read_polymer(std::string file)
{
    std::list<char> polymer;
    char ch = '\0';
    std::fstream data(file, std::fstream::in);
    while (data >> std::noskipws >> ch) {
        if (std::isalpha(ch)) {
            polymer.push_back(ch);
        }
    }
    return polymer;
}

bool reacts(char lunit, char runit)
{
    return std::abs(lunit - runit) == ('a' - 'A');
}

int reduced_length(const std::list<char>& polymer)
{
    std::stack<char> reduced;
    for (auto unit : polymer) {
        if (reduced.size() > 0 && reacts(unit, reduced.top())) {
            reduced.pop();
        } else {
            reduced.push(unit);
        }
    }
    return reduced.size();
}

int reduced_length_without(const std::list<char>& polymer, char without)
{
    std::stack<char> reduced;
    for (auto unit : polymer) {
        if (std::toupper(unit) == std::toupper(without)) {
            continue;
        }
        if (reduced.size() > 0 && reacts(unit, reduced.top())) {
            reduced.pop();
        } else {
            reduced.push(unit);
        }
    }
    return reduced.size();
}

int main()
{
    auto polymer = read_polymer("data.txt");

    // part 1
    std::cout << "Reduced polymer length: " << reduced_length(polymer) << '\n';

    // part 2
    int min_length = std::numeric_limits<int>::max();
    for (char unit = 'a'; unit <= 'z'; unit++) {
        int length = reduced_length_without(polymer, unit);
        if (length < min_length) min_length = length;
    }
    std::cout << "Shortest reduced polymer after problem removal: " << min_length << '\n';
    return EXIT_SUCCESS;
}
