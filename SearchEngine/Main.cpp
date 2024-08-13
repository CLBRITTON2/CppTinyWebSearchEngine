#include <iostream>
#include "SearchEngine.h"
#include <fstream>
#include <thread>
#include <filesystem>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Print all search results
static void PrintAllSearchResults(std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>>& searchResults)
{
	for (const auto& result : searchResults)
	{
		std::shared_ptr<WebPage> webPage = result.first;
		std::unordered_map<std::string, int> keywordFrequencies = result.second.first;
		int totalFrequencyScore = result.second.second;

		std::cout << "Web Page ID: " << webPage->GetWebPageID() << ", Frequency of search terms: " << std::endl;

		for (const auto& keyWordFrequency : keywordFrequencies)
		{
			std::cout << keyWordFrequency.first << ":" << keyWordFrequency.second << std::endl;
		}

		std::cout << "Total frequency score: " << totalFrequencyScore << std::endl;
		std::cout << std::endl;
	}
}

// Print the search result with the highest number of query matches
static void PrintSearchResultsWithHigestQueryFrequency(std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>>& searchResults)
{
	int highestFrequencyScore = 0;
	std::shared_ptr<WebPage> highestScoringWebPage = nullptr;

	for (const auto& result : searchResults)
	{
		std::shared_ptr<WebPage> currentWebPage = result.first;
		std::unordered_map<std::string, int> keywordFrequencies = result.second.first;
		int currentSearchResultFrequencyScore = result.second.second;

		if (currentSearchResultFrequencyScore > highestFrequencyScore)
		{
			highestFrequencyScore = currentSearchResultFrequencyScore;
			highestScoringWebPage = currentWebPage;
		}
	}

	if (highestScoringWebPage != nullptr)
	{
		std::cout << "Web Page ID: " << highestScoringWebPage->GetWebPageID() << ", Highest total query keyword match with:  " << highestFrequencyScore << " combined occurnces of queried terms on this web page" << std::endl;
		std::cout << "URL: " << highestScoringWebPage->GetWebPageUrl() << std::endl;
	}
	else
	{
		std::cout << "No search results found." << std::endl;
	}
}

int main()
{
	SearchEngine searchEngine;
	try
	{
		std::cout << "Loading binaries..." << std::endl;
		std::cout << std::endl;
		searchEngine.LoadRepositoryFromBinaryFile("WebPages");
	}
	catch (const boost::archive::archive_exception& e) 
	{
		std::cerr << "Deserialization failed: " << e.what() << std::endl;
	}

	while (true)
	{
		std::string query{""};
		std::cout << "Enter your search query (or 'exit' to quit): ";
		std::getline(std::cin, query);

		if (query == "exit")
		{
			break;
		}

		auto timeThree = high_resolution_clock::now();
		std::unordered_map<std::shared_ptr<WebPage>, std::pair<std::unordered_map<std::string, int>, int>> searchResults = searchEngine.Search(query);
		auto timeFour = high_resolution_clock::now();

		//PrintAllSearchResults(searchResults);
		std::cout << std::endl;
		PrintSearchResultsWithHigestQueryFrequency(searchResults);
		//PrintAllSearchResults(searchResults);

		// Getting number of milliseconds as a double. 
		duration<double, std::milli> msToSearch = timeFour - timeThree;
		std::cout << std::endl;
		std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;
		std::cout << std::endl;

	}
	return 0;
}

