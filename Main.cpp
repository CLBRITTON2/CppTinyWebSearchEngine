#include <iostream>
#include "SearchEngine.h"

int main()
{
    // Create a SearchEngine instance
    SearchEngine searchEngine;

    // Create some WebPage instances
    WebPage webPage1("https://en.wikipedia.org/wiki/Search_engine", 1, "");
    WebPage webPage2("https://en.wikipedia.org/wiki/Software_system", 2, "");
    WebPage webPage3("https://en.wikipedia.org/wiki/Software", 3, "");
    WebPage webPage4("https://en.wikipedia.org/wiki/Source_code", 4, "");

    // Index the web pages
    searchEngine.IndexWebPage(webPage1);
    searchEngine.IndexWebPage(webPage2);
    searchEngine.IndexWebPage(webPage3);
    searchEngine.IndexWebPage(webPage4);

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

