#include <fstream>
#include <array>
#include <cassert>
#include <cstring>
#include <optional>
#include <map>
#include <set>
#include <iostream>
constexpr auto buffer_size = 64;

#ifndef NDEBUG
std::ostream logging_stream = std::ostream{std::clog.rdbuf()};
#else
std::ostream logging_stream = std::ostream{nullptr};
#endif


std::optional<int> get_priority(const char character)
{
    if (character >= 'A' && character <= 'Z')
        return character - 'A' + 27;
    if (character >= 'a' && character <= 'z')
        return character - 'a' + 1;
    return std::nullopt;
}

bool cleanly_devides_by_2(const int number)
{
    return number % 2 == 0;
}



std::optional<char> duplicate_character(const std::set<char> &first_range, const std::set<char> &second_range)
{
    for (const char character : first_range)
    {
        if (second_range.contains(character))
            return character;
    }
    return std::nullopt;
}

int main(int argc, char const *argv[])
{
    auto input = std::fstream("input.txt");
    auto line_buffer = std::array<char, buffer_size>();
    auto read_line = 0;
    auto total_priority = 0;
    while (input.getline(line_buffer.data(), line_buffer.size()))
    {
        const auto length = std::strlen(line_buffer.data());
        const auto remaining_capacity = line_buffer.size() - length;
        if (!cleanly_devides_by_2(length))
            continue;

        auto first_half = std::set<char>();
        first_half.insert(line_buffer.begin(), line_buffer.end() - remaining_capacity - length / 2);

        auto second_half = std::set<char>();
        second_half.insert(line_buffer.begin() + length / 2, line_buffer.end() - remaining_capacity);

        const auto dup = duplicate_character(first_half, second_half);
        logging_stream << "duplicate characers for line: " << read_line << " is: " << *dup << '\n';
        total_priority += *get_priority(*dup);
        line_buffer.fill('\0');
        read_line++;
    }

    logging_stream << "total priority is: " << total_priority << '\n';

    return EXIT_SUCCESS;
}
