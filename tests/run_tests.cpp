// run_tests.cpp - runs every test group and prints the result.
// Build and run:  make test
// Written with help from Claude (AI assistant); reviewed by the team.
#include <iostream>

int passed = 0;
int failed = 0;

void testValidation();
void testUpload();
void testAuth();
void testCatalog();
void testSearch();

int main() {
    testValidation();
    testUpload();
    testAuth();
    testCatalog();
    testSearch();
    std::cout << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;
}
