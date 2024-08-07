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

class SearchEngine
{
public:
	void IndexWebPage(WebPage& webPage);
	std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> Search(std::string& query);
	std::vector<std::string>& GetQueryKeyWords();

private:
	WebPageCrawler _crawler;
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	std::vector<std::string> _queryKeyWords;
	void ParseQueryKeywords(std::string& query);
};