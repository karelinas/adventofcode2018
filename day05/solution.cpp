// g++ -std=c++14 -Wall -O3 solution.cpp

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <list>
#include <type_traits>
#include <vector>

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

template <typename Iterator>
auto peek_back(Iterator it, Iterator begin) -> typename std::remove_reference<decltype(*it)>::type
{
    if (it == begin) {
        return typename std::remove_reference<decltype(*it)>::type{};
    } else {
        it--;
        return *it;
    }
}

void reduce(std::list<char>& polymer)
{
    auto pos = polymer.begin();
    pos++;
    while (pos != polymer.end()) {
        if (reacts(*pos, peek_back(pos, polymer.begin()))) {
            pos--;
            pos = polymer.erase(pos);
            pos = polymer.erase(pos);
        } else {
            pos++;
        }
    }
}

int reduced_length(const std::list<char>& polymer)
{
    std::list<char> copied_polymer{std::begin(polymer), std::end(polymer)};
    reduce(copied_polymer);
    return copied_polymer.size();
}

int length_without_unit(const std::list<char>& polymer, char unit)
{
    std::list<char> copied_polymer{std::begin(polymer), std::end(polymer)};
    char unit_capital = std::toupper(unit);
    copied_polymer.remove(unit);
    copied_polymer.remove(unit_capital);
    reduce(copied_polymer);
    return copied_polymer.size();
}

int main()
{
    auto polymer = read_polymer("data.txt");

    // part 1
    std::cout << "Reduced polymer length: " << reduced_length(polymer) << '\n';

    // part 2
    std::vector<int> lengths;
    for (char unit = 'a'; unit <= 'z'; unit++) {
        lengths.push_back(length_without_unit(polymer, unit));
    }
    int min_length = *std::min_element(std::begin(lengths), std::end(lengths));
    std::cout << "Shortest reduced polymer after problem removal: " << min_length << '\n';
    return EXIT_SUCCESS;
}
