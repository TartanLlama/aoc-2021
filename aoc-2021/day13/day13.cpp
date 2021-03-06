#include <iostream>
#include <fstream>
#include <tl/getlines.hpp>
#include <tl/to.hpp>
#include <vector>
#include <ranges>
#include <set>
#include <ctre.hpp>
#include "../split.hpp"

struct point {
	int x;
	int y;
	friend auto operator<=>(point a, point b) = default;
};
void part1(std::istream& input) {
	auto lines = tl::views::getlines(input) | tl::to<std::vector>();
	auto empty_it = std::ranges::find(lines, "");

	std::set<point> graph;

	for (auto line : std::ranges::subrange{ lines.begin(), empty_it }) {
		auto s = split<int>(line, ',');
		graph.insert({ s[0], s[1] });
	}

	for (auto fold : std::ranges::subrange(std::ranges::next(empty_it), std::ranges::end(lines))) {
		auto [match, axis_match, coord_match] = ctre::match<"fold along (.)=(.+)">(fold);
		auto coord = std::stoi(coord_match.str());
		auto axis = axis_match.str();

		decltype(graph) new_graph;
		for (auto p : graph) {
			if (axis == "x") {
				if (p.x > coord) {
					new_graph.insert({ coord - (p.x - coord), p.y });
				}
				else {
					new_graph.insert(p);
				}
			}
			if (axis == "y") {
				if (p.y > coord) {
					new_graph.insert({ p.x, coord - (p.y - coord) });
				}
				else {
					new_graph.insert(p);
				}
			}
		}
		graph = new_graph;
	}

	auto max_x = std::ranges::max_element(graph, std::ranges::less{}, &point::x)->x;
	auto max_y = std::ranges::max_element(graph, std::ranges::less{}, &point::y)->y;

	for (int y = 0; y <= max_y; ++y) {
		for (int x = 0; x <= max_x; ++x) {
			std::cout << (graph.contains({x,y}) ? "#" : " ");
		}
		std::cout << '\n';
	}
}

void part2(std::istream& input) {
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day13/input.txt" };
	part1(input);
}