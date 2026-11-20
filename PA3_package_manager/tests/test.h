#ifndef TEST_H
#define TEST_H

#include <array>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief A test case: a function that asserts its expectations and returns nothing.
 */
using TestCaseFnT = void (*)();

/**
 * @brief Selects every case of every linked suite.
 */
constexpr unsigned int ALL_TESTS = 0;

/**
 * @brief Selects every given case; also the last index of the given range (1-100).
 */
constexpr unsigned int ALL_GIVEN_TESTS = 100;

/**
 * @brief Selects every hidden case; also the last index of the hidden range (101-1000).
 */
constexpr unsigned int ALL_HIDDEN_TESTS = 1000;

/**
 * @brief The 1-based position of the case now running within its suite, set by the runner before each call.
 *
 * Fixtures name their directory from it, so a suite's registry order alone decides a case's fixture directory: adding,
 * removing or moving a case needs no renumbering anywhere.
 */
inline unsigned int current_case_position = 0;

/**
 * @brief One suite's cases, in run order, and the index range it occupies.
 */
struct TestSuite {
  /**
   * @brief The cases, in the order the suite runs them.
   */
  std::vector<TestCaseFnT> cases;

  /**
   * @brief The index of the first case; later cases follow in order.
   */
  unsigned int first_test_index;

  /**
   * @brief The index that selects the whole suite, equal to the last index of its range.
   */
  unsigned int all_tests_index;
};

/**
 * @brief Runs every case of `suite`, printing each case's index as it goes.
 *
 * @param suite The suite to run.
 */
void run_suite(const TestSuite& suite);

/**
 * @brief Runs one case, or every case, across `suites` in order.
 *
 * Indices are fixed per suite: given cases number 1-100 and hidden 101-1000, each suite starting from the first index
 * of its range. `ALL_TESTS` runs every case of every linked suite, and a suite's `all_tests_index` (100 or 1000) runs
 * that whole suite. An index in a range's unused tail is out of range.
 *
 * @tparam N The number of suites.
 * @param suites The suites whose cases to run.
 * @param test_case_index A case index, a suite selector, or ALL_TESTS.
 * @return False when the index is out of range or selects a suite that is not linked.
 */
template<std::size_t N>
bool run_suites(const std::array<TestSuite, N>& suites, unsigned int test_case_index) {
  if (test_case_index == ALL_TESTS) {
    for (const TestSuite& suite : suites) {
      run_suite(suite);
    }
    return true;
  }

  for (const TestSuite& suite : suites) {
    if (test_case_index == suite.all_tests_index) {
      run_suite(suite);
      return true;
    }
    if (test_case_index >= suite.first_test_index
        && test_case_index - suite.first_test_index < static_cast<unsigned int>(suite.cases.size())) {
      std::cout << ">>> Running Test Case " << test_case_index << std::endl;
      current_case_position = test_case_index - suite.first_test_index + 1;
      suite.cases[test_case_index - suite.first_test_index]();
      std::cout << ">>> Completed Test Case " << test_case_index << std::endl;
      return true;
    }
  }

  return false;
}

/**
 * @brief Parses an unsigned decimal index from the command line.
 *
 * @param text The text to parse; a null pointer or an empty string is invalid.
 * @param out Receives the parsed index; unchanged on failure.
 * @return True when `text` is a non-empty run of decimal digits that fits an `unsigned int`.
 */
bool parse_uint(const char* text, unsigned int& out);

/**
 * @brief Runs the cases the command line selects, prompting for an index when no argument is given.
 *
 * One argument is parsed as an index. With no argument, prints a prompt and reads one line from stdin, so a student can
 * pick a case interactively. Any other argument count runs every case of every linked suite. An empty, unparsable or
 * out-of-range index prints an error.
 *
 * @tparam N The number of linked suites.
 * @param suites The suites whose cases to run.
 * @param argc The argument count `main` received.
 * @param argv The argument vector `main` received.
 * @return `EXIT_SUCCESS` when the selected cases ran, `EXIT_FAILURE` otherwise.
 */
template<std::size_t N>
int run_tests(const std::array<TestSuite, N>& suites, int argc, const char* argv[]) {
  std::string text;
  if (argc == 1) {
    std::cout << "Enter a test case number: " << std::flush;
    if (!std::getline(std::cin, text)) {
      std::cout << "Error: No test case number given" << std::endl;
      return EXIT_FAILURE;
    }
  } else if (argc == 2) {
    text = argv[1];
  } else {
    return run_suites(suites, ALL_TESTS) ? EXIT_SUCCESS : EXIT_FAILURE;
  }

  unsigned int test_case_index = 0;
  if (!parse_uint(text.c_str(), test_case_index) || !run_suites(suites, test_case_index)) {
    std::cout << "Error: Invalid test case " << text << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

#endif  // TEST_H
