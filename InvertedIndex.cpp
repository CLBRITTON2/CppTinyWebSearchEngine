#include <iostream>
#include <sstream>
#include "InvertedIndex.h"
#include "WebPage.h"

void InvertedIndex::IndexWebPageContent(const WebPage& webPage)
{
	int webPageID = webPage.GetWebPageID();
	std::stringstream stringStream(webPage.GetWebPageContent());
	std::string token;

	// Read the content of the web page
	while (stringStream >> token)
	{
		// Increment the frequency of each token for the current web page
		_index[token][webPageID]++;
	}
}

std::vector<std::pair<int, int>> InvertedIndex::GetTokenFrequency(const std::string& query)
{
	std::vector<std::pair<int, int>> searchResults;

	if (_index.find(query) != _index.end())
	{
		for (const auto& pageIdFrequency : _index[query])
		{
			searchResults.push_back(pageIdFrequency);
		}
	}
	else
	{
		std::cout << "Key doesn't exist: Coldn't find any instance of \"" << query << "\" in provided web pages" << std::endl;
		return {};
	}
	return searchResults;
}
