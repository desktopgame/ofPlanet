#include "parser.hpp"
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <cctype>

namespace csvr {
Parser::Parser() : tables() {
}
void Parser::parse(const std::string & source) {
	int start = 0;
	while (true) {
		Table table;
		int read = parse(start, source, table);
		if (start >= static_cast<int>(source.size())) {
			break;
		}
		start += read;
		this->tables.emplace_back(table);
	}
}
Table & Parser::getTableAt(int index) {
	return tables.at(index);
}
const Table & Parser::getTableAt(int index) const {
	return tables.at(index);
}
int Parser::getTableCount() const {
	return static_cast<int>(tables.size());
}

int Parser::parse(int start, const std::string & source, Table & table) {
	int len = static_cast<int>(source.size());
	int read = 0;
	Line line;
	std::stringstream sbuf;
	for (int i = start; i < len; i++) {
		char c = source.at(i);
		read++;
		if (c == separator) {
			line.emplace_back(sbuf.str());
			sbuf.str("");
			sbuf.clear(std::stringstream::goodbit);
		}
		else if (c == newline) {
			if (i == 0) { continue; }
			auto str = sbuf.str();
			if (isNullOrEmpty(str)) {
				return read;
			}
			else {
				line.emplace_back(str);
				sbuf.str("");
				sbuf.clear(std::stringstream::goodbit);
				table.emplace_back(line);
				line.clear();
			}
		}
		else {
			sbuf << c;
		}
	}
	return read;
}
bool Parser::isNullOrEmpty(const std::string & str) {
	if (str.empty()) {
		return true;
	}
	for (auto c : str) {
		if (!std::isspace(c)) {
			return false;
		}
	}
	return true;
}
}