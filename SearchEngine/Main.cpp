#include <iostream>
#include "SearchEngine.h"
#include <fstream>
#include <thread>
#include <filesystem>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Print the top 10 search results with the highest number of query matches
static void PrintTop10SearchResults(std::unordered_map<std::string, std::pair<std::unordered_map<std::string, int>, int>>& searchResults)
{
	// Create a vector to store the search results with their frequencies
	std::vector<std::pair<std::string, int>> topResults;

	// Iterate over the search results and add them to the vector
	for (const auto& result : searchResults)
	{
		topResults.emplace_back(result.first, result.second.second);
	}

	// Sort the vector in descending order based on the frequency
	std::sort(topResults.begin(), topResults.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

	std::cout << "________________________________________________________________________________________________________\n" << std::endl;
	std::cout << "Here are the top 10 web pages based on your query:\n" << std::endl;
	// Print the top 10 search results
	int count = 0;
	for (const auto& result : topResults)
	{
		if (count >= 10) break;
		std::cout << "Web Page: " << result.first << "\nTotal query keyword match: " << result.second << " combined occurrences of queried terms on this web page\n" << std::endl;
		count++;
	}

	if (count == 0)
	{
		std::cout << "No search results found." << std::endl;
	}

	std::cout << "________________________________________________________________________________________________________" << std::endl;
}

// Print all search results
static void PrintAllSearchResults(std::unordered_map<std::string, std::pair<std::unordered_map<std::string, int>, int>>& searchResults)
{
	for (const auto& result : searchResults)
	{
		std::string webPageUrl = result.first;
		std::unordered_map<std::string, int> keywordFrequencies = result.second.first;
		int totalFrequencyScore = result.second.second;

		std::cout << "Web Page: " << webPageUrl << "\nFrequency of search terms: " << std::endl;

		for (const auto& keyWordFrequency : keywordFrequencies)
		{
			std::cout << keyWordFrequency.first << ":" << keyWordFrequency.second << std::endl;
		}

		std::cout << "Total frequency score: " << totalFrequencyScore << std::endl;
		std::cout << std::endl;
	}
}

// Print the search result with the highest number of query matches
static void PrintSearchResultsWithHigestQueryFrequency(std::unordered_map<std::string, std::pair<std::unordered_map<std::string, int>, int>>& searchResults)
{
	int highestFrequencyScore{0};
	std::string highestScoringWebPage{""};

	for (const auto& result : searchResults)
	{
		std::string currentWebPageUrl = result.first;
		std::unordered_map<std::string, int> keywordFrequencies = result.second.first;
		int currentSearchResultFrequencyScore = result.second.second;

		if (currentSearchResultFrequencyScore > highestFrequencyScore)
		{
			highestFrequencyScore = currentSearchResultFrequencyScore;
			highestScoringWebPage = currentWebPageUrl;
		}
	}

	if (highestScoringWebPage != "")
	{
		std::cout << "Web Page: " << highestScoringWebPage << "\nHighest total query keyword match with:  " << highestFrequencyScore << " combined occurnces of queried terms on this web page" << std::endl;
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
		std::unordered_map<std::string, std::pair<std::unordered_map<std::string, int>, int>> searchResults = searchEngine.Search(query);
		auto timeFour = high_resolution_clock::now();

		//PrintSearchResultsWithHigestQueryFrequency(searchResults);
		PrintTop10SearchResults(searchResults);

		// Getting number of milliseconds as a double. 
		duration<double, std::milli> msToSearch = timeFour - timeThree;
		std::cout << std::endl;
		std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;
		std::cout << std::endl;

	}
	return 0;
}

