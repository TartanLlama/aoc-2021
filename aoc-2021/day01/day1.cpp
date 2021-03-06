#include <fstream>
#include <ranges>
#include <array>
#include <tl/adjacent.hpp>
#include <tl/to.hpp>
#include <tl/utility/tuple_utils.hpp>
#include <iostream>

void part1() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day01/input.txt" };

	auto input_vec = std::ranges::istream_view<int>(input) 
		| tl::to<std::vector>();

	auto filtered_input = input_vec 
		| tl::views::adjacent<2>
		| std::views::filter([](auto p) { return p.second > p.first; });

	std::cout << std::ranges::distance(filtered_input);
}

void part2() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day01/input.txt" };

	auto input_vec = std::ranges::istream_view<int>(input)
		| tl::to<std::vector>();

	auto filtered_input = input_vec
		| tl::views::adjacent<3>
		| std::views::transform([](auto t) { return tl::tuple_fold(t, 0, std::plus{}); })
		| tl::views::adjacent<2>
		| std::views::filter([](auto p) { return p.second > p.first; });

	std::cout << std::ranges::distance(filtered_input);
}

int main() {
	part1();
	part2();
}