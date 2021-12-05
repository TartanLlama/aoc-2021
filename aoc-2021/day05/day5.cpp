#include <ctre.hpp>
#include <set>
#include <tl/getlines.hpp>
#include <tl/to.hpp>
#include <iostream>
#include <fstream>
#include <vector>
struct point {
	int x;
	int y;
	friend auto operator<=>(const point&, const point&) = default;
};

struct line {
	point start;
	point end;
};

struct results {
	std::set<point> seen;
	std::set<point> multiple;

	void insert(point p) {
		if (seen.contains(p)) multiple.insert(p);
		seen.insert(p);
	}
};

auto parse_lines(std::istream& input) {
	auto lines = tl::views::getlines(input) |
		std::views::transform([](auto l) {
		auto [match, x0, y0, x1, y1] = ctre::match<"([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)">(l);
		return line{ point{std::stoi(x0.str()), std::stoi(y0.str())},
					 point{std::stoi(x1.str()), std::stoi(y1.str())} };
			})
		| tl::to<std::vector>();

			return lines;
}


void count_intersections(std::istream& input, bool count_diagonals) {
	auto lines = parse_lines(input);

	results res;
	for (auto l : lines) {
		//horizontal
		if (l.start.x == l.end.x) {
			for (auto y : std::views::iota(std::min(l.start.y, l.end.y), std::max(l.start.y, l.end.y) + 1)) {
				res.insert(point{ l.start.x, y });
			}
		}
		//vertical
		else if (l.start.y == l.end.y) {
			for (auto x : std::views::iota(std::min(l.start.x, l.end.x), std::max(l.start.x, l.end.x) + 1)) {
				res.insert(point{ x, l.start.y });
			}
		}
		//diagonal
		else if (count_diagonals) {
			auto x_direction = l.start.x < l.end.x ? 1 : -1;
			auto y_direction = l.start.y < l.end.y ? 1 : -1;

			for (auto distance : std::views::iota(0, std::abs(l.end.x - l.start.x) + 1)) {
				res.insert(point{ l.start.x + distance * x_direction,
								   l.start.y + distance * y_direction });
			}
		}

	}

	std::cout << res.multiple.size();
}

void part1(std::istream& input) {
	count_intersections(input, false);
}
void part2(std::istream& input) {
	count_intersections(input, true);
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day05/input.txt" };
	part2(input);
}