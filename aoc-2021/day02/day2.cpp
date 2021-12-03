#include <fstream>
#include <ranges>
#include <iostream>
#include <string>
#include <tl/to.hpp>
#include <vector>
#include <ctre.hpp>
#include <tl/getlines.hpp>
void part1() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day02/input.txt" };

	auto x = 0;
	auto y = 0;

	for (auto line : tl::views::getlines(input)) {
		auto match = ctre::match<"(.+) (.+)">(line);

		auto command = match.get<1>();
		auto amount = std::stoi(match.get<2>().str());
		if (command == "forward") {
			x += amount;
		}
		else if (command == "down") {
			y += amount;
		}
		else if (command == "up") {
			y -= amount;
		}
	}

	std::cout << x * y;

}

void part2() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day02/input.txt" };

	auto x = 0;
	auto y = 0;
	auto aim = 0;

	for (auto line : tl::views::getlines(input)) {
		auto match = ctre::match<"(.+) (.+)">(line);

		auto command = match.get<1>();
		auto amount = std::stoi(match.get<2>().str());

		if (command == "forward") {
			x += amount;
			y += amount * aim;
		}
		else if (command == "down") {
			aim += amount;
		}
		else if (command == "up") {
			aim -= amount;
		}
	}

	std::cout << x * y;

}

int main() {
	part2();
}