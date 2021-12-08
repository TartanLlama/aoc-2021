#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include "../split.hpp"

void part1(std::istream& input) {
	auto crabs = split<int>(input, ',');
	auto median_point = std::ranges::begin(crabs) + std::ranges::size(crabs) / 2;
	std::ranges::nth_element(crabs, median_point);
	std::cout << std::transform_reduce(crabs.begin(), crabs.end(), 0, std::plus<>(),
		[&](auto c) { return std::abs(c - *median_point); });
}

auto cost(int crab, int destination) {
	auto n = std::abs(crab - destination);
	return n * (n + 1) / 2;
}

void part2(std::istream& input) {
	auto crabs = split<int>(input, ',');
	auto [min, max] = std::ranges::minmax_element(crabs);
	
	auto costed_positions = std::views::iota(*min, *max + 1)
		| std::views::transform([&](auto position) {
		return std::transform_reduce(crabs.begin(), crabs.end(), 0, std::plus<>(),
			[&](auto c) { return cost(c, position); });
			});

	std::cout << *std::ranges::min_element(costed_positions);
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day07/input.txt" };
	part2(input);
}