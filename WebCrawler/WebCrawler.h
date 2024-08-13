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

private:
	WebPageProcessor _webPageProcessor;
	InvertedIndex _index;
	WebPageRepository _webPageRepository;
	std::queue<string> _urlQueue;
	std::atomic<int> _webPageID;
	std::atomic<int> _totalPagesScraped;
	int _numberOfPagesToScrape;
	std::mutex _pageIdMutex;
	std::mutex _totalPagesScrapedMutex;
	std::mutex _urlQueueMutex;
};