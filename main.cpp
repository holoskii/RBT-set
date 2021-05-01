//#include "set.h"

#include "set.h"

#include <chrono>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime> 

void testSet(const int batchSize, const int iterations) {
	srand(time(NULL));

	auto begin = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < iterations; i++) {
		Set<int> set;
		std::vector<int> v;

		for (int i = 0; i < batchSize; i++) {
			int value = rand() % batchSize;
			v.push_back(value);
			set.insert(value);
		}

		for (int i = 0; i < batchSize; i++) {
			set.erase(v.at(i));
		}
	}

	auto end = std::chrono::high_resolution_clock::now();
	double duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count();
	duration = duration / iterations / 1e6;

	char buffer[64];
	snprintf(buffer, 64, "Run time with %.0e batch size is %.3lfms", (double)batchSize, duration);

	std::cout << buffer << std::endl;
}



int main() {
	testSet(1e1, 10);
	testSet(1e2, 10);
	testSet(1e3, 10);
	testSet(1e4, 10);
	testSet(1e5, 10);
	testSet(1e6, 10);
	testSet(1e7, 10);
}

