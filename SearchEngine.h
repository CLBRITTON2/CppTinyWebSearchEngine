#pragma once

#include <iostream>
#include <string>
#include "WebPage.h"
#include "WebPageCrawler.h"
#include "InvertedIndex.h"
#include "RdrLemmatizer.h"
#include <unordered_map>
#include <memory>
#include "WebPageRepository.h"
#include "QueryParser.h"

class SearchEngine
{
public:
	SearchEngine();
	void IndexWebPage(WebPage& webPage);
	std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> Search(std::string& query);

private:
	WebPageCrawler _crawler;
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	QueryParser _queryParser;
	RdrLemmatizer _lemmatizer;
};