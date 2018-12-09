// g++ -std=c++14 -Wall solution.cpp -o solution

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <vector>

// 418 players; last marble is worth 71339 points
const int PLAYER_COUNT = 418;
const int LAST_MARBLE1 = 71339;
// part 2
const int LAST_MARBLE2 = LAST_MARBLE1 * 100;

// every 23rd marble is special
const int SPECIAL_MARBLE = 23;

struct marble
{
    int value;
    marble* previous;
    marble* next;
};

marble* insert_marble(marble* current, int value)
{
    current = current->next;
    auto new_marble = new marble{value, current, current->next};
    current->next = new_marble;
    new_marble->next->previous = new_marble;
    return new_marble;
}

marble* special_marble(marble* current, int value, uint64_t* score)
{
    *score += value;
    for (int i = 0; i < 7; i++)
        current = current->previous;
    *score += current->value;
    current->next->previous = current->previous;
    current->previous->next = current->next;
    auto next = current->next;
    delete current;
    return next;
}

void delete_marbles(marble* current)
{
    while (current) {
        auto next = current->next;
        current->previous->next = nullptr;
        delete current;
        current = next;
    }
}

uint64_t play_game(int player_count, int marbles)
{
    std::vector<uint64_t> player_scores(player_count, 0);
    auto current = new marble{0, nullptr, nullptr};
    current->next = current;
    current->previous = current;
    for (int value = 1; value <= marbles; value++) {
        if ((value % SPECIAL_MARBLE) == 0) {
            auto player = &player_scores.at((value - 1) % player_count);
            current = special_marble(current, value, player);
        } else {
            current = insert_marble(current, value);
        }
    }
    auto max = *std::max_element(std::begin(player_scores),
                                 std::end(player_scores));
    delete_marbles(current);
    return max;
}

int main()
{
    auto game1 = play_game(PLAYER_COUNT, LAST_MARBLE1);
    auto game2 = play_game(PLAYER_COUNT, LAST_MARBLE2);

    std::cout << "Max score with " << LAST_MARBLE1 << " marbles: " << game1 << '\n';
    std::cout << "Max score with " << LAST_MARBLE2 << " marbles: " << game2 << '\n';
}
