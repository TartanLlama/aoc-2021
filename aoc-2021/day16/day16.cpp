#include <iostream>
#include <fstream>
#include <ranges>
#include <vector>
#include <tl/to.hpp>
#include <variant>
#include <numeric>
#include <string>
#include <memory>
#include <functional>
#include "../fold.hpp"

struct packet {
	int version;
	virtual ~packet() = default;
};

struct literal : packet {
	unsigned long long value = {};
};

struct op : packet {
	int id;
	std::vector<std::unique_ptr<packet>> subpackets;
};


auto parse_input(std::istream& input) {
	auto data = std::ranges::istream_view<char>(input)
		| std::views::transform([](auto c) { return (std::byte)std::stoi(std::string{ c }, nullptr, 16); })
		| tl::to<std::vector>();
	data.push_back(std::byte{ 0 });
	return data;
}

enum packet_id {
	literal_packet = 4,
};

template <class T>
auto read_data(auto it, std::size_t offset) {
	T t{};
	for (auto i = 0; i < sizeof(T)*2; ++i) {
		t <<= 4;
		t |= static_cast<unsigned char>(*it++);
	}
	return t << offset;
}

auto increase_offset(auto& it, std::size_t& offset, std::size_t amount) {
	offset += amount;
	while (offset >= 4) {
		++it;
		offset -= 4;
	}
}

std::unique_ptr<packet> parse_packet(auto& it, std::size_t& offset) {
	auto header = read_data<std::uint16_t>(it, offset);
	auto version = (header & 0b1110000000000000) >> 13;
	auto id = (header & 0b0001110000000000) >> 10;

	if (id == literal_packet) {
		literal pack;
		pack.version = version;

		increase_offset(it, offset, 6);

		auto data = read_data<std::uint8_t>(it, offset);

		while (data & 0b10000000) {
			pack.value <<= 4;
			pack.value |= (data & 0b01111000) >> 3;
			increase_offset(it, offset, 5);
			data = read_data<std::uint8_t>(it, offset);
		}
		pack.value <<= 4;
		pack.value |= (data & 0b01111000) >> 3;
		increase_offset(it, offset, 5);
		
		return std::make_unique<literal>(pack);
	}
	else {
		op pack;
		pack.version = version;
		pack.id = id;
		auto length_id = header & 0b0000001000000000;

		//total length in bits
		if (length_id == 0) {
			auto data = read_data<std::uint32_t>(it, offset);
			
			std::uint32_t length_in_bits = (0b00000001111111111111110000000000 & data) >> 10;
			
			it += 4;
			increase_offset(it, offset, 6);

			auto start_pos = it;
			auto start_offset = offset;
			while (((it - start_pos) * 4 + (offset - start_offset)) < length_in_bits) {
				pack.subpackets.push_back(parse_packet(it, offset));
			}			
		}
		//length in packets
		else {
			auto data = read_data<std::uint32_t>(it, offset);
			
			auto length_in_packets = (0b00000001111111111100000000000000 & data) >> 14;
			
			it += 4;
			increase_offset(it, offset, 2);

			for (auto i = 0; i < length_in_packets; ++i) {
				pack.subpackets.push_back(parse_packet(it, offset));
			}
		}

		return std::make_unique<op>(std::move(pack));
	}
}

std::size_t sum_versions(packet const* p) {
	if (auto l = dynamic_cast<literal const*>(p)) {
		return l->version;
	}
	else {
		auto o = dynamic_cast<op const*>(p);
		return o->version + std::accumulate(o->subpackets.begin(), o->subpackets.end(), 0,
			[](auto acc, auto&& p) { return acc + sum_versions(p.get()); });
	}
}

void part1(std::istream& input) {
	auto packets = parse_input(input);

	auto it = packets.begin();
	std::size_t offset = 0;
	auto packet = parse_packet(it, offset);

	std::cout << sum_versions(packet.get());
}

std::function<std::uint64_t(std::uint64_t, std::uint64_t)> get_op(int id) {
	switch (id) {
	case 0: return std::plus<std::uint64_t>{};
	case 1: return std::multiplies<std::uint64_t>{};
	case 2: return [](auto lhs, auto rhs) { return std::min(lhs, rhs); };
	case 3: return [](auto lhs, auto rhs) { return std::max(lhs, rhs); };
	case 5: return std::greater<std::uint64_t>{};
	case 6: return std::less<std::uint64_t>{};
	case 7: return std::equal_to<std::uint64_t>{};
	 }
}

std::uint64_t eval(packet const* p) {
	if (auto l = dynamic_cast<literal const*>(p)) {
		return l->value;
	}
	else {
		auto o = dynamic_cast<op const*>(p);
		auto op = [&](std::uint64_t acc, std::unique_ptr<packet> const& p) { return get_op(o->id)(acc, eval(p.get())); };
		return std::accumulate(o->subpackets.begin()+1, o->subpackets.end(), eval(o->subpackets.front().get()), op);
	}
}


void part2(std::istream& input) {
	auto packets = parse_input(input);

	auto it = packets.begin();
	std::size_t offset = 0;
	auto packet = parse_packet(it, offset);

	std::cout << eval(packet.get());
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day16/input.txt" };
	part2(input);
}