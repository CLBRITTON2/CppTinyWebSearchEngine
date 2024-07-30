#pragma once

#include <iostream>
#include <string>
#include "WebPage.h"
#include "WebPageCrawler.h"
#include "InvertedIndex.h"

class SearchEngine
{
public:
	void IndexWebPage(const WebPage& webPage);
	std::vector<std::pair<WebPage, int>> Search(const std::string& query, int numberOfResults);

private:
	WebPageCrawler _crawler;
	InvertedIndex _index;
};