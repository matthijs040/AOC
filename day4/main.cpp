#include <algorithm>
#include <cassert>
#include <charconv>
#include <cstdint>
#include <expected>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

std::expected<std::string, std::error_code>
extract_digit_range(const std::string_view &hypen_seperated_outer_values) {
  std::expected<std::string, std::error_code> ret;

  std::uint32_t lower_digit = 0;
  auto results = std::from_chars(hypen_seperated_outer_values.data(),
                                 hypen_seperated_outer_values.data() +
                                     hypen_seperated_outer_values.size(),
                                 lower_digit);
  if (results.ec != std::errc())
    return ret = std::unexpected(std::make_error_code(results.ec));

  std::uint32_t higher_digit = 0;
  results = std::from_chars(results.ptr + 1,
                            hypen_seperated_outer_values.data() +
                                hypen_seperated_outer_values.size(),
                            higher_digit);

  if (results.ec != std::errc())
    return ret = std::unexpected(std::make_error_code(results.ec));

  if (lower_digit > higher_digit)
    return ret = std::unexpected(
               std::make_error_code(std::errc::invalid_argument));

  const auto num_digits = (higher_digit - lower_digit) + 1;

  auto digits = std::vector<uint32_t>(num_digits);
  std::iota(digits.begin(), digits.end(), lower_digit);

  auto output = std::string();
  for (const auto digit : digits)
    output.append(std::to_string(digit));

  return ret = output;
}

std::expected<std::pair<std::string, std::string>, std::error_code>
extract_two_ranges(const std::string_view &values) {
  std::expected<std::pair<std::string, std::string>, std::error_code> ret;
  auto sep_index = std::find(values.begin(), values.end(), ',');
  if (sep_index == values.end())
    return ret = std::unexpected(
               std::make_error_code(std::errc::invalid_argument));

  std::pair<std::string, std::string> output;
  auto result = extract_digit_range(values);
  if (!result.has_value())
    return ret = std::unexpected(result.error());
  output.first = result.value();

  result = extract_digit_range(
      {sep_index + 1, static_cast<size_t>(values.end() - sep_index)});
  if (!result.has_value())
    return ret = std::unexpected(result.error());
  output.second = result.value();

  return ret = output;
}

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file_path>\n";
    return EXIT_FAILURE;
  }

  auto input = std::fstream("test_input.txt");
  std::vector<std::pair<std::string, std::string>> texts;
  uint32_t count = 0;

  while (!input.eof()) {
    std::string line;
    input >> line;
    auto result = extract_two_ranges(line);
    if (!result.has_value()) {
      std::cerr << "Caught error: " << result.error().message() << "\n";
      return EXIT_FAILURE;
    }
    std::cout << "found digits: " << result.value().first << ","
              << result.value().second << "\n";

    if (result.value().first.contains(result.value().second)
     || result.value().second.contains(result.value().first))
      count++;
  }

  std::cout << "num of substrings: " << count << "\n";

  return EXIT_SUCCESS;
}
