#pragma once
#include <SearchEngineSharedLibrary.h>
#include "WebPageProcessor.h"
#include <atomic>

class WebCrawler
{
public:
	bool IndexWebPage(WebPage& webPage);
	WebCrawler();
	void Crawl();
	void SaveRepositoryToBinaryFile(const std::string& fileName);
	void LoadRepositoryFromBinaryFile(const std::string& fileName);
	void SetNumberOfPagesToScrape(int numberOfPagesToScrape);
	void Log(const std::string& message);

private:
	WebPageProcessor _webPageProcessor;
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	std::queue<string> _urlQueue;
	std::atomic<int> _totalPagesScraped;
	std::mutex _totalPagesScrapedMutex;
	int _numberOfPagesToScrape;
	std::mutex _logMutex;
};