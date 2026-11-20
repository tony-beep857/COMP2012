#include "given_tests.h"
#include "test.h"

#include <array>

int main(int argc, const char* argv[]) {
  const std::array<TestSuite, 1> suites = {given_tests()};
  return run_tests(suites, argc, argv);
}
