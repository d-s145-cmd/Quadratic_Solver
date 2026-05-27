#pragma once
#include <string>
#include <utility>

int compareStrings(std::string x, std::string y);
std::string add(std::string x, std::string y);
std::string sub(std::string x, std::string y);
std::string mul(const std::string& x, const std::string& y);
std::pair<std::string, std::string> divmod(const std::string& x, const std::string& y);
