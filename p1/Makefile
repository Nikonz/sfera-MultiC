TEST_FILES = gtest/gtest-all.cc gtest/gtest-main.cc
SRC = allocator.cpp allocator_test.cpp
HDR = allocator.h


all: tests.done

allocator_test: $(SRC) $(HDR)
	g++ -O1 -g -std=c++11 -o allocator_test $(SRC) $(TEST_FILES) -lpthread

tests.done: allocator_test
	./allocator_test
	touch tests.done
