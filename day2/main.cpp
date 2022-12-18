#include <fstream>
#include <cstdlib>
#include <string>
#include <array>
#include <optional>
#include <iostream>

enum class move_t
{
    rock,
    paper,
    scissors
};

std::optional<move_t> get_move_from_character(const char move_char)
{
    switch (move_char)
    {
    case 'A':
        return move_t::rock;
    case 'B':
        return move_t::paper;
    case 'C':
        return move_t::scissors;
    default:
        return std::nullopt;
    }
}

std::optional<move_t> get_response_from_character_and_move(const move_t move, const char response_char)
{
    // X = loss,
    // Y = draw,
    // Z = win
    switch (move)
    {
    case move_t::rock:
        switch (response_char)
        {
        case 'X':
            return move_t::scissors;
        case 'Y':
            return move_t::rock;
        case 'Z':
            return move_t::paper;
        }
        break;
    case move_t::paper:
        switch (response_char)
        {
        case 'X':
            return move_t::rock;
        case 'Y':
            return move_t::paper;
        case 'Z':
            return move_t::scissors;
        }
        break;

    case move_t::scissors:
        switch (response_char)
        {
        case 'X':
            return move_t::paper;
        case 'Y':
            return move_t::scissors;
        case 'Z':
            return move_t::rock;
        }
        break;
    }
    return std::nullopt;
}

int get_score_for_move_and_response(const move_t move, const move_t response)
{
    constexpr auto selection_points_rock = 1;
    constexpr auto selection_points_paper = 2;
    constexpr auto selection_points_scissors = 3;

    constexpr auto result_points_victory = 6;
    constexpr auto result_points_draw = 3;
    constexpr auto result_points_loss = 0;

    switch (move)
    {
    case move_t::rock:
        switch (response)
        {
        case move_t::rock:
            return selection_points_rock + result_points_draw;
        case move_t::paper:
            return selection_points_rock + result_points_loss;
        case move_t::scissors:
            return selection_points_rock + result_points_victory;
        }
    case move_t::paper:
        switch (response)
        {
        case move_t::rock:
            return selection_points_paper + result_points_victory;
        case move_t::paper:
            return selection_points_paper + result_points_draw;
        case move_t::scissors:
            return selection_points_paper + result_points_loss;
        }
    case move_t::scissors:
        switch (response)
        {
        case move_t::rock:
            return selection_points_scissors + result_points_loss;
        case move_t::paper:
            return selection_points_scissors + result_points_victory;
        case move_t::scissors:
            return selection_points_scissors + result_points_draw;
        }
    }
    throw std::runtime_error("invalid enum value given?");
}

int main(int argc, char const *argv[])
{
    constexpr auto buffer_size = 10;
    auto line_buffer = std::array<char, buffer_size>();
    line_buffer.fill('\0');
    auto input = std::fstream("input.txt");

    auto total_score = 0;
    while (input.getline(line_buffer.data(), line_buffer.size()))
    {
        const auto opponent_move = get_move_from_character(line_buffer.at(0));
        if(!opponent_move.has_value())
        {
            line_buffer.fill('\0');
            continue;
        }

        const auto my_move = get_response_from_character_and_move(*opponent_move, line_buffer.at(2));
        if (!my_move.has_value())
        {
            line_buffer.fill('\0');
            continue;
        }
        total_score += get_score_for_move_and_response(*my_move, *opponent_move);
        line_buffer.fill('\0');
    }

    std::cout << "total score: " << total_score << '\n';

    return EXIT_SUCCESS;
}
