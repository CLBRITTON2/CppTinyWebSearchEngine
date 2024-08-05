#pragma once

#include <iostream>
#include <string>
#include "WebPage.h"
#include "WebPageCrawler.h"
#include "InvertedIndex.h"
#include "RdrLemmatizer.h"

class SearchEngine
{
public:
	void IndexWebPage(WebPage& webPage);
	std::vector<std::pair<WebPage*, int>> Search(std::string& query);

private:
	WebPageCrawler _crawler;
	InvertedIndex _index;
	std::map<int, std::shared_ptr<WebPage>> _webPages;
	WebPage* GetWebPageById(int webPageId);
};