#include "test.h"

#include <limits>

void run_suite(const TestSuite& suite) {
  for (std::size_t i = 0; i < suite.cases.size(); ++i) {
    const unsigned int test_case_index = suite.first_test_index + static_cast<unsigned int>(i);
    std::cout << ">>> Running Test Case " << test_case_index << std::endl;
    current_case_position = static_cast<unsigned int>(i) + 1;
    suite.cases[i]();
    std::cout << ">>> Completed Test Case " << test_case_index << std::endl;
  }
}

bool parse_uint(const char* text, unsigned int& out) {
  if (text == nullptr || *text == '\0') {
    return false;
  }

  unsigned long value = 0;
  for (const char* digit = text; *digit != '\0'; ++digit) {
    if (*digit < '0' || *digit > '9') {
      return false;
    }
    value = (value * 10) + static_cast<unsigned long>(*digit - '0');
    if (value > std::numeric_limits<unsigned int>::max()) {
      return false;
    }
  }

  out = static_cast<unsigned int>(value);
  return true;
}
