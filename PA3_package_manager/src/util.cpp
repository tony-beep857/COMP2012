#include "util.h"

#include <fstream>
#include <sstream>

using std::operator""s;

std::vector<std::string_view> split(std::string_view str, char delimiter) {
  std::vector<std::string_view> result;

  if (str.empty()) {
    return result;
  }

  std::size_t start{0};
  std::size_t pos{str.find(delimiter)};
  while (pos != std::string_view::npos) {
    result.emplace_back(str.substr(start, pos - start));
    start = pos + 1;
    pos = str.find(delimiter, start);
  }
  result.emplace_back(str.substr(start));

  return result;
}

Expected<std::string, std::string> read_file(const std::filesystem::path& path) {
  std::ifstream in{path, std::ios::binary};
  if (!in) {
    return Unexpected{"Cannot open file: "s + path.string()};
  }

  std::ostringstream buffer;
  buffer << in.rdbuf();
  if (in.bad()) {
    return Unexpected{"Cannot read file: "s + path.string()};
  }
  return buffer.str();
}
