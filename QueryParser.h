#pragma once
#include <vector>
#include <string>

class QueryParser
{

private:
	std::vector<std::string> _stopWords;
	void LoadStopWords(std::string_view fileName);
public:
	std::vector<std::string> Parse(std::string& query);
};