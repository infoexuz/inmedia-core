# Build:  make        Run:  ./inmedia-core        Tests:  make test
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Isrc
MODULES = src/validation.cpp src/upload.cpp src/auth.cpp src/catalog.cpp src/catalog_search.cpp
TESTS = tests/run_tests.cpp tests/test_validation.cpp tests/test_upload.cpp tests/test_auth.cpp \
        tests/test_catalog.cpp tests/test_search.cpp

inmedia-core: src/main.cpp $(MODULES) src/*.h
	$(CXX) $(CXXFLAGS) src/main.cpp $(MODULES) -o inmedia-core

run_tests: $(TESTS) $(MODULES) src/*.h tests/check.h
	$(CXX) $(CXXFLAGS) $(TESTS) $(MODULES) -o run_tests

test: run_tests
	./run_tests

clean:
	rm -f inmedia-core run_tests

.PHONY: test clean
