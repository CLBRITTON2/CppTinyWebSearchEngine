#pragma once
#include <vector>
#include <string>
#include "RdrLemmatizer.h"

class QueryParser
{
private:
	std::vector<std::string> _stopWords;
	void LoadStopWords(std::string_view fileName);
	RdrLemmatizer* _lemmatizer;
public:
	QueryParser();
	std::vector<std::string> Parse(std::string& query);
	void SetLemmatizer(RdrLemmatizer* lemmatizer);
};