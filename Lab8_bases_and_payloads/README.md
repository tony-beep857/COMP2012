# COMP 2012 Lab 8: Bases and Payloads

## Introduction
This repository contains the starter code and implementation for **Lab 8: Bases and Payloads** of the COMP 2012 Object-Oriented Programming and Data Structures course. 

The project is a logistics simulator that models the transportation of items between several bases using payloads (shipments), each with a specific travel time. The simulator utilizes various C++ standard library components, including ordered maps, pairs, and priority queues, along with custom iterators.

For full details, please refer to the official lab manual: "COMP 2012 Lab 8: Bases and Payloads.pdf".

## Learning Objectives
After completing this lab, you should be able to:
1. Gain a basic understanding of how iterators work and how to use them.
2. Use a loop to summarize values stored in an ordered map (`std::map`).
3. Use a container adaptor, specifically `std::priority_queue`, to solve problems (including implementing lazy deletion).

## Code Structure & Submission
The skeleton code includes several files, but you are only required to implement and submit the following four files to the ZINC platform:

*   **`MapView.tpp`**: Implementation of template functions for the `MapView` class, specifically custom iterator operators.
*   **`BaseAnalyzer.cpp`**: Implementation of non-template static functions (`range_sum` and `max_item`) to analyze items in a base.
*   **`World.tpp`**: Implementation of template functions for the `World` simulator (e.g., `create_payload`).
*   **`World.cpp`**: Implementation of non-template functions for the `World` simulator (e.g., `remove_payload`, `process_arrived_payloads`, `elapse`, and the destructor).

*Note: You are NOT allowed to include additional libraries, define any static/global variables, or use the `auto` or `decltype` keywords.*

## Task Breakdown

### Part 1: Iterators (`MapView.tpp`)
Implement the `operator*` (reference) and `operator->` (pointer) for `MapView`'s custom iterator. `MapView` acts as a wrapper for `map<string, T*>`, and your iterators should dereference to `const T &` and `const T *` respectively, bypassing the map's string key.

### Part 2: Algorithms (`BaseAnalyzer.cpp`)
*   **`range_sum`**: Iterate through a base's items and sum the quantities of items whose names fall inclusively between a given lower and upper bound.
*   **`max_item`**: Identify the most frequent item in a base. In the event of a tie, select the lexicographically smallest item name.

### Part 3: Containers (`World.cpp`, `World.tpp`)
Implement the core simulator functions using a `std::map` for active payloads and a `std::priority_queue` for arrival times.
*   **`create_payload`**: Create and dispatch a new shipment between bases.
*   **`remove_payload`**: Perform lazy deletion on a payload by marking it as deleted and removing it from the active map.
*   **`process_arrived_payloads`**: Deliver non-deleted payloads whose arrival time is at most the current time.
*   **`elapse`**: Advance the simulation time and process any payloads that arrive during this period.
*   **`~World`**: Clean up and deallocate all dynamically allocated bases and payloads.

## Compilation and Execution

To compile the code, simply run the provided Makefile:
```bash
make
```

To run the program and test cases on Linux/Mac:
```bash
# Run a specific test case (e.g., test case 1)
./lab8 1

# Run all test cases
./lab8
```

To run the program on Windows:
```cmd
# Run a specific test case (e.g., test case 1)
\lab8.exe 1

# Run all test cases
\lab8.exe
```

## Grading & Debugging
*   Expected outputs are located in the `test/out/` directory.
*   A successful run will print "Beginning of test X" and "End of test X". Assertion failures indicate logical errors.
*   The autograder (ZINC) will run the tests twice: once normally and once with memory leak checking enabled (`-fsanitize=address,leak,undefined`). Ensure all dynamically allocated memory is properly freed in your destructors.