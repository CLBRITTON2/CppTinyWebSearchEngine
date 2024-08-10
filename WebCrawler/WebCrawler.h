#pragma once
#include <SearchEngineSharedLibrary.h>
#include "WebPageProcessor.h"

class WebCrawler
{
public:
	void SaveRepositoryToBinaryFile(const std::string& fileName);
	void LoadRepositoryFromBinaryFile(const std::string& fileName);
	void IndexWebPage(WebPage& webPage);

private:
	WebPageProcessor _webPageProcessor;
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	RdrLemmatizer _lemmatizer;
};