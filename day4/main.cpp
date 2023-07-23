#include <cassert>
#include <charconv>
#include <expected>
#include <fstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <string_view>
#include <utility>

#include <cstdint>

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

  const auto num_digits = higher_digit - lower_digit;

  auto output = std::string(num_digits, '\0');
  std::iota(output.begin(), output.end(), lower_digit);
  return ret = output;
}

int main(int argc, char const *argv[]) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file_path>\n";
    return EXIT_FAILURE;
  }

  auto input = std::fstream("test_input.txt");
  std::string line;
  input >> line;

  auto result = extract_digit_range(line);
  if (auto error = result.error()) {
    std::cerr << "Caught error: " << error.message() << "\n";
    return EXIT_FAILURE;
  }

  std::cout << "found digits: " << result.value() << "\n";

  return EXIT_SUCCESS;
}
