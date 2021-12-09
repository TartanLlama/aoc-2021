#include <fstream>
#include <iostream>
#include <tl/getlines.hpp>
#include <tl/cartesian_product.hpp>
#include "../fold.hpp"
#include <tl/to.hpp>
#include <ranges>
#include <vector>

struct location {
	short height;
	bool filled;
};

using map_t = std::vector<std::vector<location>>;
auto parse_map(std::istream& input) {
	std::vector<std::vector<location>> map;
	for (auto line : tl::views::getlines(input)) {
		std::vector<location> l{ {9,false} };
		for (auto c : line) l.push_back({ c - '0', false });
		l.push_back({ 9,false });
		map.push_back(std::move(l));
	}
	std::vector<location> l(map[0].size(), { 9,false });
	map.insert(map.begin(), l);
	map.push_back(std::move(l));

	return map;
}

void part1(std::istream& input) {
	auto map = parse_map(input);

	auto low_point_risks = tl::views::cartesian_product(
			std::views::iota(1, std::ssize(map) - 1),
			std::views::iota(1, std::ssize(map[0]) - 1))
		| std::views::filter([&](auto c) {
			auto [x, y] = c;
			return map[x - 1][y].height > map[x][y].height &&
				map[x + 1][y].height > map[x][y].height &&
				map[x][y + 1].height > map[x][y].height &&
				map[x][y - 1].height > map[x][y].height;
			})
		| std::views::transform([&](auto c) { return map[c.first][c.second].height + 1; });


		std::cout << tl::fold(low_point_risks, 0, std::plus<>());

}

auto flood_fill(map_t& map, std::ptrdiff_t x, std::ptrdiff_t y) {
	if (map[x][y].filled || map[x][y].height == 9) return 0;

	map[x][y].filled = true;
	auto n_filled = 1;

    n_filled += flood_fill(map, x - 1, y);
	n_filled += flood_fill(map, x + 1, y);
	n_filled += flood_fill(map, x, y + 1);
	n_filled += flood_fill(map, x, y - 1);

	return n_filled;
}

void part2(std::istream& input) {
	auto map = parse_map(input);

	auto low_points = tl::views::cartesian_product(
		std::views::iota(1, std::ssize(map) - 1),
		std::views::iota(1, std::ssize(map[0]) - 1))
		| std::views::filter([&](auto c) {
		auto [x, y] = c;
		return map[x - 1][y].height > map[x][y].height &&
			map[x + 1][y].height > map[x][y].height &&
			map[x][y + 1].height > map[x][y].height &&
			map[x][y - 1].height > map[x][y].height;
			});

	auto basin_sizes = low_points
		| std::views::transform([&](auto c) { return flood_fill(map, c.first, c.second); })
		| tl::to<std::vector>();
	std::ranges::partial_sort(basin_sizes, basin_sizes.begin() + 3, std::ranges::greater());
	std::cout << tl::fold(basin_sizes | std::views::take(3), 1, std::multiplies<>());
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day09/input.txt" };
	part2(input);
}