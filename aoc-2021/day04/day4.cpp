#include <iostream>
#include <fstream>
#include <tl/getlines.hpp>
#include <tl/to.hpp>
#include <ranges>
#include <array>
#include <numeric>
#include <sstream>

struct board {
	std::array<std::array<std::pair<short, bool>, 5>, 10> rows;

	bool finished() {
		for (auto row : rows) {
			auto count = std::ranges::count_if(row, [](auto p) { return p.second == true; });
			if (count == 5) return true;
		}
		return false;
	}

	void call_number(short n) {
		for (auto& row : rows) {
			for (auto& [num, called] : row) {
				if (num == n) called = true;
			}
		}
	}
	int score() {
		auto sum = 0;
		for (auto row : rows | std::views::take(5)) {
			for (auto [num, called] : row) {
				if (!called) {
					sum += num;
				}
			}
		}
		return sum;
	}
};

auto parse_numbers_called(auto lines_it) {
	auto num_called_string = (*lines_it++);
	auto numbers_called = num_called_string
		| std::views::split(',')
		| std::views::transform([](auto s) { return std::stoi(std::string{ s.begin(), s.end() }); })
		| tl::to<std::vector>();
	++lines_it;

	return numbers_called;
}

auto parse_boards(auto& lines, auto lines_it) {
	std::vector<board> boards;
	while (lines_it != std::ranges::end(lines)) {
		board b;
		for (auto i : std::views::iota(0, 5)) {
			std::stringstream line;
			line << *lines_it++;
			auto v =
				std::ranges::istream_view<short>(line)
				| std::views::transform([](auto j) { return std::pair{ j,false }; })
				| tl::to < std::vector < std::pair<short, bool> >>();
			for (auto j : std::views::iota(0, 5)) {
				b.rows[i][j] = v[j];
			}
		}

		for (auto i : std::views::iota(0, 5)) {
			for (auto j : std::views::iota(0, 5)) {
				b.rows[i + 5][j] = b.rows[j][i];
			}
		}
		boards.push_back(b);

		if (lines_it != std::ranges::end(lines))
			++lines_it;
	}

	return boards;
}

void part1(std::istream& input) {
	auto lines = tl::views::getlines(input);
	auto lines_it = std::ranges::begin(lines);
	auto numbers_called = parse_numbers_called(lines_it);
	auto boards = parse_boards(lines, lines_it);

	auto i = 0;
	while (!std::ranges::any_of(boards, &board::finished)) {
		for (auto& board : boards) {
			board.call_number(numbers_called[i]);
		}
		++i;
	}

	auto winning_board = std::ranges::find_if(boards, &board::finished);
	std::cout << winning_board->score() * numbers_called[i-1];
}

void part2(std::istream& input) {
	auto lines = tl::views::getlines(input);
	auto lines_it = std::ranges::begin(lines);
	auto numbers_called = parse_numbers_called(lines_it);
	auto boards = parse_boards(lines, lines_it);

	auto i = 0;
	auto not_finished = [](auto& b) { return !b.finished(); };
	while (std::ranges::count_if(boards, not_finished) != 1) {
		for (auto& board : boards) {
			board.call_number(numbers_called[i]);
		}
		++i;
	}

	auto losing_board = std::ranges::find_if(boards, not_finished);
	while (!losing_board->finished()) {
		losing_board->call_number(numbers_called[i++]);
	}
	std::cout << losing_board->score() * numbers_called[i-1];
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day04/input.txt" };
	part2(input);
}