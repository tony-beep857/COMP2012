PA1 student package

Contents
- Six .cpp skeleton files and six headers: complete the TODOs.
- main.cpp: the 44 given public test cases.
- Makefile: builds shopping_system (shopping_system.exe on Windows).
- tests/input_01.txt through input_44.txt: one test number per file.
- tests/output_01.txt through output_44.txt: expected standard output.

After implementing the required methods, open a terminal in pa1_student and run:
  make

Linux / macOS:
  ./shopping_system < tests/input_01.txt > actual_01.txt
  diff -u tests/output_01.txt actual_01.txt

Windows PowerShell:
  Get-Content tests/input_01.txt | .\shopping_system.exe
Compare the printed output with tests/output_01.txt.

Each run reads one integer from 1 to 44 and executes that test in main.cpp.
No product or customer data needs to be entered. Change 01 to 02, ..., 44
for the other cases. Match the expected text, spaces, and line breaks; do
not print extra prompts or debugging messages. A test must also complete
without assertion failures. These commands do not enable memory checking;
see the assignment page and Makefile for sanitizer instructions.
The skeleton is incomplete: it is not expected to pass before you implement it.
