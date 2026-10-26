#include "testcases.h"

void (*tests[NUM_TESTCASES])() {
    test1, test2, test3, test4
};

void run(int num) {
    if (num <= 0 || num > NUM_TESTCASES) {
        cout << "Test " << num << " does not exist, did you forget to change NUM_TESTCASES?\n";
        return;
    }

    tests[num-1]();

}

int main(int argc, char **argv) {
    if (argc == 1) {
        for (int i = 1; i <= NUM_TESTCASES; i++)
            run(i);
    } else {
        run(stoi(string(argv[1])));
    }
}