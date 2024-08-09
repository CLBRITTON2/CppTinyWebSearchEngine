#pragma once
#include "WebPage.h"
#include <lexbor/html/html.h>
#include "RdrLemmatizer.h"

class WebPageProcessor 
{
public:
	void ProcessWebPage(WebPage& webPage);
	std::string GetContentFromHttpRequest(const std::string& webPageUrl, WebPage& webPage);
	std::string ExtractTextFromHtml(const std::string& webPageContent);
	void SetLemmatizer(RdrLemmatizer* lemmatizer);
private:
	static size_t WriteCallback(void* buffer, size_t bufferSize, size_t numberOfBlocks, void* userData);
	void SerializeTextContent(lxb_dom_node_t* node, std::string& extractedText);
	void ExtractUrlsFromWebpage(lxb_dom_node_t* node, std::ofstream& outputFile);
	void CleanupLXB(lxb_html_parser_t* htmlParser, lxb_html_document_t* document);
	RdrLemmatizer* _lemmatizer;
};