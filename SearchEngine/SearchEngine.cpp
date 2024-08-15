#include "SearchEngine.h"

SearchEngine::SearchEngine()
{
	_lemmatizer.LoadBinary("english.bin");
	_queryParser.SetLemmatizer(&_lemmatizer);
}

void SearchEngine::LoadRepositoryFromBinaryFile(const std::string& fileName)
{
	_webPageRepository.LoadFromBinaryFile(fileName + ".repository");
	_index.LoadFromBinaryFile(fileName + ".index");
}

// searchResults: Map of Web Pages with their Titles and Keyword Frequencies
//   - Key: Web Page URL
//   - Value: Pair containing:
//       - Web Page Title
//       - Pair containing:
//           - Word Frequency Map (Word -> Frequency)
//           - Total Frequency of all Keywords 
std::unordered_map<std::string, std::pair<std::string, std::pair<std::unordered_map<std::string, int>, int>>> SearchEngine::Search(std::string& query)
{
    std::unordered_map<std::string, std::pair<std::string, std::pair<std::unordered_map<std::string, int>, int>>> searchResults;

    std::vector<std::string> parsedSearchQuery = _queryParser.Parse(query);

    for (const auto& word : parsedSearchQuery)
    {
        std::vector<std::pair<std::string, int>> tokenFrequencies = _index.GetTokenFrequency(word);

        for (const auto& result : tokenFrequencies)
        {
            // Use the web page URL as the key
            std::string webPageUrl = result.first;

            // Get the web page title
            std::string webPageTitle = _webPageRepository.GetWebPageTitle(webPageUrl);

            // Store the web page title
            searchResults[webPageUrl].first = webPageTitle;

            // Store the frequency count of the individual word
            searchResults[webPageUrl].second.first[word] += result.second;

            // Store the total frequency of all keywords from the query
            searchResults[webPageUrl].second.second += result.second;
        }
    }
    return searchResults;
}

size_t SearchEngine::GetTotalSearchableWebPages()
{
    return _webPageRepository.GetWebPageRepositoryCount();
}

std::string SearchEngine::GetWebPageTitleByUrl(const std::string& webPageUrl)
{
   return _webPageRepository.GetWebPageTitle(webPageUrl);
}
