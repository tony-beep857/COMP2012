#ifndef UTIL_H
#define UTIL_H

#include <filesystem>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "expected.h"

/**
 * @brief Reads one nibble (4 bits) of an integral value, most significant first.
 *
 * @tparam T An integral type.
 * @param value The value to read from.
 * @param index The nibble index, 0-based from the most significant nibble.
 * @return The nibble, in `[0x0, 0xF]`.
 */
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
std::size_t nibble(T value, std::size_t index) {
  return static_cast<std::size_t>((value >> (((sizeof(value) * 2) - 1 - index) * 4)) & T{0xF});
}

/**
 * @brief Splits a string on a delimiter.
 *
 * Empty fields are kept, so every delimiter adds one field; the returned views alias `str`, which must outlive them.
 *
 * @param str The text to split.
 * @param delimiter The separator character.
 * @return The fields, in order; empty when `str` is empty.
 */
std::vector<std::string_view> split(std::string_view str, char delimiter);

/**
 * @brief Reads a whole file, binary-safe.
 *
 * @param path The file to read.
 * @return The file's contents, or a message naming the path.
 */
Expected<std::string, std::string> read_file(const std::filesystem::path& path);

#endif  // UTIL_H
