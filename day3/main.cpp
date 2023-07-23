#include <fstream>
#include <array>
#include <cassert>
#include <cstring>
#include <optional>
#include <map>
#include <set>
#include <iostream>
#include <string_view>
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

template <typename ContainedT, typename Container>
auto contain(ContainedT key, Container container)
{
    return container.contains(key);
}

template <typename ContainedT, typename Container, typename... Containers>
auto contain(ContainedT key, Container container, Containers... containers)
{
    if (container.contains(key))
        return contain(key, containers...);
    return false;
}

template <typename KeyT, typename Container, typename... Containers>
auto shared_key(Container container, Containers... containers)
{
    for (const auto key : container)
    {
        if (contain(key, containers...))
            return std::optional(key);
    }
    return std::optional<KeyT>();
}

int main(int argc, char const *argv[])
{
    auto input = std::fstream("input2.txt");
    auto line_buffer = std::array<char, buffer_size>();
    auto total_priority = 0;
    while (input.good())
    {
        const auto get_new_set = [&]
        {
            input.getline(line_buffer.data(), line_buffer.size());
            const auto new_set = std::set<char>(line_buffer.begin(), line_buffer.begin() + std::strlen(line_buffer.data()));
            line_buffer.fill('\0');

            return new_set;
        };

        const auto first_set = get_new_set();
        const auto second_set = get_new_set();
        const auto third_set = get_new_set();

        const auto found_key = shared_key<char>(first_set, second_set, third_set);
        logging_stream << "duplicate character is: " << *found_key << '\n';
        total_priority += *get_priority(*found_key);
    }

    logging_stream << "total priority is: " << total_priority << '\n';

    return EXIT_SUCCESS;
}
