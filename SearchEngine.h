#pragma once

#include "WebPage.h"
#include "WebPageProcessor.h"
#include "InvertedIndex.h"
#include "RdrLemmatizer.h"
#include "WebPageRepository.h"
#include "QueryParser.h"

class SearchEngine
{
public:
	SearchEngine();
	void SaveRepositoryToBinaryFile(const std::string& fileName);
	void LoadRepositoryFromBinaryFile(const std::string& fileName);
	void IndexWebPage(WebPage& webPage);
	std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> Search(std::string& query);

private:
	WebPageProcessor _webPageProcessor;
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	QueryParser _queryParser;
	RdrLemmatizer _lemmatizer;
};