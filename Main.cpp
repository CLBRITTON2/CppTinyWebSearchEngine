#include <iostream>
#include "SearchEngine.h"
#include <chrono>
#include <fstream>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

static void PrintAllSearchResults(std::vector<std::pair<WebPage*, int>>& searchResults, std::string& query)
{
    // Print all of the search results
    for (const auto& result : searchResults)
    {
        WebPage* webPage = result.first;
        int frequency = result.second;
        std::cout << "Web Page ID: " << webPage->GetWebPageID() << ", Frequency of (some variant of) search term \"" << query << "\": " << frequency << std::endl;
    }
}

static void PrintSearchResultsWithHigestQueryFrequency(std::vector<std::pair<WebPage*, int>>& searchResults, std::string& query)
{
    int highestFrequency = 0;
    WebPage* maxFrequencyWebPage = nullptr;

    for (const auto& result : searchResults)
    {
        WebPage* webPage = result.first;
        int frequency = result.second;

        if (frequency > highestFrequency)
        {
            highestFrequency = frequency;
            maxFrequencyWebPage = webPage;
        }
    }

    if (maxFrequencyWebPage != nullptr)
    {
        std::cout << "Web Page ID: " << maxFrequencyWebPage->GetWebPageID() << ", URL: " << maxFrequencyWebPage->GetWebPageUrl() << ", Highest Frequency of (some variant of) search term \"" << query << "\": " << highestFrequency << std::endl;
    }
    else
    {
        std::cout << "No search results found." << std::endl;
    }
}

int main()
{
    SearchEngine searchEngine;

    std::vector<std::string> webPagesFromConfig;
    std::string line;
    std::ifstream inFile("WebPages.txt");
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open config file." << std::endl;
        return -1;
    }

    // Add each URL from config file to a collection
    while (getline(inFile, line))
    {
        webPagesFromConfig.push_back(line);
    }

    auto timeOne = high_resolution_clock::now();
    int webPageID = 1;
    for (auto& line : webPagesFromConfig)
    {
        WebPage webPage(line, webPageID);
        searchEngine.IndexWebPage(webPage);
        webPageID++;
    }
    auto timeTwo = high_resolution_clock::now();

    // Getting number of milliseconds as a double. 
    duration<double, std::milli> msToIndex = timeTwo - timeOne;
    std::cout << "It took " << msToIndex.count() << " ms to get and index all web pages" << std::endl;
    std::cout << std::endl;

    // Search for a token
    std::string query = "Computer";

    auto timeThree = high_resolution_clock::now();
    std::vector<std::pair<WebPage*, int>> searchResults = searchEngine.Search(query);
    auto timeFour = high_resolution_clock::now();

    //PrintAllSearchResults(searchResults, query);
    PrintSearchResultsWithHigestQueryFrequency(searchResults, query);

    // Getting number of milliseconds as a double. 
    duration<double, std::milli> msToSearch = timeFour - timeThree;
    std::cout << std::endl;
    std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;

    return 0;
}

