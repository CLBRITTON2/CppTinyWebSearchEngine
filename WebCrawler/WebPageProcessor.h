#pragma once
#include "WebPage.h"
#include <lexbor/html/html.h>
#include "RdrLemmatizer.h"
#include <queue>
#include <mutex>

class WebPageProcessor 
{
public:
	WebPageProcessor();
	void ProcessWebPage(WebPage& webPage, std::queue<string>& urlQueue);

private:
	static size_t WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData);
	void SerializeTextContent(lxb_dom_node_t* node, std::string& extractedText);
	void ExtractUrlsFromWebpage(lxb_dom_node_t* node, std::queue<string>& urlQueue);
	void CleanupLXB(lxb_html_parser_t* htmlParser, lxb_html_document_t* document);
	std::string GetContentFromHttpRequest(const std::string& webPageUrl, WebPage& webPage);
	std::string ExtractTextFromHtml(const std::string& webPageContent, std::queue<string>& urlQueue);
	RdrLemmatizer _lemmatizer;
	std::mutex _lemmatizerMutex;
};