#include <iostream>
#include <fstream>
#include "../split.hpp"
#include "../fold.hpp"
#include <array>
#include <algorithm>
#include <numeric>

auto parse_buckets(std::istream& input) {
	auto fish = split<int>(input, ',');

	std::array<std::uint64_t, 9> buckets = {0};
	for (auto f : fish) {
		buckets[f]++;
	}
	return buckets;
}

auto run_simulation(auto buckets, int days) {
	for (auto i = 0; i < days; ++i) {
		std::ranges::rotate(buckets, buckets.begin() + 1);
		buckets[6] += buckets[8];
	}

	return tl::fold(buckets, 0ull, std::plus());
}

void part1(std::istream& input) {
	auto buckets = parse_buckets(input);
	std::cout << run_simulation(buckets, 80);
}


void part2(std::istream& input) {
	auto buckets = parse_buckets(input);
	std::cout << run_simulation(buckets, 256);
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day06/input.txt" };
	part2(input);
}