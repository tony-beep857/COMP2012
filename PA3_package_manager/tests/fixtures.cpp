#include "fixtures.h"

#include "test.h"

#include <cassert>
#include <fstream>
#include <string>

void write_text(const std::filesystem::path& path, const std::string& text) {
  std::ofstream output{path};
  // NOLINTBEGIN(misc-static-assert)
  assert(output && "failed to open a test fixture file");
  output << text;
  assert(output && "failed to write a test fixture file");
  // NOLINTEND(misc-static-assert)
}

std::filesystem::path case_root(const std::string& suite) {
  const std::filesystem::path root =
      std::filesystem::current_path() / ("pa3_test_" + suite + "_" + std::to_string(current_case_position));
  std::filesystem::remove_all(root);
  std::filesystem::create_directories(root);
  return root;
}

void write_package(const std::filesystem::path& dir,
                   const std::string& name,
                   const std::string& version,
                   const std::string& depends,
                   const std::string& conflicts) {
  std::string text = "pkgname=" + name + "\ndesc=test package\npkgver=" + version + "\n";
  if (!depends.empty()) {
    text += "depends=" + depends + "\n";
  }
  if (!conflicts.empty()) {
    text += "conflicts=" + conflicts + "\n";
  }
  write_text(dir / name, text);
}

std::filesystem::path write_remote(const std::filesystem::path& dir, const std::vector<std::string>& names) {
  std::string index;
  for (const std::string& name : names) {
    index += name + "\n";
  }
  write_text(dir / "index", index);
  return dir / "index";
}

void write_local_index(const std::filesystem::path& path, const std::vector<std::string>& lines) {
  std::string text;
  for (const std::string& line : lines) {
    text += line + "\n";
  }
  write_text(path, text);
}

std::filesystem::path write_standard_remote(const std::filesystem::path& dir) {
  std::filesystem::create_directories(dir);
  write_package(dir, "numcpp", "1.0.0");
  write_package(dir, "octplotlib", "1.0.0");
  write_package(dir, "blanket", "1.0.0");
  write_package(dir, "tensorstream", "1.0.0", "numcpp octplotlib", "cpptorch");
  write_package(dir, "cpptorch", "1.0.0", "numcpp", "tensorstream");
  write_package(dir, "sarek", "1.0.0", "tensorstream", "scikit-img");
  write_package(dir, "scikit-img", "1.0.0");
  return write_remote(dir, {"numcpp", "octplotlib", "blanket", "tensorstream", "cpptorch", "sarek", "scikit-img"});
}

std::set<int> as_set(const std::vector<int>& values) {
  return std::set<int>{values.begin(), values.end()};
}
