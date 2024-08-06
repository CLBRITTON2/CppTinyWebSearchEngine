#pragma once

#include <iostream>
#include <string>
#include "WebPage.h"
#include "WebPageCrawler.h"
#include "InvertedIndex.h"
#include "RdrLemmatizer.h"
#include <unordered_map>

class SearchEngine
{
public:
	void IndexWebPage(WebPage& webPage);
	std::unordered_map<WebPage*, std::pair<std::unordered_map<std::string, int>, int>> Search(std::string& query);
	std::vector<std::string>& GetQueryKeyWords();

private:
	WebPageCrawler _crawler;
	InvertedIndex _index;
	std::map<int, std::shared_ptr<WebPage>> _webPages;
	std::vector<std::string> _queryKeyWords;
	WebPage* GetWebPageById(int webPageId);
	void ParseQueryKeywords(std::string& query);
};