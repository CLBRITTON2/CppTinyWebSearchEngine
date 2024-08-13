#include "WebCrawler.h"
#include <thread>

void WebCrawler::SaveRepositoryToBinaryFile(const std::string& fileName)
{
	_webPageRepository.SaveToBinaryFile(fileName + ".repository");
	_index.SaveToBinaryFile(fileName + ".index");
	std::cout << "Repository and index have been saved to binary" << std::endl;
}

void WebCrawler::LoadRepositoryFromBinaryFile(const std::string& fileName)
{
	_webPageRepository.LoadFromBinaryFile(fileName + ".repository");
	_index.LoadFromBinaryFile(fileName + ".index");
}

void WebCrawler::SetNumberOfPagesToScrape(int numberOfPagesToScrape)
{
	_numberOfPagesToScrape = numberOfPagesToScrape;
}

// Synchronize multi threaded logging - only required for printing in diff threads
void WebCrawler::Log(const std::string& message)
{
	std::lock_guard<std::mutex> lock(_logMutex);
	std::cout << message << std::endl;
}

bool WebCrawler::IndexWebPage(WebPage& webPage)
{
	// Dont try processing a page if we've already hit our predefined max
	if (_totalPagesScraped >= _numberOfPagesToScrape)
	{
		return false;
	}

	// Check to see if we've already indexed this page - no need for duplicates
	if (_webPageRepository.IsWebPagedIndexed(webPage.GetWebPageUrl()))
	{
		// We don't want to incement the web page ID if the page is already indexed
		Log("Web Page: " + webPage.GetWebPageUrl() + ", has already been indexed - moving on...");
		return false;
	}
	else
	{
		// Page hasn't been indexed - process it
		_webPageProcessor.ProcessWebPage(webPage, _urlQueue);
		_index.TokenizeWebPageContent(webPage);
		_webPageRepository.AddWebPage(webPage);
		return true;
	}
}

WebCrawler::WebCrawler()
	:_totalPagesScraped{0}, _numberOfPagesToScrape{0}
{
	// Seed URL
	_urlQueue.push("https://www.wikipedia.org/");
}

void WebCrawler::Crawl()
{
	Log("Crawler started...");

	std::vector<std::thread> threads;
	int batchSize = 100;

	// Used for managing batches - not total
	int webPagesProcessedCount = 0;

	// Only process the number of pages configured by the user
	while (_totalPagesScraped < _numberOfPagesToScrape)
	{
		// Process web pages in batches of 100 to manage threads
		while (!_urlQueue.empty() && threads.size() < batchSize)
		{
			// Exit loop - we've scraped the desired number of pages
			if (_totalPagesScraped >= _numberOfPagesToScrape)
			{
				break;
			}

			std::string url = _urlQueue.front();
			_urlQueue.pop();

			// Create a thread to index each page - page indexing can get expensive
			threads.emplace_back([this, url]()
				{
					try
					{
						// Create the web page to index
						WebPage webPage(url);

						// Returns true if the page hasn't already been indexed
						if (IndexWebPage(webPage))
						{
							std::lock_guard<std::mutex> pagesScrapedLock(_totalPagesScrapedMutex);
							if (_totalPagesScraped < _numberOfPagesToScrape) 
							{
								_totalPagesScraped++;
								Log("Total number of pages scraped update: " + std::to_string(_totalPagesScraped));
							}
						}
					}
					catch (const std::exception& e)
					{
						Log("Error processing web page: " + std::string(e.what()));
					}
				});

			webPagesProcessedCount++;

			if (webPagesProcessedCount >= batchSize)
			{
				// Wait for threads to finish
				for (auto& thread : threads)
				{
					thread.join();
				}
				threads.clear();

				// Save repository by batch - saving at the end of the program could be expensive
				SaveRepositoryToBinaryFile("WebPages");

				// Reset the count - the batch has been saved
				webPagesProcessedCount = 0;

				// Check if the maximum number of pages has been reached
				if (_totalPagesScraped >= _numberOfPagesToScrape)
				{
					break;
				}
			}
		}

		// Check if there are still threads running
		if (!threads.empty())
		{
			// Join the remaining threads
			for (auto& thread : threads)
			{
				thread.join();
			}
			threads.clear();
		}
	}
	Log("Number of web pages scraped: " + std::to_string(_totalPagesScraped));
}