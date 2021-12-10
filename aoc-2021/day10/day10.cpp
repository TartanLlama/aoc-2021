#include <iostream>
#include <fstream>
#include <tl/getlines.hpp>
#include <stack>
#include <numeric>
#include "../fold.hpp"
#include <vector>
#include <tl/to.hpp>
#include <ranges>

auto score_paren(char c) {
	switch (c) {
	case '(': return 1;
	case '[': return 2;
	case '{': return 3;
	case '<': return 4;
	}
}

std::pair<int, unsigned long long> score_line(std::string const& line) {
	std::stack<char> stack;
	for (auto c : line) {
		switch (c) {
		case '(':
		case '[':
		case '<':
		case '{':
			stack.push(c);
			break;
		case ')':
			if (stack.top() != '(') {
				return { 3,0 };
			}
			stack.pop(); break;
		case ']':
			if (stack.top() != '[') {
				return { 57,0 };
			}
			stack.pop(); break;
		case '}':
			if (stack.top() != '{') {
				return { 1197, 0 };
			}
			stack.pop(); break;
		case '>':
			if (stack.top() != '<') {
				return { 25137, 0 };
			}
			stack.pop(); break;
		}
	}

	unsigned long long score = 0;
	while (!stack.empty()) {
		score = score * 5 + score_paren(stack.top());
		stack.pop();
	}
	return { 0, score };
}

void part1(std::istream& input) {
	auto scores = tl::views::getlines(input)
		| std::views::transform(score_line);
	std::cout << tl::fold(scores, 0, [](auto acc, auto p) { return acc + p.first; });
}

void part2(std::istream& input) {
	auto scores = tl::views::getlines(input)
		| std::views::transform(score_line)
		| std::views::filter([](auto p) { return p.second > 0; })
		| std::views::transform([](auto p) { return p.second; })
		| tl::to<std::vector>();

	auto middle_point = scores.begin() + scores.size() / 2;
	std::ranges::nth_element(scores, middle_point);
	std::cout << *middle_point;
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day10/input.txt" };
	part2(input);
}