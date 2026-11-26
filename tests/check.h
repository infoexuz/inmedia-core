// check.h - the tiny test helper every test file uses.
// Written with help from Claude (AI assistant); reviewed by the team.
#ifndef INMEDIA_CHECK_H
#define INMEDIA_CHECK_H

#include <iostream>
#include <string>

extern int passed;
extern int failed;

inline void check(bool condition, const std::string& name) {
    if (condition) {
        ++passed;
    } else {
        ++failed;
        std::cout << "FAILED: " << name << '\n';
    }
}

#endif
