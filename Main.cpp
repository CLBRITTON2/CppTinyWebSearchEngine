#include <iostream>
#include "SearchEngine.h"
#include <chrono>
#include <fstream>

int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    // Create a SearchEngine instance
    SearchEngine searchEngine;

    std::vector<std::string> webPagesFromConfig;
    std::string line;
    std::ifstream inFile("WebPages.txt");
    if (!inFile.is_open())
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }

    // Add each URL from config file to a collection
    while (getline(inFile, line))
    {
        webPagesFromConfig.push_back(line);
    }

    auto t1 = high_resolution_clock::now();
    int webPageID = 1;
    for (auto& line : webPagesFromConfig)
    {
        WebPage webPage(line, webPageID);
        searchEngine.IndexWebPage(webPage);
        webPageID++;
    }
    auto t2 = high_resolution_clock::now();

    // Getting number of milliseconds as a double. 
    duration<double, std::milli> ms_double = t2 - t1;
    std::cout << "It took " << ms_double.count() << " ms to get and index all web pages" << std::endl;

    // Search for a token
    std::string query = "the";
    std::vector<std::pair<WebPage*, int>> searchResults = searchEngine.Search(query);

    // Print the search results
    for (const auto& result : searchResults)
    {
        WebPage* webPage = result.first;
        int frequency = result.second;
        std::cout << "Web Page ID: " << webPage->GetWebPageID() << ", Frequency of search term \"" << query << "\": " << frequency << std::endl;
    }

    return 0;
}

