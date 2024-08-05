#include <iostream>
#include "SearchEngine.h"
#include <chrono>
#include <fstream>
#include <thread>
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

// Print all search results
static void PrintAllSearchResults(std::vector<std::pair<WebPage*, int>>& searchResults, std::string& query)
{
    for (const auto& result : searchResults)
    {
        WebPage* webPage = result.first;
        int frequency = result.second;
        std::cout << "Web Page ID: " << webPage->GetWebPageID() << ", Frequency of (some variant of) search term \"" << query << "\": " << frequency << std::endl;
    }
}

// Print the search result with the highest number of query matches
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


    // Create an empty vector of threads for each web page
    std::vector<std::thread> threads;
    int webPageID = 1;
    auto timeOne = high_resolution_clock::now();

    // Basically a C# foreach loop going over each line in the txt file containing urls
    for (auto& line : webPagesFromConfig)
    {
        // Create a thread for each web page so we can index every page concurrently, adds the thread to threads vector
        // This is a combination of AI/Stack overflow 
        // Might have to look at refactoring this as the project scales because we'll run out of threads
        threads.emplace_back([&searchEngine, &line, webPageID]() {
            WebPage webPage(line, webPageID);
            searchEngine.IndexWebPage(webPage);
            });
        webPageID++;
    }

    for (auto& thread : threads) 
    {
        thread.join();
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

    PrintAllSearchResults(searchResults, query);
    std::cout << std::endl;
    PrintSearchResultsWithHigestQueryFrequency(searchResults, query);

    // Getting number of milliseconds as a double. 
    duration<double, std::milli> msToSearch = timeFour - timeThree;
    std::cout << std::endl;
    std::cout << "It took " << msToSearch.count() << " ms to run the search query against the index" << std::endl;

    return 0;
}

