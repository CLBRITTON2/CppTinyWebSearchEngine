#include <iostream>
#include "SearchEngine.h"
#include <chrono>
#include <fstream>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

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
    std::string query = "computer";

    auto timeThree = high_resolution_clock::now();
    std::vector<std::pair<WebPage*, int>> searchResults = searchEngine.Search(query);
    auto timeFour = high_resolution_clock::now();

    // Print the search results
    for (const auto& result : searchResults)
    {
        WebPage* webPage = result.first;
        int frequency = result.second;
        std::cout << "Web Page ID: " << webPage->GetWebPageID() << ", Frequency of search term \"" << query << "\": " << frequency << std::endl;
    }

    // Getting number of milliseconds as a double. 
    duration<double, std::milli> msToSearch = timeFour - timeThree;
    std::cout << std::endl;
    std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;

    return 0;
}

