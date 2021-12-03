#include <iostream>
#include <fstream>
#include <tl/getlines.hpp>
#include <tl/to.hpp>
#include <tl/enumerate.hpp>
#include <vector>
#include <numeric>
#include "../fold.hpp"

void part1(std::istream& input) {
	auto lines = tl::views::getlines(input) | tl::to<std::vector>();
	auto n_positions = lines[0].size();
	auto counts = std::vector<std::uint64_t>(n_positions);

	for (auto&& line : lines) {
		for (auto [i, c] : line | tl::views::enumerate) {
			if (c == '1') ++counts[i];
		}
	}

	auto new_number = counts
		| std::views::transform([&](auto count) { return count > (lines.size() / 2) ? 1 : 0; });


	auto gamma = tl::fold_left(new_number.begin(), new_number.end(), 0,
		[](std::uint64_t acc, std::uint64_t i) { return (acc << 1) | i; });
	auto epsilon = tl::fold_left(new_number.begin(), new_number.end(), 0,
		[](std::uint64_t acc, std::uint64_t i) { return (acc << 1) | (i ^ 1); });

	std::cout << gamma * epsilon;
}

auto calculate_score(auto lines, auto criteria) {
	auto position = 0;
	while (lines.size() > 1) {
		auto filtered = lines | std::views::filter([&](auto line) { return line[position] == '1'; });
		auto count = std::ranges::distance(filtered);
		auto most_common = criteria(count >= (lines.size() - count));
		std::erase_if(lines, [&](auto line) { return line[position] != most_common; });
		++position;
	}
	return std::stoi(lines[0], nullptr, 2);
}
void part2(std::istream& input) {
	auto lines = tl::views::getlines(input) | tl::to<std::vector>();
	auto n_positions = lines[0].size();


	auto oxygen_score = calculate_score(lines, [](bool one_is_most_common) {
		return one_is_most_common ? '1' : '0';
		});
	auto co2_score = calculate_score(lines, [](bool one_is_most_common) {
		return one_is_most_common ? '0' : '1';
		});
	std::cout << oxygen_score * co2_score;
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day03/input.txt" };
	part2(input);
}