#include <iostream>
#include <fstream>
#include "../split.hpp"
#include <tl/getlines.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <numeric>

using cave_system_t = std::unordered_multimap<std::string, std::string>;

bool is_small_cave(std::string const& cave) {
	return std::ranges::all_of(cave, [](auto c) { return std::islower(c); });
}

std::size_t count_paths(cave_system_t const& cave_system, std::string const& cave,
		std::unordered_set<std::string> visited_small_caves = {}, 
	    bool visited_small_cave_twice = false) {
	if (cave == "end") return 1;

	if (is_small_cave(cave) && visited_small_caves.contains(cave)) {
		if (cave == "start" || visited_small_cave_twice) return 0;

		visited_small_cave_twice = true;
	}

	if (is_small_cave(cave)) {
		visited_small_caves.insert(cave);
	}

	auto [begin, end] = cave_system.equal_range(cave);
	return std::accumulate(begin, end, std::size_t(0), [&](auto acc, auto&& pair) {
		return acc + count_paths(cave_system, pair.second, visited_small_caves, visited_small_cave_twice);
		});
}

void part1(std::istream& input) {
	cave_system_t cave_system;

	for (auto line : tl::views::getlines(input)) {
		auto s = split<std::string>(line, '-');
		cave_system.insert({ s[0],s[1] });
		cave_system.insert({ s[1],s[0] });
	}

	std::cout << count_paths(cave_system, "start");
}

void part2(std::istream& input) {
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day12/input.txt" };
	part1(input);
}