#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <tl/to.hpp>
#include <tl/getlines.hpp>
#include "../split.hpp"
#include <string>
#include <numeric>
#include <array>
#include <string_view>
#include "../fold.hpp"

auto to_bitset(std::string_view s) {
	std::bitset<7> bits;
	for (auto c : s) {
		bits[c - 'a'] = true;
	}
	return bits;
}

auto contains(std::bitset<7> a, std::bitset<7> b) {
	return (a & b) == b;
}

struct display {
	using digit = std::bitset<7>;
	std::vector<digit> representations;
	std::vector<digit> output;
	std::array<digit, 10> mapping;

	void decode() {
		std::vector<digit> remainder;
		for (auto& d : representations) {
			switch (d.count()) {
			case 2: mapping[1] = d; break;
			case 3: mapping[7] = d; break;
			case 4: mapping[4] = d; break;
			case 7: mapping[8] = d; break;
			default: remainder.push_back(d); break;
			}
		}

		representations = std::move(remainder);

		for (auto& d : representations) {
			if (d.count() == 6 && contains(d, mapping[4])) {
				mapping[9] = d;
			}
			else if (d.count() == 5 && contains(d, mapping[1])) {
				mapping[3] = d;
			}
			else {
				remainder.push_back(d);
			}
		}

		representations = std::move(remainder);

		for (auto& d : representations) {
			if (d.count() == 6 && contains(d, mapping[1])) {
				mapping[0] = d;
			}
			else {
				remainder.push_back(d);
			}
		}

		representations = std::move(remainder);

		for (auto& d : representations) {
			if (d.count() == 6) {
				mapping[6] = d;
			}
			else {
				remainder.push_back(d);
			}
		}

		representations = std::move(remainder);


		for (auto& d : representations) {
			if (d.count() == 5 && contains(mapping[6], d)) {
				mapping[5] = d;
			}
			else {
				mapping[2] = d;
			}
		}
	}

	auto decode_output(digit d) {
		return std::ranges::find(mapping, d) - mapping.begin();
	}

	auto get_output() {
		auto decoded_digits = output | std::views::transform([this](auto d) { return decode_output(d); });
		return tl::fold(decoded_digits, 0, [](auto acc, auto n) {
			return acc * 10 + n;
			});
	}
};

auto parse_display(std::string const& line) {
	auto inputs = split<std::string>(line, ' ');
	auto representations = inputs
		| std::views::take(10)
		| std::views::transform(to_bitset)
		| tl::to<std::vector>();
	auto outputs = inputs
		| std::views::drop(11)
		| std::views::take(4)
		| std::views::transform(to_bitset)
		| tl::to<std::vector>();
	return display{ std::move(representations), std::move(outputs) };
}

auto parse_displays(std::istream& input) {
	return tl::views::getlines(input)
		| std::views::transform(parse_display)
		| tl::to<std::vector>();
}

void part1(std::istream& input) {
	auto displays = parse_displays(input);
	std::cout << std::accumulate(displays.begin(), displays.end(), 0,
		[](auto sum, auto& display) {
			return sum + std::ranges::count_if(display.output, [](auto bits) {
				return bits.count() == 2 ||
					bits.count() == 3 ||
					bits.count() == 4 ||
					bits.count() == 7;
				});
		});
}

void part2(std::istream& input) {
	auto displays = parse_displays(input);
	for (auto& display : displays) {
		display.decode();
	}
	std::cout << tl::fold(displays, 0, [](auto acc, auto& display) { return acc + display.get_output(); });
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day08/input.txt" };
	part2(input);
}