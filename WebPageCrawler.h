#pragma once
#include <string>
#include "WebPage.h"
#include <lexbor/html/html.h>
#include "RdrLemmatizer.h"

class WebPageCrawler 
{
public:
	WebPageCrawler();
	void Crawl(const std::string& webPageUrl, WebPage& webPage);
	std::string ExtractTextFromHtml(const std::string& webPageContent);
private:
	static size_t WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData);
	void SerializeTextContent(lxb_dom_node_t* node, std::string& extractedText);
	RdrLemmatizer _lemmatizer;
};