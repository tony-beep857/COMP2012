#include "testcases.h"
using namespace std;


// test case teseter heavily inspired by 2025 Fall PA3
// changin to hidden test case should be as simple as swapping out testcases.cpp
function<void(void)> test_functions[NUM_TESTCASES];

template<int N>
void init_testcase() {
    init_testcase<N-1>();
    test_functions[N-1] = test<N>;
}

template<>
void init_testcase<0>() {} // does nothing

void run(int i) {
    if (i <= 0 || i > NUM_TESTCASES) {
        cout << "Test case " << i << " not found, did you forget to change NUM_TESTCASES?\n";
        return;
    }
    cout << "Beginning of test " << i << "\n";
    test_functions[i-1]();
    cout << "End of test " << i << "\n";
}


int main(int argc, char** argv) {
    init_testcase<NUM_TESTCASES>();
    if (argc == 1) {// only argument is the exe file
        for (int i = 1; i <= NUM_TESTCASES; i++) {
            run(i);
        }
    } else {
        run(stoi(string(argv[1])));
    }
}