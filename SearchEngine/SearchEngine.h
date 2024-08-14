#pragma once
#include "WebPage.h"
#include "QueryParser.h"
#include "SearchEngineSharedLibrary.h"

class SearchEngine
{
public:
	SearchEngine();
	void LoadRepositoryFromBinaryFile(const std::string& fileName);
	std::unordered_map<std::string, std::pair<std::unordered_map<std::string, int>, int>> Search(std::string& query);
	size_t GetTotalSearchableWebPages();

private:
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	QueryParser _queryParser;
	RdrLemmatizer _lemmatizer;
};