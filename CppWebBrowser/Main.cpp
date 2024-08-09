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



	/*
	_____________________________________________________________________________________________________________________________________________________________
	TEST LOGIC FOR ADDING WEB PAGES FROM A TXT FILE MANUALLY BEGIN - Comment this entire block out once binaries are created - simulates web crawling
	_____________________________________________________________________________________________________________________________________________________________
	*/

	//std::vector<std::string> webPagesFromConfig;
	//std::string line;
	//std::ifstream inFile("WebPages.txt");
	//if (!inFile.is_open())
	//{
	//    std::cerr << "Failed to open config file." << std::endl;
	//    return -1;
	//}

	//// Add each URL from config file to a collection
	//while (getline(inFile, line))
	//{
	//    webPagesFromConfig.push_back(line);
	//}

	//// Create an empty vector of threads for each web page
	//std::vector<std::thread> threads;
	//int webPageID = 1;
	//auto timeOne = high_resolution_clock::now();

	//// Basically a C# foreach loop going over each line in the txt file containing urls
	//for (auto& line : webPagesFromConfig)
	//{
	//    // Create a thread for each web page so we can index every page concurrently, adds the thread to threads vector
	//    // This is a combination of AI/Stack overflow 
	//    // Might have to look at refactoring this as the project scales because we'll run out of threads
	//    threads.emplace_back([&searchEngine, &line, webPageID]() {
	//        WebPage webPage(line, webPageID);
	//        searchEngine.IndexWebPage(webPage);
	//        });
	//    webPageID++;
	//}

	//for (auto& thread : threads) 
	//{
	//    thread.join();
	//}

	//auto timeTwo = high_resolution_clock::now();

	//// Getting number of milliseconds as a double. 
	//duration<double, std::milli> msToIndex = timeTwo - timeOne;
	//std::cout << "It took " << msToIndex.count() << " ms to get and index all web pages" << std::endl;
	//std::cout << std::endl;

/*
_____________________________________________________________________________________________________________________________________________________________
TEST LOGIC FOR ADDING WEB PAGES FROM A TXT FILE MANUALLY END
_____________________________________________________________________________________________________________________________________________________________
*/



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

		// Getting number of milliseconds as a double. 
		duration<double, std::milli> msToSearch = timeFour - timeThree;
		std::cout << std::endl;
		std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;
		std::cout << std::endl;

	}
	return 0;
}

