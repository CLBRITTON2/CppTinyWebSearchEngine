#include "SearchEngine.h"

void SearchEngine::IndexWebPage(WebPage& webPage)
{
	_crawler.Crawl(webPage.GetWebPageUrl(), webPage);
	_webPages[webPage.GetWebPageID()] = std::make_unique<WebPage>(webPage);
	_index.IndexWebPageContent(webPage);
}

std::vector<std::pair<WebPage*, int>> SearchEngine::Search(const std::string& query)
{
	std::vector<std::pair<int, int>> tokenFrequencies = _index.GetTokenFrequency(query);

	std::vector<std::pair<WebPage*, int>> searchResults;

	// Iterate over tokenFreqeuncies vector
	for (const auto& result : tokenFrequencies)
	{
		WebPage* webPage = GetWebPageById(result.first);
		if (webPage != nullptr)
		{
			searchResults.push_back(std::make_pair(webPage, result.second));
		}
	}
	return searchResults;
}

WebPage* SearchEngine::GetWebPageById(int webPageId)
{
	if (_webPages.find(webPageId) != _webPages.end())
	{
		return _webPages[webPageId].get();
	}
	else
	{
		std::cout << "No web page found returning null pointer - probably going to crash when something tries to access it";
		return nullptr;
	}
}
