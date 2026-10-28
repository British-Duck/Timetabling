#pragma once
#include <iostream>
#include <limits>

// Reads a numbered menu choice from std::cin.
// Non-numeric input is discarded and 0 is returned (treated as "Invalid choice").
// At end of input `backChoice` is returned so every menu unwinds and the program exits.
inline int readMenuChoice(int backChoice) {
    int choice;
    if (std::cin >> choice) return choice;
    if (std::cin.eof()) return backChoice;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 0;
}
