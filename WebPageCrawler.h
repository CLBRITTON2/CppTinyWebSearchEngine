#pragma once
#include <string>
#include "WebPage.h"
#include <lexbor/html/html.h>
#include "RdrLemmatizer.h"

class WebPageCrawler 
{
public:
	void Crawl(const std::string& webPageUrl, WebPage& webPage);
	std::string ExtractTextFromHtml(const std::string& webPageContent);
	void SetLemmatizer(RdrLemmatizer* lemmatizer);
private:
	static size_t WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData);
	void SerializeTextContent(lxb_dom_node_t* node, std::string& extractedText, std::ofstream& outputFile);
	RdrLemmatizer* _lemmatizer;
};