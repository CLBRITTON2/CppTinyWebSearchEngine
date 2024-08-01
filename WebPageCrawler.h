#pragma once
#include <string>
#include "WebPage.h"
#include <lexbor/html/html.h>

class WebPageCrawler 
{
public:
	void Crawl(const std::string& webPageUrl, WebPage& webPage);
	std::string ExtractTextFromHtml(const std::string& webPageHtmlContent);
private:
	static size_t WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData);
	static unsigned int SerializerCallback(const lxb_char_t* data, size_t len, void* ctx);
};