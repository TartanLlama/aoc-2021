#include <vector>
#include <tl/to.hpp>
#include <tl/getlines.hpp>
#include <queue>
#include <map>
#include <numeric>
#include <array>
#include <iostream>
#include <fstream>

auto parse_map_p1(std::istream& input) {
	return tl::views::getlines(input)
		| std::views::transform([](auto&& line) {
		return line
			| std::views::transform([](auto c) {
			return static_cast<short>(c - '0');
				})
			| tl::to<std::vector>();
			})
		| tl::to<std::vector>();
}

auto parse_map_p2(std::istream& input) {
	auto map = parse_map_p1(input);
	auto orig_y = map.size();
	auto orig_x = map[0].size();

	map.resize(map.size() * 5);
	for (auto& line : map) line.resize(orig_x * 5);

	for (auto i = 0; i < 5; ++i) {
		for (auto j = 0; j < 5; ++j) {
			for (auto y = 0; y < orig_y; ++y) {
				for (auto x = 0; x < orig_x; ++x) {
					map[y + (j * orig_y)][x + (i * orig_x)] = ((map[y][x] + i + j - 1) % 9) + 1;
				}
			}
		}
	}

	return map;
}

struct point {
	int x;
	int y;
	friend auto operator<=>(point a, point b) = default;
};

void dijkstra(auto const& map) {
	std::vector<std::vector<std::size_t>> dist;
	std::vector<std::vector<point>> prev;
	std::vector<point> queue;
	std::array<point, 4> neighbours{ { {-1,0},{1,0},{0,-1},{0,1} } };
	auto target = point(map.size() - 1, map[0].size() - 1);
	auto undefined = point{ -1,-1 };
	auto priority = [&](auto lhs, auto rhs) {
		return dist[lhs.x][lhs.y] > dist[rhs.x][rhs.y];
	};

	dist.resize(map.size());
	prev.resize(map.size());
	for (auto i = 0; i < map.size(); ++i) {
		dist[i].resize(map[0].size());
		prev[i].resize(map[0].size());
		for (auto j = 0; j < map[0].size(); ++j) {
			dist[i][j] = std::numeric_limits<std::size_t>::max();
			prev[i][j] = undefined;
			queue.push_back({ i,j });
			std::push_heap(queue.begin(), queue.end(), priority);
		}
	}

	dist[0][0] = 0;

	while (!queue.empty()) {
		auto u = queue.front();
		std::pop_heap(queue.begin(), queue.end(), priority);
		queue.pop_back();

		if (u == target) break;

		for (auto [dx, dy] : neighbours) {
			auto v = point{ u.x + dx, u.y + dy };
			if (std::ranges::find(queue, v) != queue.end()) {
				auto alt = dist[u.x][u.y] + map[v.x][v.y];
				if (alt < dist[v.x][v.y]) {
					dist[v.x][v.y] = alt;
					prev[v.x][v.y] = u;
					std::make_heap(queue.begin(), queue.end(), priority);
				}
			}
		}
	}

	std::vector<point> S;
	auto u = target;

	if (prev[u.x][u.y] != undefined or u == point{0,0}) {
		while (u != undefined) {
			S.push_back(u);
			u = prev[u.x][u.y];
		}
	}

	std::cout << std::accumulate(S.begin(), S.end(), 0, [&](auto acc, auto p) { return acc + map[p.x][p.y]; }) - map[0][0];

}
void part1(std::istream& input) {
	dijkstra(parse_map_p1(input));
}


void part2(std::istream& input) {
	dijkstra(parse_map_p2(input));
}

int main() {
	std::ifstream input{ "C:/Users/Sy/source/repos/aoc-2021/aoc-2021/day15/input.txt" };
	part2(input);
}