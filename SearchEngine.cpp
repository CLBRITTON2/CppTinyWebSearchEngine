#include "SearchEngine.h"
#include <algorithm>

void SearchEngine::IndexWebPage(WebPage& webPage)
{
	_crawler.Crawl(webPage.GetWebPageUrl(), webPage);
	_webPages[webPage.GetWebPageID()] = std::make_unique<WebPage>(webPage);
	_index.IndexWebPageContent(webPage);
}

std::vector<std::pair<WebPage*, int>> SearchEngine::Search(std::string& query)
{
	// Brute force query to lowercase so we can query case agnostic
	std::transform(query.begin(), query.end(), query.begin(), [](unsigned char c) { return std::tolower(c); });

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
