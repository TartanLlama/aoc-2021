#pragma once

#include <ranges>
#include <sstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <tl/to.hpp>

template <class T>
auto split(std::string const& s, char delim) {
	return s | std::views::split(delim)
		| std::views::transform([](auto s) {
		std::stringstream ss;
		ss.str(std::string(s.begin(), s.end()));
		T t;
		ss >> t;
		return t;
			}) | tl::to<std::vector>();
}

template <class T>
auto split(std::istream& stream, char delim) {
	std::string s(std::istreambuf_iterator<char>(stream), {});
	return split<T>(s, delim);
}