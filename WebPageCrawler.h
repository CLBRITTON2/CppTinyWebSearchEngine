#pragma once
#include <string>
#include "WebPage.h"

class WebPageCrawler 
{
public:
	void Crawl(const std::string& url, WebPage& webPage);
	std::string ExtractTextFromHtml(const std::string& webPageHtmlContent);
};