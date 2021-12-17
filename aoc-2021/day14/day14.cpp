#include <iostream>
#include <fstream>
#include <tl/getlines.hpp>
#include <tl/adjacent.hpp>
#include <map>
#include <numeric>
void part1(std::istream& input) {
	auto lines = tl::views::getlines(input);
	auto it = lines.begin();

	auto polymer_str = *it;
	std::map<std::pair<char, char>, std::size_t> polymer;
	for (auto [a, b] : tl::views::pairwise(polymer_str)) {
		++polymer[{a, b}];
	}

	++it;
	++it;

	std::map<std::pair<char, char>, char> rules;
	for (auto line : std::ranges::subrange{ it, lines.end() }) {
		rules[{line[0], line[1]}] = line[6];
	}

	for (auto i = 0; i < 40; ++i) {
		std::map<std::pair<char, char>, std::size_t> new_polymer;

		for (auto [p, count] : polymer) {
			auto new_element = rules[p];
			new_polymer[{p.first, new_element}] += count;
			new_polymer[{new_element, p.second}] += count;
		}

		polymer = new_polymer;
		std::cout << std::accumulate(begin(polymer), end(polymer), 0, [](auto acc, auto p) { return acc + p.second; }) << '\n';
	}

	std::map<char, std::size_t> counts;
	for (auto [p, count] : polymer) {
		counts[p.first] += count;
	}
	counts[polymer_str.back()]++;

	auto [min,max] = std::ranges::minmax_element(counts, std::ranges::less(), [](auto p) { return p.second; });
	std::cout << max->second - min->second;
}
void part2(std::istream& input) {
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day14/input.txt" };
	part1(input);
}