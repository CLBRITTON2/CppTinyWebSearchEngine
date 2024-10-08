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
static void PrintTop10SearchResults(std::unordered_map<std::string, std::pair<std::string, std::pair<std::unordered_map<std::string, int>, int>>>& searchResults, size_t totalSearchablePages)
{
	// Create a vector to store the search results with their frequencies
	std::vector<std::tuple<std::string, std::string, int>> topResults;

	// Iterate over the search results and add them to the vector
	for (const auto& result : searchResults)
	{
		topResults.emplace_back(result.first, result.second.first, result.second.second.second);
	}

	// Sort the vector in descending order based on the frequency which is the 3rd element of the tuple  (std::get<2> is getting the 3rd element aka frequency)
	std::sort(topResults.begin(), topResults.end(), [](const auto& a, const auto& b) { return std::get<2>(a) > std::get<2>(b); });

	std::cout << "________________________________________________________________________________________________________\n" << std::endl;
	std::cout << "Displaying \"top 10\" web pages out of: " << std::to_string((int)totalSearchablePages) << " based on your query: " << std::endl;
	std::cout << "________________________________________________________________________________________________________\n" << std::endl;

	// Print the top 10 search results
	int count = 0;
	for (const auto& result : topResults)
	{
		if (count >= 10)
		{
			break;
		}

		std::string webPageUrl = std::get<0>(result);
		std::string webPageTitle = std::get<1>(result);
		int totalMatches = std::get<2>(result);

		std::cout << "Web Page: " << webPageTitle << std::endl; 
		std::cout << "Total query word matches: " << totalMatches << std::endl;
		std::cout << "URL: " << webPageUrl << std::endl;
		std::cout << std::endl;
		count++;
	}

	if (count == 0)
	{
		std::cout << "No search results found." << std::endl;
	}

	std::cout << "________________________________________________________________________________________________________" << std::endl;
}

// Print all search results (should probably be used for nothing more than testing on small batches of data)
//static void PrintAllSearchResults(std::unordered_map<std::string, std::pair<std::string, std::pair<std::unordered_map<std::string, int>, int>>>& searchResults)
//{
//	for (const auto& result : searchResults)
//	{
//		std::string webPageUrl = result.first;
//		std::unordered_map<std::string, int> keywordFrequencies = result.second.first;
//		int totalFrequencyScore = result.second.second;
//
//		std::cout << "Web Page: " << webPageUrl << "\nFrequency of search terms: " << std::endl;
//
//		for (const auto& keyWordFrequency : keywordFrequencies)
//		{
//			std::cout << keyWordFrequency.first << ":" << keyWordFrequency.second << std::endl;
//		}
//
//		std::cout << "Total frequency score: " << totalFrequencyScore << std::endl;
//		std::cout << std::endl;
//	}
//}
//
//// Print the search result with the highest number of query matches
//static void PrintSearchResultsWithHigestQueryFrequency(std::unordered_map<std::string, std::pair<std::string, std::pair<std::unordered_map<std::string, int>, int>>>& searchResults)
//{
//	int highestFrequencyScore{0};
//	std::string highestScoringWebPage{""};
//
//	for (const auto& result : searchResults)
//	{
//		std::string currentWebPageUrl = result.first;
//		std::unordered_map<std::string, int> keywordFrequencies = result.second.first;
//		int currentSearchResultFrequencyScore = result.second.second;
//
//		if (currentSearchResultFrequencyScore > highestFrequencyScore)
//		{
//			highestFrequencyScore = currentSearchResultFrequencyScore;
//			highestScoringWebPage = currentWebPageUrl;
//		}
//	}
//
//	if (highestScoringWebPage != "")
//	{
//		std::cout << "Web Page: " << highestScoringWebPage << "\nHighest total query keyword match with:  " << highestFrequencyScore << " combined occurnces of queried terms on this web page" << std::endl;
//	}
//	else
//	{
//		std::cout << "No search results found." << std::endl;
//	}
//}

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
		std::cout << "Type \'exit\' to quit." << std::endl;
		std::cout << "Enter a search query: ";
		std::getline(std::cin, query);

		if (query == "exit")
		{
			break;
		}

		auto timeThree = high_resolution_clock::now();
		std::unordered_map<std::string, std::pair<std::string, std::pair<std::unordered_map<std::string, int>, int>>> searchResults = searchEngine.Search(query);
		auto timeFour = high_resolution_clock::now();

		size_t totalSearchablePages = searchEngine.GetTotalSearchableWebPages();
		PrintTop10SearchResults(searchResults, totalSearchablePages);

		// Getting number of milliseconds as a double. 
		duration<double, std::milli> msToSearch = timeFour - timeThree;
		std::cout << std::endl;
		std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;
		std::cout << "________________________________________________________________________________________________________" << std::endl;
		std::cout << std::endl;

	}
	return 0;
}

