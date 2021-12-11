#include <fstream>
#include <iostream>
#include <tl/getlines.hpp>
#include <tl/cartesian_product.hpp>
#include "../fold.hpp"
#include <tl/to.hpp>
#include <ranges>
#include <tl/enumerate.hpp>
#include <vector>
#include <queue>

using map_t = std::vector<std::vector<short>>;
auto parse_map(std::istream& input) {
	std::vector<std::vector<short>> map;
	for (auto line : tl::views::getlines(input)) {
		std::vector<short> l{ -2 };
		for (auto c : line) l.push_back(c - '0');
		l.push_back(-2);
		map.push_back(std::move(l));
	}
	std::vector<short> l(map[0].size(), -2);
	map.insert(map.begin(), l);
	map.push_back(std::move(l));

	return map;
}

void part1(std::istream& input) {
	auto map = parse_map(input);

	int flashes = 0;
	for (auto step : std::views::iota(0, 100)) {
		std::queue<std::pair<std::size_t, std::size_t>> to_flash;
		for (auto i : std::views::iota(1, 11)) {
			for (auto j : std::views::iota(1, 11)) {
				++map[i][j];

				if (map[i][j] > 9) {
					to_flash.push({ i,j });
				}
			}
		}

		while (!to_flash.empty()) {
			auto [x, y] = to_flash.front();
			if (map[x][y] == -1) {
				to_flash.pop();
				continue;
			}
			++flashes;
			map[x][y] = -1;
			to_flash.pop();

			for (auto i : std::views::iota(-1, 2)) {
				for (auto j : std::views::iota(-1, 2)) {
					if (map[x + i][y + j] >= 0 && !(i == 0 && j == 0)) {
						++map[x + i][y + j];
						if (map[x + i][y + j] > 9) {
							to_flash.push({ x+i,y+j });
						}
					}
				}
			}
		}

		for (auto& line : map) {
			for (auto& octopus : line) {
				if (octopus == -1) octopus = 0;
			}
		}
	}

	std::cout << flashes;
}

auto is_synchronised(map_t& map) {
	for (auto i : std::views::iota(1, 11)) {
		for (auto j : std::views::iota(1, 11)) {
			if (map[i][j] != -1) return false;
		}
	}

	return true;
}

auto calculate_first_synchronisation_point(map_t& map) {
	int flashes = 0;
	int step = 0;
	while (true) {
		std::queue<std::pair<std::size_t, std::size_t>> to_flash;
		for (auto i : std::views::iota(1, 11)) {
			for (auto j : std::views::iota(1, 11)) {
				++map[i][j];

				if (map[i][j] > 9) {
					to_flash.push({ i,j });
				}
			}
		}

		while (!to_flash.empty()) {
			auto [x, y] = to_flash.front();
			if (map[x][y] == -1) {
				to_flash.pop();
				continue;
			}
			++flashes;
			map[x][y] = -1;
			to_flash.pop();

			for (auto i : std::views::iota(-1, 2)) {
				for (auto j : std::views::iota(-1, 2)) {
					if (map[x + i][y + j] >= 0 && !(i == 0 && j == 0)) {
						++map[x + i][y + j];
						if (map[x + i][y + j] > 9) {
							to_flash.push({ x + i,y + j });
						}
					}
				}
			}
		}

		if (is_synchronised(map)) return step;

		for (auto& line : map) {
			for (auto& octopus : line) {
				if (octopus == -1) octopus = 0;
			}
		}

		++step;
	}
}

void part2(std::istream& input) {
	auto map = parse_map(input);
	std::cout << calculate_first_synchronisation_point(map) + 1;
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day11/input.txt" };
	part2(input);
}