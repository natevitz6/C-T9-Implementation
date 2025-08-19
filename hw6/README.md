# T9 Predictive Text Implementation

## Overview
This project implements a simplified version of the T9 predictive text input system, commonly found on older mobile phones. The goal is to allow users to input words using numeric key sequences, which are then mapped to possible word suggestions based on a dictionary.

## Project Structure
- `t9_lib.c` / `t9_lib.h`: Core library for T9 functionality, including mapping digit sequences to words and managing dictionary lookups.
- `t9_demo.c`: Demo program showing how to use the T9 library interactively.
- `t9_tests.c`: Unit tests for validating the correctness of the T9 implementation.
- `dictionary.txt` / `small_dictionary.txt`: Word lists used for predictive text suggestions.
- `clint.py`: Python script for code linting or formatting (if applicable).
- `Makefile`: Build instructions for compiling the project.
- `safe_assert.h`: Utility for safer assertions in C tests.

## How It Works
1. **Dictionary Loading**: The program loads a dictionary of words into memory.
2. **Digit Mapping**: Each digit (2-9) maps to a set of letters (e.g., 2 → a, b, c).
3. **Word Prediction**: When a user enters a sequence of digits, the program finds all words in the dictionary that match the possible letter combinations.
4. **Demo & Testing**: The demo program allows interactive testing, while the test suite ensures correctness.

## Getting Started
1. **Build the Project**: Use the provided `Makefile` to compile the demo and test programs.
2. **Run the Demo**: Execute the demo to try out T9 predictions interactively.
3. **Run Tests**: Use the test program to verify the implementation.

## Usage Example
- Enter a digit sequence (e.g., `43556`) and receive word suggestions (e.g., `hello`).

## Notes
- The implementation is for educational purposes and may not include all features of commercial T9 systems.
- The dictionary can be swapped for different word lists to test prediction quality.

## Author
Nathan Vitzthum

## License
This project is for academic use in CSE 374 and does not specify a license.
